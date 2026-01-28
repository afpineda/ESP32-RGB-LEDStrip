/**
 * @file LEDStrip.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 * @date 2025-08-27
 *
 * @copyright Under EUPL 1.2 License
 */

//-------------------------------------------------------------------
// Imports and globals
//-------------------------------------------------------------------

#include "LEDStrip.hpp"

//------------------------------------------------------------------------------
// ESP32 implementation
//------------------------------------------------------------------------------
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP_PLATFORM)
//------------------------------------------------------------------------------

#include <cassert>
#include <cstring>               // Required to set/copy memory
#include "esp_log.h"             // For LOG_E()
#include "driver/rmt_tx.h"       // For the RMT API
#include "driver/gpio.h"         // For GPIO_IS_VALID... and others
#include "esp_private/esp_clk.h" // To read the CPU frequency

#define LOG_TAG "LEDStrip"

//-------------------------------------------------------------------
// Active wait
//-------------------------------------------------------------------

// Each CPU instruction takes 6.25 nanoseconds in an ESP32 RISC-V @ 160 Mhz
// Each CPU instruction takes 4.16 nanoseconds in an ESP32 Xtensa @ 240 Mhz

#define CPU_INSTRUCTIONS_PER_LOOP 5

/**
 * @brief Active wait without context switching
 *
 * @note Not accurate but close
 *
 * @param n Time to wait in nanoseconds
 */
#define active_wait_ns(n)                         \
    for (uint32_t i = 0; i < n; i += ns_per_loop) \
        asm("");

/**
 * @brief LED strip implementation for the ESP32 architecture
 *
 */
class LEDStrip::Implementation
{
private:
    /// @brief Transmission configuration
    static constexpr rmt_transmit_config_t rmt_transmit_config = {
        .loop_count = 0,
        .flags = {
            .eot_level = 0,
            .queue_nonblocking = false}};

    /// @brief Clock resolution in hertz (1 tick=0.1 us=100ns)
    static constexpr uint32_t clockResolutionHz = 10000000;

    /// @brief RMT symbol count per encoded byte
    static constexpr size_t symbols_per_byte = 8;
    /// @brief RMT symbol count per pixel
    static constexpr size_t symbols_per_pixel = sizeof(Pixel) * symbols_per_byte;
    /// @brief Transmission handle
    rmt_channel_handle_t rmtHandle = nullptr;
    /// @brief Pixel encoder handle
    rmt_encoder_handle_t pixel_encoder_handle = nullptr;
    /// @brief Byte encoder configuration for pixel display
    rmt_bytes_encoder_config_t byte_enc_config{};
    /// @brief Configured pixel driver
    PixelDriver driver;
    /// @brief Nanoseconds per active wait loop
    static inline uint32_t ns_per_loop = 17;

public:
    /// @brief Global brightness correction factor in the range [1,256]
    uint16_t brightness = 256;
    /// @brief Working parameters of the LED matrix
    LedMatrixParameters params;

