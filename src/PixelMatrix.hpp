/**
 * @file PixelMatrix.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2026-01-15
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include <initializer_list>
#include "PixelVector.hpp"

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
    /// @brief Alias for "serpentine"
    zig_zag = serpentine,
    /// @brief Alias for "linear"
    progressive = linear,
};

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

/**
 * @brief Working parameters of an LED matrix
 *
 */
struct LedMatrixDefinition
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
};

/**
 * @brief Matrix of pixels
 *
 */
struct PixelMatrix : public PixelVector
{
    /// @brief Canonical first pixel in any pixel matrix
    /// @note Informational. No use.
    inline static constexpr const LedMatrixFirstPixel first_pixel =
        LedMatrixFirstPixel::top_left;

    /// @brief Canonical arrangement of rows and columns in any pixel matrix
    /// @note Informational. No use.
    inline static constexpr const LedMatrixArrangement arrangement =
        LedMatrixArrangement::rows;

    /// @brief Canonical equivalent wiring schema in any pixel matrix
    /// @note Informational. No use.
    inline static constexpr const LedMatrixWiring wiring =
        LedMatrixWiring::linear;

    /// @brief Size type
    using size_type = typename PixelVector::size_type;

    /// @brief Initializer list type
    using initializer_list_type =
        typename ::std::initializer_list<::std::initializer_list<Pixel>>;

    /**
     * @brief Create an empty matrix of pixels
     *
     */
    PixelMatrix() noexcept;

    /**
     * @brief Create a matrix of pixels
     *
     * @param rows Number of rows
     * @param columns Number of columns
     * @param color Initial color for all cells
     */
    PixelMatrix(size_type rows, size_type columns, Pixel color = 0) noexcept;

    /**
     * @brief Create matrix of pixels suitable for an LED matrix
     *
     * @param def Working parameters of the LED matrix
     * @param color Initial color for all cells
     */
    PixelMatrix(
        const LedMatrixDefinition &def,
        Pixel color = 0) noexcept
        : PixelMatrix(def.row_count, def.column_count, color) {}

    /**
     * @brief Initialize a matrix of pixels
     *
     * @note Missing pixels in @p init_list will be initialized to black.
     *
     * @param init_list List of rows in ascending order.
     *                  Each row is a list of pixels in ascending order.
     */
    PixelMatrix(const initializer_list_type &init_list) noexcept;

    /**
     * @brief Access to a pixel
     *
     * @note There are bound checks
     *
     * @param row Row index
     * @param col Column index
     * @return Pixel& Pixel
     */
    Pixel &at(size_type row, size_type col);

    /**
     * @brief Access to a pixel
     *
     * @note There are bound checks
     *
     * @param row Row index
     * @param col Column index
     * @return Pixel& Pixel
     */
    const Pixel &at(size_type row, size_type col) const;

    PixelMatrix(const PixelMatrix &) noexcept = default;
    PixelMatrix(PixelMatrix &&) noexcept = default;
    PixelMatrix &operator=(const PixelMatrix &) noexcept = default;
    PixelMatrix &operator=(PixelMatrix &&) noexcept = default;

    /**
     * @brief Assign an initializer list
     *
     * @note Missing pixels in @p init_list will be initialized to black.
     *
     * @param init_list List of rows in ascending order.
     *                  Each row is a list of pixels in ascending order.
     */
    PixelMatrix &operator=(const initializer_list_type &init_list) noexcept;

    /**
     * @brief Get the number of rows in this matrix
     *
     * @return size_type Number of rows
     */
    size_type row_count() const noexcept { return rows; }

    /**
     * @brief Get the number of columns in this matrix
     *
     * @return size_type Number of columns
     */
    size_type column_count() const noexcept { return columns; }

    /**
     * @brief Fill the entire matrix with a pixel color
     *
     * @param color Pixel color
     */
    void fill(const Pixel &color) noexcept;

    /**
     * @brief Scroll left
     *
     * @param count Shift count
     */
    void operator<<(size_type count) noexcept;

    /**
     * @brief Scroll right
     *
     * @param count Shift count
     */
    void operator>>(size_type count) noexcept;

    /**
     * @brief Scroll up
     *
     * @param count Shift count
     */
    void scroll_up(size_type count) noexcept;

    /**
     * @brief Scroll down
     *
     * @param count Shift count
     */
    void scroll_down(size_type count) noexcept;

    /**
     * @brief Check if this matrix of pixels is suitable for display
     *        in a given LED matrix
     *
     * @param def Working parameters of the LED matrix
     * @return true If suitable
     * @return false If not
     */
    bool suitable_for(const LedMatrixDefinition &def)
    {
        return (rows == def.row_count) && (columns = def.column_count);
    }

private:
    size_type rows;
    size_type columns;
};
