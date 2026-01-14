/**
 * @file PixelDriver.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Supported pixel drivers
 *
 * @date 2025-08-29
 *
 * @copyright Under EUPL 1.2 License
 *
 */

#pragma once

#include <cstdint>
#include <chrono>

//------------------------------------------------------------------------------

/**
 * @brief Byte order of pixel data starting with the least significant byte
 *
 */
enum class PixelFormat : unsigned char
{
    ///  @brief Red-green-blue
    RGB = 0,
    ///  @brief Red-blue-green
    RBG,
    ///  @brief Green-red-blue
    GRB,
    ///  @brief Green-blue-red
    GBR,
    ///  @brief Blue-red-green
    BRG,
    ///  @brief Blue-green-red
    BGR
};

/**
 * @brief Working parameters of a pixel driver
 *
 */
struct PixelDriver
{
    /// @brief Pixel format required by this driver
    PixelFormat pixelFormat;
    /// @brief Bit encoding for each pixel driver
    bool bitEncodingHighToLow = true;
    /// @brief Transmission bit order
    bool msbFirst = true;
    /// @brief Duration of the first voltage stage of bit 0
    ::std::chrono::nanoseconds bit0FirstStageTime;
    /// @brief Duration of the second voltage stage of bit 0
    ::std::chrono::nanoseconds bit0SecondStageTime;
    /// @brief Duration of the first voltage stage of bit 1
    ::std::chrono::nanoseconds bit1FirstStageTime;
    /// @brief Duration of the second voltage stage of bit 1
    ::std::chrono::nanoseconds bit1SecondStageTime;
    /// @brief Duration of the rest state required to show pixel data
    ::std::chrono::nanoseconds restTime;
};

/// @brief WS2811 family of pixel drivers
inline constexpr PixelDriver WS2811{
    .pixelFormat = PixelFormat::RGB,
    .bit0FirstStageTime = ::std::chrono::nanoseconds{500},
    .bit0SecondStageTime = ::std::chrono::nanoseconds{2000},
    .bit1FirstStageTime = ::std::chrono::nanoseconds{1200},
    .bit1SecondStageTime = ::std::chrono::nanoseconds{1300},
    .restTime = ::std::chrono::nanoseconds{50000}};

/// @brief WS2812 family of pixel drivers
inline constexpr PixelDriver WS2812{
    .pixelFormat = PixelFormat::GRB,
    .bit0FirstStageTime = ::std::chrono::nanoseconds{300},
    .bit0SecondStageTime = ::std::chrono::nanoseconds{900},
    .bit1FirstStageTime = ::std::chrono::nanoseconds{900},
    .bit1SecondStageTime = ::std::chrono::nanoseconds{300},
    .restTime = ::std::chrono::nanoseconds{280000}};

/// @brief WS2815 pixel driver
inline constexpr PixelDriver WS2815{
    .pixelFormat = PixelFormat::GRB,
    .bit0FirstStageTime = ::std::chrono::nanoseconds{300},
    .bit0SecondStageTime = ::std::chrono::nanoseconds{900},
    .bit1FirstStageTime = ::std::chrono::nanoseconds{900},
    .bit1SecondStageTime = ::std::chrono::nanoseconds{300},
    .restTime = ::std::chrono::nanoseconds{280000}};

/// @brief SK6812 pixel driver
inline constexpr PixelDriver SK6812{
    .pixelFormat = PixelFormat::GRB,
    .bit0FirstStageTime = ::std::chrono::nanoseconds{300},
    .bit0SecondStageTime = ::std::chrono::nanoseconds{900},
    .bit1FirstStageTime = ::std::chrono::nanoseconds{600},
    .bit1SecondStageTime = ::std::chrono::nanoseconds{600},
    .restTime = ::std::chrono::nanoseconds{80000}};

/// @brief UCS1903 pixel driver
inline constexpr PixelDriver UCS1903{
    .pixelFormat = PixelFormat::RGB,
    .bit0FirstStageTime = ::std::chrono::nanoseconds{500},
    .bit0SecondStageTime = ::std::chrono::nanoseconds{800},
    .bit1FirstStageTime = ::std::chrono::nanoseconds{800},
    .bit1SecondStageTime = ::std::chrono::nanoseconds{400},
    .restTime = ::std::chrono::nanoseconds{24000}};
