/**
 * @file LEDStrip.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-10-21
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include "RgbLedController.hpp"
#include <memory> // For ::std::unique_ptr

#ifdef CD_CI
#include <functional> // For testing
#endif

//------------------------------------------------------------------------------

/**
 * @brief Custom LED strip or LED matrix (any pixel driver)
 *
 */
class LEDStrip : public RgbLedController
{
private:
    /// @brief Private implementation type
    class Implementation; // https://cpppatterns.com/patterns/pimpl.html
    /// @brief Private implementation instance
    ::std::unique_ptr<Implementation> _impl;

public:
    /**
     * @brief Construct an LED strip using a custom pixel driver
     *
     * @param pixelCount Number of pixels in the LED strip.
     *                   Do not pass an higher value than needed.
     * @param dataPin Data transmission pin number
     * @param openDrain True to use open drain output
     * @param useDMA True to use direct memory access (if available)
     * @param pixelDriver Working parameters of the pixel driver
     * @param reversed True if the physical arrangement of the pixels
     *                 is the inverse of their logical order
     */
    LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain,
        bool useDMA,
        PixelDriver pixelDriver,
        bool reversed);

    /**
     * @brief Construct an LED matrix (2D LED strip)
     *
     * @param params Working parameters of the LED matrix
     * @param dataPin Data transmission pin number
     * @param openDrain True to use open drain output
     * @param useDMA True to use direct memory access (if available)
     * @param pixelDriver Working parameters of the pixel driver
     */
    LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain,
        bool useDMA,
        PixelDriver pixelDriver);

    /// @brief Destroy the LED strip/matrix
    virtual ~LEDStrip();

    /// @brief Transfer ownership of this LED strip/matrix via constructor
    /// @param from Instance transfering ownership
    LEDStrip(LEDStrip &&from);

    /// @brief Transfer ownership of this LED strip/matrix via assignment
    /// @param  from Instance transfering ownership
    /// @return This instance
    LEDStrip &operator=(LEDStrip &&from);

    LEDStrip(const LEDStrip &) = delete;
    LEDStrip &operator=(const LEDStrip &) = delete;

    virtual void show(const PixelVector &pixels) override;

    /**
     * @brief Turn all LEDs off
     *
     * @note Ignores any display guard.
     */
    void shutdown();

    /**
     * @brief Get the global brightness reduction factor
     *
     * @note Defaults to maximum brightness (255)
     *
     * @return uint8_t Current brightness reduction factor.
     *                 255 means maximum brightness.
     *                 0 means no brightness (display is disabled).
     */
    virtual uint8_t brightness();

    /**
     * @brief Set the global brightness reduction factor
     *
     * @param value  New brightness reduction factor.
     *               255 means maximum brightness.
     *               0 means no brightness (display is disabled).
     * @return uint8_t Previous brightness reduction factor.
     */
    virtual uint8_t brightness(uint8_t value);

    /**
     * @brief Get the configured pixel driver
     *
     * @return PixelDriver Pixel driver
     */
    PixelDriver pixelDriver() const noexcept;

    /**
     * @brief Synchronize the timings of the LED strip
     *        with the current CPU frequency.
     *
     * @warning You must call this method if the CPU frequency changes
     */
    static void syncWithCPUFrequency();

    /**
     * @brief Get the LED matrix working parameters
     *
     * @return const LedMatrixParameters& A read-only copy of
     *                                    the working parameters.
     *                                    If this is an 1D led strip,
     *                                    it returns the equivalent LED matrix
     *                                    parameters, having a single row.
     */
    const LedMatrixParameters &parameters() const noexcept;

    /**
     * @brief Retrieve a suitable pixel matrix for this LED strip
     *
     * @param color Initial color for all pixels
     * @return PixelMatrix Pixel matrix object
     */
    PixelMatrix pixelMatrix(const Pixel &color = 0) const noexcept;
};

