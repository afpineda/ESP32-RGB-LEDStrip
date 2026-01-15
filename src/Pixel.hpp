/**
 * @file Pixel.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-09-01
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include "PixelDriver.hpp"

//------------------------------------------------------------------------------

/**
 * @brief Pixel in 3-byte packed RGB format
 *
 */
struct Pixel
{
    /// @brief Blue channel
    uint8_t blue;
    /// @brief Green channel
    uint8_t green;
    /// @brief Red channel
    uint8_t red;

    /**
     * @brief Get the minimum channel value
     *
     * @return uint8_t Value of the channel with the lowest value
     */
    uint8_t min() const noexcept;

    /**
     * @brief Get the maximum channel value
     *
     * @return uint8_t Value of the channel with the highest value
     */
    uint8_t max() const noexcept;

    /**
     * @brief Get the average channel value
     *
     * @return uint8_t Average
     */
    uint8_t avg() const noexcept;

    /**
     * @brief Get the pixel luminance in the HSL model
     *
     * @return unsigned int Luminance in the range [0,255]
     */
    unsigned int luminance() const noexcept
    {
        return (min() + max()) / 2;
    }

    /**
     * @brief Get the pixel saturation in the HSL model
     *
     * @return uint8_t Saturation in the range [0,255]
     */
    uint8_t HSL_saturation() const noexcept;

    /**
     * @brief Get the pixel hue
     *
     * @return unsigned int Hue in degrees [0,359]
     */
    unsigned int hue() const noexcept;

    void hsl(unsigned int hue, uint8_t saturation, uint8_t luminance) noexcept;

public:
    /**
     * @brief Convert to packed RGB format
     *
     * @return uint32_t Packed RGB value
     */
    operator uint32_t() const noexcept
    {
        return (blue) | (green << 8) | (red << 16);
    }

    /**
     * @brief Convert to packed RGB format
     *
     * @return int Packed RGB value
     */
    operator int() const noexcept
    {
        return (blue) | (green << 8) | (red << 16);
    }

    /**
     * @brief Create from a packed RGB value
     *
     * @param packedRGB Packed RGB value
     */
    Pixel(uint32_t packedRGB) noexcept
    {
        red = packedRGB >> 16;
        green = packedRGB >> 8;
        blue = packedRGB;
    }

    /**
     * @brief Create as a black pixel
     *
     */
    Pixel() noexcept
    {
        red = 0;
        green = 0;
        blue = 0;
    }

    Pixel(const Pixel &) = default;
    Pixel(Pixel &&) = default;

    /**
     * @brief Assign a packed RGB color
     *
     * @param packedRGB Packed RGB value
     * @return int Non-zero
     */
    Pixel &operator=(uint32_t packedRGB) noexcept
    {
        red = packedRGB >> 16;
        green = packedRGB >> 8;
        blue = packedRGB;
        return *this;
    }

    Pixel &operator=(const Pixel &) = default;
    Pixel &operator=(Pixel &&) = default;

    /**
     * @brief Compare to a packed RGB color
     *
     * @param packedRGB Packed RGB value
     * @return true If this pixel matches @p packedRGB
     * @return false Otherwise
     */
    bool operator==(uint32_t packedRGB) const noexcept
    {
        return (packedRGB == static_cast<uint32_t>(*this));
    }

    /**
     * @brief Compare to a packed RGB color
     *
     * @param packedRGB Packed RGB value
     * @return true If this pixel matches @p packedRGB
     * @return false Otherwise
     */
    bool operator==(int packedRGB) const noexcept
    {
        return (packedRGB == static_cast<int>(*this));
    }

    /**
     * @brief Compare to another pixel
     *
     * @param other Other pixel
     * @return true If this pixel matches @p other
     * @return false Otherwise
     */
    bool operator==(const Pixel &other) const noexcept
    {
        return (red == other.red) &&
               (blue == other.blue) &&
               (green == other.green);
    }

    /**
     * @brief Compare to a packed RGB color
     *
     * @param packedRGB Packed RGB value
     * @return true If this pixel does not match @p packedRGB
     * @return false Otherwise
     */
    bool operator!=(uint32_t packedRGB) const noexcept
    {
        return (packedRGB != static_cast<uint32_t>(*this));
    }

    /**
     * @brief Compare to a packed RGB color
     *
     * @param packedRGB Packed RGB value
     * @return true If this pixel does not match @p packedRGB
     * @return false Otherwise
     */
    bool operator!=(int packedRGB) const noexcept
    {
        return (packedRGB != static_cast<int>(*this));
    }

    /**
     * @brief Compare to another pixel
     *
     * @param other Other pixel
     * @return true If this pixel does not match @p other
     * @return false Otherwise
     */
    bool operator!=(const Pixel &other) const noexcept
    {
        return (red != other.red) ||
               (blue != other.blue) ||
               (green != other.green);
    }

    /**
     * @brief Get the first color channel in a certain pixel format
     *
     * @param format Pixel format
     * @return uint8_t Color channel
     */
    uint8_t byte0(PixelFormat format) const noexcept;

    /**
     * @brief Get the second color channel in a certain pixel format
     *
     * @param format Pixel format
     * @return uint8_t Color channel
     */
    uint8_t byte1(PixelFormat format) const noexcept;

    /**
     * @brief Get the third color channel in a certain pixel format
     *
     * @param format Pixel format
     * @return uint8_t Color channel
     */
    uint8_t byte2(PixelFormat format) const noexcept;

    /**
     * @brief Reduce brightness
     *
     * @param brightness_factor A brightness reduction factor.
     *                          255 will not reduce brightness.
     *                          0 will reduce brightness to black.
     */
    void dim(uint8_t brightness_factor) noexcept;
};

static_assert(sizeof(Pixel) == 3);