    /**
     * @brief Initialize the RMT hardware
     *
     * @param params Working parameters of the LED matrix
     * @param dataPin Data output pin
     * @param openDrain Wether to use open drain or not
     * @param useDMA Wether to use DMA or not
     * @param driver Pixel driver
     */
    void initialize(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain,
        bool useDMA,
        PixelDriver driver)
    {
        // Check parameters
        if (!GPIO_IS_VALID_OUTPUT_GPIO(dataPin))
        {
            ESP_LOGE(
                LOG_TAG,
                "Pin %d is not output-capable in LED strip/matrix",
                dataPin);
            abort();
        }

        // Configure the RMT channel
        rmt_tx_channel_config_t tx_config = {
            .gpio_num = (gpio_num_t)dataPin,
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = clockResolutionHz,
            .mem_block_symbols = 64, // Note: must be even
            .trans_queue_depth = 1,
            .intr_priority = 0,
            .flags{
                .invert_out = 0,
                .with_dma = (useDMA) ? 1 : 0,
                .io_loop_back = 0,
                .io_od_mode = (openDrain) ? 1 : 0,
                .allow_pd = 0}};
        esp_err_t err = rmt_new_tx_channel(&tx_config, &rmtHandle);
        // Check if there is no DMA support and fall back
        if (useDMA && (err == ESP_ERR_NOT_SUPPORTED))
        {
            tx_config.flags.with_dma = 0;
            err = rmt_new_tx_channel(&tx_config, &rmtHandle);
        }
        ESP_ERROR_CHECK(err);
        ESP_ERROR_CHECK(rmt_enable(rmtHandle));

        // Configure the byte encoder
        if (driver.bitEncodingHighToLow)
        {
            byte_enc_config.bit0.level0 = 1;
            byte_enc_config.bit0.level1 = 0;
            byte_enc_config.bit1.level0 = 1;
            byte_enc_config.bit1.level1 = 0;
        }
        else
        {
            byte_enc_config.bit0.level0 = 0;
            byte_enc_config.bit0.level1 = 1;
            byte_enc_config.bit1.level0 = 0;
            byte_enc_config.bit1.level1 = 1;
        }
        if (driver.msbFirst)
            byte_enc_config.flags.msb_first = 1;
        else
            byte_enc_config.flags.msb_first = 0;
        byte_enc_config.bit0.duration0 =
            driver.bit0FirstStageTime.count() / 100;
        byte_enc_config.bit0.duration1 =
            driver.bit0SecondStageTime.count() / 100;
        byte_enc_config.bit1.duration0 =
            driver.bit1FirstStageTime.count() / 100;
        byte_enc_config.bit1.duration1 =
            driver.bit1SecondStageTime.count() / 100;
        rmt_simple_encoder_config_t cfg{
            .callback = pixels_rmt_encoder,
            .arg = (void *)this,
            .min_chunk_size = symbols_per_pixel};
        ESP_ERROR_CHECK(
            rmt_new_simple_encoder(
                &cfg,
                &pixel_encoder_handle));

        // Initialize instance members
        this->driver = driver;
        this->params = params;
        syncWithCPUFrequency();
    } // initialize()

    /**
     * @brief Encode pixel data and apply the brightness reduction factor
     *
     * @note This encoder is quite similar to the built-in byte encoder
     *
     * @param data Pixel data in BGR format as stored in PixelVector
     * @param data_size Pixel data size in bytes
     * @param symbols_written Count of symbols previously written
     * @param symbols_free Count of symbols available in the transmit buffer
     * @param symbols Pointer to the transmit buffer
     * @param done Pointer to end of transaction flag
     * @param arg Pointer to the LEDMatrix::Implementation instance
     * @return size_t Symbols written
     */
    static size_t pixels_rmt_encoder(
        const void *data,
        size_t data_size,
        size_t symbols_written,
        size_t symbols_free,
        rmt_symbol_word_t *symbols,
        bool *done,
        void *arg)
    {
        size_t total_symbol_count = (data_size * symbols_per_byte);
        if (symbols_written >= total_symbol_count)
        {
            // Transaction finished
            *done = true;
            return 0;
        }
        else
        {
            LEDStrip::Implementation *instance =
                static_cast<LEDMatrix::Implementation *>(arg);
            const Pixel *pixel_ptr = static_cast<const Pixel *>(data);
            size_t previous_symbols_written = symbols_written;
            size_t pixelIndex = (symbols_written / symbols_per_pixel);
            while (
                (symbols_free >= symbols_per_pixel) &&
                (symbols_written < total_symbol_count))
            {
                uint8_t byte[3];
                size_t canonicalIndex =
                    instance->params.canonicalIndex(pixelIndex);
                byte[0] =
                    (pixel_ptr[canonicalIndex]
                         .byte0(instance->driver.pixelFormat) *
                     instance->brightness) >>
                    8;
                byte[1] =
                    (pixel_ptr[canonicalIndex]
                         .byte1(instance->driver.pixelFormat) *
                     instance->brightness) >>
                    8;
                byte[2] =
                    (pixel_ptr[canonicalIndex]
                         .byte2(instance->driver.pixelFormat) *
                     instance->brightness) >>
                    8;

                for (size_t byteIndex = 0; byteIndex < 3; byteIndex++)
                {
                    // Note: we write 8 bytes to symbols[] for each byte
                    int bitIndex =
                        (instance->driver.msbFirst)
                            ? (symbols_per_byte - 1)
                            : 0;
                    int bitIncrement = (instance->driver.msbFirst) ? -1 : 1;
                    while ((bitIndex >= 0) && (bitIndex < symbols_per_byte))
                    {
                        if ((1 << bitIndex) & byte[byteIndex])
                        {
                            // Bit 1
                            symbols[0].duration0 =
                                instance->byte_enc_config.bit1.duration0;
                            symbols[0].duration1 =
                                instance->byte_enc_config.bit1.duration1;
                            symbols[0].level0 =
                                instance->byte_enc_config.bit1.level0;
                            symbols[0].level1 =
                                instance->byte_enc_config.bit1.level1;
                        }
                        else
                        {
                            // Bit 0
                            symbols[0].duration0 =
                                instance->byte_enc_config.bit0.duration0;
                            symbols[0].duration1 =
                                instance->byte_enc_config.bit0.duration1;
                            symbols[0].level0 =
                                instance->byte_enc_config.bit0.level0;
                            symbols[0].level1 =
                                instance->byte_enc_config.bit0.level1;
                        }
                        symbols++;
                        bitIndex += bitIncrement;
                    }
                }
                symbols_written += symbols_per_pixel;
                symbols_free -= symbols_per_pixel;
                pixelIndex++;
            }
            // Note: when the return value is 0,
            // we ask for the rmt tx channel to free more buffer space
            return symbols_written - previous_symbols_written;
        }
    } // pixels_rmt_encoder()