typedef LEDStrip LEDMatrix;

//------------------------------------------------------------------------------

/// @brief LED strip using a WS2811 pixel driver
class WS2811LEDStrip : public LEDStrip
{
public:
    WS2811LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(pixelCount, dataPin, openDrain, useDMA, WS2811, reversed) {}
    WS2811LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false)
        : LEDStrip(params, dataPin, openDrain, useDMA, WS2811) {}
    WS2811LEDStrip(WS2811LEDStrip &&) = default;
    WS2811LEDStrip &operator=(WS2811LEDStrip &&) = default;
};

/// @brief LED strip using a WS2812 pixel driver
class WS2812LEDStrip : public LEDStrip
{
public:
    WS2812LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(pixelCount, dataPin, openDrain, useDMA, WS2812, reversed) {}
    WS2812LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false)
        : LEDStrip(params, dataPin, openDrain, useDMA, WS2812) {}
    WS2812LEDStrip(WS2812LEDStrip &&) = default;
    WS2812LEDStrip &operator=(WS2812LEDStrip &&) = default;
};

/// @brief LED strip using a WS2815 pixel driver
class WS2815LEDStrip : public LEDStrip
{
public:
    WS2815LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(pixelCount, dataPin, openDrain, useDMA, WS2815, reversed) {}
    WS2815LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false)
        : LEDStrip(params, dataPin, openDrain, useDMA, WS2815) {}
    WS2815LEDStrip(WS2815LEDStrip &&) = default;
    WS2815LEDStrip &operator=(WS2815LEDStrip &&) = default;
};

/// @brief LED strip using an SK6812 pixel driver
class SK6812LEDStrip : public LEDStrip
{
public:
    SK6812LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(pixelCount, dataPin, openDrain, useDMA, SK6812, reversed) {}
    SK6812LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false)
        : LEDStrip(params, dataPin, openDrain, useDMA, SK6812) {}
    SK6812LEDStrip(SK6812LEDStrip &&) = default;
    SK6812LEDStrip &operator=(SK6812LEDStrip &&) = default;
};

/// @brief LED strip using an UCS1903 pixel driver
class UCS1903LEDStrip : public LEDStrip
{
public:
    UCS1903LEDStrip(
        ::std::size_t pixelCount,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(pixelCount, dataPin, openDrain, useDMA, UCS1903, reversed) {}
    UCS1903LEDStrip(
        const LedMatrixParameters &params,
        int dataPin,
        bool openDrain = true,
        bool useDMA = false)
        : LEDStrip(params, dataPin, openDrain, useDMA, UCS1903) {}
    UCS1903LEDStrip(UCS1903LEDStrip &&) = default;
    UCS1903LEDStrip &operator=(UCS1903LEDStrip &&) = default;
};

//------------------------------------------------------------------------------

#ifdef CD_CI
/**
 * @brief Dummy LED strip for testing
 *
 */
class DummyLEDStrip : public RgbLedController
{
public:
    /**
     * @brief Callback function triggered when show() is called
     *
     * @note Only available while testing
     */
    ::std::function<void(const PixelVector &pixels)> onShow;

    /**
     * @brief Callback function triggered when shutdown() is called
     *
     * @note Only available while testing
     */
    ::std::function<void(::std::size_t pixelCount)> onShutdown;

public:
    DummyLEDStrip() : RgbLedController() {}
    DummyLEDStrip(DummyLEDStrip &&) = default;
    DummyLEDStrip &operator=(DummyLEDStrip &&) = default;
    DummyLEDStrip(const DummyLEDStrip &) = delete;
    DummyLEDStrip &operator=(const DummyLEDStrip &) = delete;

    virtual void show(const PixelVector &pixels) override
    {
        if (onShow)
            onShow(pixels);
    }

    void shutdown(::std::size_t pixelCount)
    {
        if (onShutdown)
            onShutdown(pixelCount);
    }
};
#endif