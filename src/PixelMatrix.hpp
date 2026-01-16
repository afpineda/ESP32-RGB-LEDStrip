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
 * @brief Matrix of pixels
 *
 */
struct PixelMatrix : public PixelVector
{
    using size_type = typename PixelVector::size_type;
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