    /**
     * @brief Send black color pixels for shutdown
     *
     * @param data Any non-null pointer as data is not required
     * @param data_size Count of pixels in bytes (pixel count*3)
     * @param symbols_written Count of symbols previously written
     * @param symbols_free Count of symbols available in the transmit buffer
     * @param symbols Pointer to the transmit buffer
     * @param done Pointer to end of transaction flag
     * @param arg Pointer to the LEDMatrix::Implementation instance
     * @return size_t Symbols written
     */
    static size_t shutdown_rmt_encoder(
        const void *data,
        size_t data_size,
        size_t symbols_written,
        size_t symbols_free,
        rmt_symbol_word_t *symbols,
        bool *done,
        void *arg)
    {
        size_t symbol_count = (data_size * 8);
        if (symbols_written >= symbol_count)
        {
            // Transaction finished
            *done = true;
            return 0;
        }
        else
        {
            LEDStrip::Implementation *instance =
                (LEDStrip::Implementation *)arg;
            size_t writeCount =
                (symbols_free <= (symbol_count - symbols_written))
                    ? symbols_free
                    : symbol_count - symbols_written;
            for (size_t i = 0; i < writeCount; i++)
            {
                symbols[i].duration0 = instance->byte_enc_config.bit0.duration0;
                symbols[i].duration1 = instance->byte_enc_config.bit0.duration1;
                symbols[i].level0 = instance->byte_enc_config.bit0.level0;
                symbols[i].level1 = instance->byte_enc_config.bit0.level1;
            }
            return writeCount;
        }
        return 0;
    } // shutdown_rmt_encoder()

    void show(const PixelVector &pixels)
    {
        ESP_ERROR_CHECK(
            rmt_transmit(
                rmtHandle,
                pixel_encoder_handle,
                pixels.data(),
                pixels.size() * sizeof(Pixel),
                &rmt_transmit_config));
        ESP_ERROR_CHECK(
            rmt_tx_wait_all_done(
                rmtHandle,
                -1));
        active_wait_ns(driver.restTime.count());
    } // show()

    void shutdown()
    {
        rmt_simple_encoder_config_t cfg{
            .callback = shutdown_rmt_encoder,
            .arg = (void *)this,
            .min_chunk_size = 1};
        rmt_encoder_handle_t shutdown_encoder_handle = nullptr;
        ESP_ERROR_CHECK(
            rmt_new_simple_encoder(
                &cfg,
                &shutdown_encoder_handle));
        if (shutdown_encoder_handle)
        {
            ESP_ERROR_CHECK(
                rmt_transmit(
                    rmtHandle,
                    shutdown_encoder_handle,
                    &cfg, // Note: not used
                    params.size() * sizeof(Pixel),
                    &rmt_transmit_config));
            ESP_ERROR_CHECK(
                rmt_tx_wait_all_done(
                    rmtHandle,
                    -1));
            active_wait_ns(driver.restTime.count());
            ESP_ERROR_CHECK(rmt_del_encoder(shutdown_encoder_handle));
        }
    } // shutdown()

