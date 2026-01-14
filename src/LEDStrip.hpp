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
 * @brief Custom LED strip (any pixel driver)
 *
 */
class LEDStrip : public RgbLedController
{
private:
    class Implementation; // https://cpppatterns.com/patterns/pimpl.html
    ::std::unique_ptr<Implementation> _impl;

public:
    /**
     * @brief Construct an LED strip using a custom pixel driver
     *
     * @param dataPin Data transmission pin number
     * @param openDrain True to use open drain output
     * @param useDMA True to use direct memory access (if available)
     * @param pixelDriver Working parameters of the pixel driver
     * @param reversed True if the physical arrangement of the pixels
     *                 is the inverse of their logical order
     */
    LEDStrip(
        int dataPin,
        bool openDrain,
        bool useDMA,
        PixelDriver pixelDriver,
        bool reversed);

    virtual ~LEDStrip();
    LEDStrip(LEDStrip &&);
    LEDStrip &operator=(LEDStrip &&);
    LEDStrip(const LEDStrip &) = delete;
    LEDStrip &operator=(const LEDStrip &) = delete;

    virtual void show(const PixelVector &pixels) override;
    virtual void shutdown(::std::size_t pixelCount) override;

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
};

//------------------------------------------------------------------------------

/// @brief LED strip using a WS2811 pixel driver
class WS2811LEDStrip : public LEDStrip
{
public:
    WS2811LEDStrip(
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(dataPin, openDrain, useDMA, WS2811, reversed) {}
    WS2811LEDStrip(WS2811LEDStrip &&) = default;
    WS2811LEDStrip &operator=(WS2811LEDStrip &&) = default;
};

/// @brief LED strip using a WS2812 pixel driver
class WS2812LEDStrip : public LEDStrip
{
public:
    WS2812LEDStrip(
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(dataPin, openDrain, useDMA, WS2812, reversed) {}
    WS2812LEDStrip(WS2812LEDStrip &&) = default;
    WS2812LEDStrip &operator=(WS2812LEDStrip &&) = default;
};

/// @brief LED strip using a WS2815 pixel driver
class WS2815LEDStrip : public LEDStrip
{
public:
    WS2815LEDStrip(
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(dataPin, openDrain, useDMA, WS2815, reversed) {}
    WS2815LEDStrip(WS2815LEDStrip &&) = default;
    WS2815LEDStrip &operator=(WS2815LEDStrip &&) = default;
};

/// @brief LED strip using an SK6812 pixel driver
class SK6812LEDStrip : public LEDStrip
{
public:
    SK6812LEDStrip(
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(dataPin, openDrain, useDMA, SK6812, reversed) {}
    SK6812LEDStrip(SK6812LEDStrip &&) = default;
    SK6812LEDStrip &operator=(SK6812LEDStrip &&) = default;
};

/// @brief LED strip using an UCS1903 pixel driver
class UCS1903LEDStrip : public LEDStrip
{
public:
    UCS1903LEDStrip(
        int dataPin,
        bool openDrain = true,
        bool useDMA = false,
        bool reversed = false)
        : LEDStrip(dataPin, openDrain, useDMA, UCS1903, reversed) {}
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

    virtual void shutdown(::std::size_t pixelCount) override
    {
        if (onShutdown)
            onShutdown(pixelCount);
    }
};
#endif