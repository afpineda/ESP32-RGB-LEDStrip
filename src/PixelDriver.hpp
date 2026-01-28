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

//------------------------------------------------------------------------------

/**
 * @brief Wiring schema of rows or columns in an LED matrix
 *
 */
enum class LedMatrixWiring : unsigned char
{
    /**
     * @brief The rightmost/bottom pixel in an odd row/column is connected to
     *        the rightmost/bottom pixel in the next row/column.
     *        The leftmost/top pixel in an even row/column is connected to
     *        the leftmost/top pixel in the next row/column.
     */
    serpentine = 0,
    ///  @brief The rightmost/bottom pixel in a row/column is connected
    ///         to the leftmost/top pixel in the next row/column.
    linear = 1,
    /// @brief Alias for "serpentine" in Adafruit's terminology
    zig_zag = serpentine,
    /// @brief Alias for "linear" in Adafruit's terminology
    progressive = linear,
};

//------------------------------------------------------------------------------

/**
 * @brief First pixel attached to the data wire in an LED matrix
 *
 */
enum class LedMatrixFirstPixel : unsigned char
{
    /// @brief First pixel is the top left one
    top_left,
    /// @brief First pixel is the top right one
    top_right,
    /// @brief First pixel is the bottom left one
    bottom_left,
    /// @brief First pixel is the bottom right one
    bottom_right
};

//------------------------------------------------------------------------------

/**
 * @brief Arrangement of rows and columns in an LED matrix
 *
 */
enum class LedMatrixArrangement : unsigned char
{
    /// @brief The first pixel is wired to a row
    rows,
    /// @brief The first pixel is wired to a column
    columns
};

//------------------------------------------------------------------------------

/**
 * @brief Working parameters of an LED matrix
 *
 */
struct LedMatrixParameters
{
    /// @brief Number of rows
    ::std::size_t row_count = 0;
    /// @brief Number of columns
    ::std::size_t column_count = 0;
    /// @brief First pixel in the pixel chain
    LedMatrixFirstPixel first_pixel = LedMatrixFirstPixel::top_left;
    /// @brief Arrangement of rows and columns
    LedMatrixArrangement arrangement = LedMatrixArrangement::rows;
    /// @brief Wiring schema
    LedMatrixWiring wiring = LedMatrixWiring::serpentine;

    /**
     * @brief Retrieve the coordinates of a pixel index according to
     *        this working parameters
     *
     * @warning Bounds are checked with assertions, only.
     *
     * @param[in] index Pixel index
     * @param[out] row Pixel row
     * @param[out] col Pixel column
     */
    void indexToCoordinates(
        ::std::size_t index,
        ::std::size_t &row,
        ::std::size_t &col) const noexcept;

    /**
     * @brief Retrieve the index of the given pixel coordinates
     *        according to this working parameters
     *
     * @warning Bounds are checked with assertions, only.
     *
     * @param row Pixel row
     * @param col Pixel column
     * @return ::std::size_t Pixel index
     */
    ::std::size_t coordinatesToIndex(
        ::std::size_t row,
        ::std::size_t col) const noexcept;

    /**
     * @brief Retrieve the canonical index of the given pixel index
     *
     * @note The canonical index is the corresponding pixel index
     *       in the PixelMatrix implementation
     *
     * @param index Pixel index
     * @return ::std::size_t Canonical pixel index
     */
    ::std::size_t canonicalIndex(::std::size_t index) const noexcept;

    /**
     * @brief Compare to other working parameters
     *
     * @param other Working parameters to compare to
     * @return true If equal
     * @return false If not equal
     */
    bool operator==(const LedMatrixParameters &other) const noexcept
    {
        return (other.row_count == row_count) &&
               (other.column_count == column_count) &&
               (other.first_pixel == first_pixel) &&
               (other.arrangement == arrangement) &&
               (other.wiring == wiring);
    }

    /// @brief Reverse display along the horizontal axis
    void flipVertical() noexcept;

    /// @brief Reverse display along the vertical axis
    void flipHorizontal() noexcept;

    /// @brief Rotate display 90 degrees clockwise
    /// @note Make several calls to rotate 180 or 270 degrees.
    void rotate90clockwise() noexcept;

    /// @brief Size in pixels
    ::std::size_t size() const noexcept { return (row_count * column_count); }
};

/// @brief Basic parameters for an LED strip (1D LED Matrix)
static constexpr const LedMatrixParameters basicLedStriParameters{
    .row_count = 1,
    .column_count = 0,
    .first_pixel = LedMatrixFirstPixel::top_left,
    .arrangement = LedMatrixArrangement::rows,
    .wiring = LedMatrixWiring::linear,
};

/// @brief Basic parameters for an LED strip (1D LED Matrix) in reverse order
static constexpr const LedMatrixParameters basicReversedLedStriParameters{
    .row_count = 1,
    .column_count = 0,
    .first_pixel = LedMatrixFirstPixel::bottom_right,
    .arrangement = LedMatrixArrangement::rows,
    .wiring = LedMatrixWiring::linear,
};