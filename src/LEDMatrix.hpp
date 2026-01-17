/**
 * @file LEDMatrix.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2026-01-17
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include "RgbLedController.hpp"
#include "PixelMatrix.hpp"
#include <memory> // For ::std::unique_ptr

//------------------------------------------------------------------------------

/**
 * @brief LED Matrix
 *
 */
class LEDMatrix : public RgbLedController
{
protected:
    class Implementation;
    ::std::unique_ptr<Implementation> _impl;

public:
    /**
     * @brief Construct an LED matrix
     *
     * @param dataPin Data transmission pin number
     * @param openDrain True to use open drain output
     * @param useDMA True to use direct memory access (if available)
     * @param pixelDriver Working parameters of the pixel driver
     * @param params Working parameters of the LED matrix
     */
    LEDMatrix(
        int dataPin,
        bool openDrain,
        bool useDMA,
        PixelDriver pixelDriver,
        const LedMatrixParameters &params);

    virtual ~LEDMatrix();
    LEDMatrix(LEDMatrix &&);
    LEDMatrix &operator=(LEDMatrix &&);
    LEDMatrix(const LEDMatrix &) = delete;
    LEDMatrix &operator=(const LEDMatrix &) = delete;

    virtual void show(const PixelVector &pixels) override;
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
     * @brief Get the working parameters
     */
    const LedMatrixParameters &parameters() const noexcept;

    /**
     * @brief Retrieve a pixel matrix suitable for this LED matrix
     *
     * @param color Initial color for all pixels
     * @return PixelMatrix Pixel matrix object
     */
    PixelMatrix pixelMatrix(const Pixel &color = 0) const noexcept;

    /**
     * @brief Synchronize the timings of the LED strip
     *        with the current CPU frequency.
     *
     * @warning You must call this method if the CPU frequency changes
     */
    static void syncWithCPUFrequency();
}; // LEDMatrix

//------------------------------------------------------------------------------
