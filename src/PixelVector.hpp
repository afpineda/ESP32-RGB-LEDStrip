/**
 * @file PixelVector.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-08-27
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include <vector>
#include <initializer_list>
#include "Pixel.hpp"

//------------------------------------------------------------------------------

/**
 * @brief Vector of pixels
 *
 */
struct PixelVector : public ::std::vector<Pixel>
{
public:
    using size_type = typename ::std::vector<Pixel>::size_type;

    /**
     * @brief Shift the contents of the vector
     *        up (right) or down (left)
     *
     * @note if @p from_index is greater than @p to_index,
     *       contents are shifted down, otherwise,
     *       contents are shifted up.
     *
     * @note The pixel that overflows in one end is reintroduced
     *       in the other end
     *
     * @param from_index Index where shifting starts (inclusive)
     * @param to_index Index where shifting ends (inclusive)
     * @param shift Count of pixels to be shifted
     */
    void shift(size_type from_index, size_type to_index, size_type shift = 1);

    /**
     * @brief Shift down (or left) all pixels
     *
     * @param count Shift count
     */
    void operator<<(size_type count);

    /**
     * @brief Shift up (or right) all pixels
     *
     * @param count Shift count
     */
    void operator>>(size_type count);

    /**
     * @brief Fill the entire vector with a pixel color
     *
     * @param color Pixel color
     */
    void fill(const Pixel &color);

    /**
     * @brief Fill a segment with a pixel color
     *
     * @param color Pixel color
     * @param fromIndex Segment start index (inclusive)
     * @param toIndex Segment end index (inclusive)
     */
    void fill(const Pixel &color, size_type fromIndex, size_type toIndex);

    // Do not hide constructors
    using ::std::vector<Pixel>::vector;
}; // PixelVector

//------------------------------------------------------------------------------

/**
 * @brief Matrix of pixels
 *
 */
struct PixelMatrix : public PixelVector
{
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
     * @param rows Number of rows (heigh)
     * @param columns Number of columns (width)
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
        const LedMatrixParameters &def,
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
     * @brief Resize
     *
     * @param rows New number of rows (height)
     * @param columns New number of columns (width)
     * @param color  Initial color for all cells
     */
    void resize(size_type rows, size_type columns, Pixel color = 0) noexcept;

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

private:
    size_type rows;
    size_type columns;
};
