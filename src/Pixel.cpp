/**
 * @file Pixel.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-09-01
 *
 * @copyright Under EUPL 1.2 License
 */

//------------------------------------------------------------------------------

#include "Pixel.hpp"
#include <cassert>
#include <cmath>

#if CD_CI
// For debugging
#include <iostream>
#endif

//------------------------------------------------------------------------------

uint8_t Pixel::min() const noexcept
{
    return ::std::min(::std::min(blue, green), red);
    // return (blue > green) ? (green > red ? red : green)
    //                       : (blue > red ? red : blue);
}

uint8_t Pixel::max() const noexcept
{
    return ::std::max(::std::max(blue, green), red);
    // return (blue < green) ? (green < red ? red : green)
    //                       : (blue < red ? red : blue);
}

uint8_t Pixel::avg() const noexcept
{
    return (blue + green + red) / 3;
}

uint8_t Pixel::HSL_saturation() const noexcept
{
    uint8_t max_ch = max();
    uint8_t min_ch = min();
    int chroma = max_ch - min_ch;
    if (chroma == 0)
        return 0;
    uint8_t lum = (max_ch + min_ch) / 2;
    if (lum <= 127)
        return (chroma * 255) / (max_ch + min_ch);
    else
        return (chroma * 255) / (510 - max_ch - min_ch);
}

unsigned int Pixel::hue() const noexcept
{
    int max_ch = max();
    int min_ch = min();
    int chroma = max_ch - min_ch;
    if (chroma == 0)
        return 0;
    else if (red == max_ch)
        return ((600 * (green - blue) / chroma) / 10) % 360;
    else if (green == max_ch)
        return (((600 * (blue - red) / chroma) / 10) + 120) % 360;
    else
        return (((600 * (red - green) / chroma) / 10) + 240) % 360;
}

void Pixel::hsl(
    unsigned int hue,
    uint8_t saturation,
    uint8_t luminance) noexcept
{
    assert((hue < 360) && "Hue value not in the [0,360) range");
    int c_255 = ((255 - ::std::abs((2 * luminance) - 255)) * saturation) / 254;
    // ::std::cout << "C= " << c_255 << ::std::endl;
    double x_aux =
        ::std::abs(::std::fmod((double)hue / 60.0, (double)2.0) - 1.0);
    // ::std::cout << "X_aux= " << x_aux << ::std::endl;
    int x_255 = c_255 * (10000 - (int)(x_aux * 10000)) / 10000;
    // ::std::cout << "X= " << x_255 << ::std::endl;
    int m_255 = luminance - (c_255 / 2);
    // ::std::cout << "M= " << m_255 << ::std::endl;

    if (hue >= 300)
    {
        red = c_255 + m_255;
        green = m_255;
        blue = x_255 + m_255;
    }
    else if (hue >= 240)
    {
        red = x_255 + m_255;
        green = m_255;
        blue = c_255 + m_255;
    }
    else if (hue >= 180)
    {
        red = 0 + m_255;
        green = x_255 + m_255;
        blue = c_255 + m_255;
    }
    else if (hue >= 120)
    {
        red = m_255;
        green = c_255 + m_255;
        blue = x_255 + m_255;
    }
    else if (hue >= 60)
    {
        red = x_255 + m_255;
        green = c_255 + m_255;
        blue = m_255;
    }
    else
    {
        red = c_255 + m_255;
        green = x_255 + m_255;
        blue = m_255;
    }
}

uint8_t Pixel::byte0(PixelFormat format) const noexcept
{
    switch (format)
    {
    case PixelFormat::BGR:
        [[fallthrough]];
    case PixelFormat::BRG:
        return blue;
    case PixelFormat::GBR:
        [[fallthrough]];
    case PixelFormat::GRB:
        return green;
    case PixelFormat::RBG:
        [[fallthrough]];
    case PixelFormat::RGB:
        return red;
    }
    return 0;
}

uint8_t Pixel::byte1(PixelFormat format) const noexcept
{
    switch (format)
    {
    case PixelFormat::RBG:
        [[fallthrough]];
    case PixelFormat::GBR:
        return blue;
    case PixelFormat::BGR:
        [[fallthrough]];
    case PixelFormat::RGB:
        return green;
    case PixelFormat::BRG:
        [[fallthrough]];
    case PixelFormat::GRB:
        return red;
    }
    return 0;
}

uint8_t Pixel::byte2(PixelFormat format) const noexcept
{
    switch (format)
    {
    case PixelFormat::GRB:
        [[fallthrough]];
    case PixelFormat::RGB:
        return blue;
    case PixelFormat::BRG:
        [[fallthrough]];
    case PixelFormat::RBG:
        return green;
    case PixelFormat::GBR:
        [[fallthrough]];
    case PixelFormat::BGR:
        return red;
    }
    return 0;
}

void Pixel::dim(uint8_t brightness_factor) noexcept
{
    uint16_t b = brightness_factor + 1;
    red = (red * b) >> 8;
    green = (green * b) >> 8;
    blue = (blue * b) >> 8;
}