    inline PixelDriver pixelDriver() const noexcept
    {
        return driver;
    }

    inline void move(Implementation &&source) noexcept
    {
        rmtHandle = source.rmtHandle;
        pixel_encoder_handle = source.pixel_encoder_handle;
        byte_enc_config = source.byte_enc_config;
        driver = source.driver;
        params = source.params;
        source.rmtHandle = nullptr;
        source.pixel_encoder_handle = nullptr;
    }

    Implementation() noexcept = default;
    Implementation(const Implementation &) noexcept = delete;
    Implementation &operator=(const Implementation &) noexcept = delete;
    Implementation(Implementation &&source) noexcept
    {
        move(::std::forward<Implementation>(source));
    }

    Implementation &operator=(Implementation &&source) noexcept
    {
        move(::std::forward<Implementation>(source));
        return *this;
    }

    ~Implementation()
    {
        if (pixel_encoder_handle)
            ESP_ERROR_CHECK(rmt_del_encoder(pixel_encoder_handle));
        if (rmtHandle)
        {
            ESP_ERROR_CHECK(rmt_disable(rmtHandle));
            ESP_ERROR_CHECK(rmt_del_channel(rmtHandle));
        }
    }

    static void syncWithCPUFrequency() noexcept
    {
        uint32_t cpu_freq = esp_clk_cpu_freq(); // In Hz
        assert(cpu_freq > 0);
        ns_per_loop = (1000000000 * CPU_INSTRUCTIONS_PER_LOOP) / cpu_freq;
        ns_per_loop++;
        ESP_LOGD(LOG_TAG, "ns_per_loop = %u", ns_per_loop);
    }
}; // ESP32 implementation class

//------------------------------------------------------------------------------
#else
#error There is not an LEDStrip implementation for your board
#endif

//------------------------------------------------------------------------------
// LEDStrip
//------------------------------------------------------------------------------

LEDStrip::~LEDStrip() = default;

LEDStrip::LEDStrip(LEDStrip &&source) : RgbLedController(::std::move(source))
{
    _impl = ::std::move(source._impl);
}

LEDStrip &LEDStrip::operator=(LEDStrip &&source)
{
    _impl = ::std::move(source._impl);
    return static_cast<LEDStrip &>(
        RgbLedController::operator=(::std::move(source)));
}

LEDStrip::LEDStrip(
    ::std::size_t pixelCount,
    int dataPin,
    bool openDrain,
    bool useDMA,
    PixelDriver pixelDriver,
    bool reversed) : RgbLedController(),
                     _impl{::std::make_unique<Implementation>()}
{
    LedMatrixParameters params =
        (reversed)
            ? basicReversedLedStriParameters
            : basicLedStriParameters;
    params.column_count = pixelCount;
    _impl->initialize(params,
                      dataPin,
                      openDrain,
                      useDMA,
                      pixelDriver);
}

LEDStrip::LEDStrip(
    const LedMatrixParameters &params,
    int dataPin,
    bool openDrain,
    bool useDMA,
    PixelDriver pixelDriver)
{
    _impl->initialize(params,
                      dataPin,
                      openDrain,
                      useDMA,
                      pixelDriver);
}

void LEDStrip::show(const PixelVector &pixels)
{
    _impl->show(pixels);
}

void LEDStrip::shutdown()
{
    _impl->shutdown();
}

PixelDriver LEDStrip::pixelDriver() const noexcept
{
    return _impl->pixelDriver();
}

uint8_t LEDStrip::brightness()
{
    return _impl->brightness - 1;
}

uint8_t LEDStrip::brightness(uint8_t value)
{
    uint8_t result = _impl->brightness - 1;
    _impl->brightness = value + 1;
    return result;
}

const LedMatrixParameters &LEDMatrix::parameters() const noexcept
{
    return _impl->params;
}

void LEDStrip::syncWithCPUFrequency()
{
    LEDStrip::Implementation::syncWithCPUFrequency();
}

PixelMatrix LEDMatrix::pixelMatrix(const Pixel &color) const noexcept
{
    PixelMatrix result(
        _impl->params.row_count,
        _impl->params.column_count,
        color);
    return result;
}