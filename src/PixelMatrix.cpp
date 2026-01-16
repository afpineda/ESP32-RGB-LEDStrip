/**
 * @file PixelMatrix.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2026-01-15
 *
 * @copyright Under EUPL 1.2 License
 */

//------------------------------------------------------------------------------
// Imports and globals
//------------------------------------------------------------------------------

#include "PixelMatrix.hpp"
#include <numeric> // std::gcd()
#include <cassert>

#define Idx(r, c) (((r) * columns) + (c))

//------------------------------------------------------------------------------
// PixelMatrix
//------------------------------------------------------------------------------

PixelMatrix::PixelMatrix() noexcept
{
    resize(0);
    rows = 0;
    columns = 0;
}

PixelMatrix::PixelMatrix(
    PixelMatrix::size_type rows,
    PixelMatrix::size_type columns,
    Pixel color) noexcept : rows{rows}, columns{columns}
{
    resize(rows * columns, color);
}

PixelMatrix::PixelMatrix(
    const PixelMatrix::initializer_list_type &init_list) noexcept
{
    *this = init_list;
}

PixelMatrix &PixelMatrix::operator=(
    const PixelMatrix::initializer_list_type &init_list) noexcept
{
    // Determine column size
    columns = 0;
    for (auto column_list : init_list)
        if (column_list.size() > columns)
            columns = column_list.size();

    // Resize
    rows = init_list.size();
    resize(rows * columns);

    // Copy pixels
    PixelMatrix::size_type row = 0;
    for (auto column_list : init_list)
    {
        PixelMatrix::size_type col = 0;
        for (auto pixel : column_list)
        {
            (*this)[Idx(row, col)] = pixel;
            col++;
        }
        row++;
    }
    return *this;
}

Pixel &PixelMatrix::at(
    PixelMatrix::size_type row,
    PixelMatrix::size_type col)
{
    return PixelVector::at(Idx(row, col));
}

const Pixel &PixelMatrix::at(
    PixelMatrix::size_type row,
    PixelMatrix::size_type col) const
{
    return PixelVector::at(Idx(row, col));
}

void PixelMatrix::fill(const Pixel &color) noexcept
{
    for (PixelMatrix::size_type i = 0; i < size(); i++)
        (*this)[i] = color;
}

void PixelMatrix::operator<<(PixelMatrix::size_type count) noexcept
{
    if ((rows > 0) && (columns > 1))
    {
        count = count % columns; // optimization
        for (PixelMatrix::size_type row = 0; row < rows; row++)
            shift(Idx(row, columns - 1), Idx(row, 0), count);
    }
}

void PixelMatrix::operator>>(PixelMatrix::size_type count) noexcept
{
    if ((rows > 0) && (columns > 1))
    {
        count = count % columns; // optimization
        for (PixelMatrix::size_type row = 0; row < rows; row++)
            shift(Idx(row, 0), Idx(row, columns - 1), count);
    }
}

void PixelMatrix::scroll_up(PixelMatrix::size_type count) noexcept
{
    if ((rows > 1) && (columns > 0))
    {
        shift(size() - 1, 0, (count * columns));
    }
}

void PixelMatrix::scroll_down(PixelMatrix::size_type count) noexcept
{
    if ((rows > 1) && (columns > 0))
    {
        shift(0, size() - 1, (count * columns));
    }
}

//------------------------------------------------------------------------------
// LedMatrixDefinition
//------------------------------------------------------------------------------

void LedMatrixDefinition::indexToCoordinates(
    ::std::size_t index,
    ::std::size_t &row,
    ::std::size_t &col) const noexcept
{
    assert(index < (row_count * column_count));
    if (arrangement == LedMatrixArrangement::rows)
    {
        row = (index / column_count);
        col = (index % column_count);
        bool reverse =
            (row % 2) && (wiring == LedMatrixWiring::serpentine);

        if (((first_pixel == LedMatrixFirstPixel::top_right) ||
             (first_pixel == LedMatrixFirstPixel::bottom_right)) ^
            reverse)
            col = (column_count - 1) - col;
        if ((first_pixel == LedMatrixFirstPixel::bottom_left) ||
            (first_pixel == LedMatrixFirstPixel::bottom_right))
            row = (row_count - 1) - row;
    }
    else // (arrangement == LedMatrixArrangement::columns)
    {
        col = (index / row_count);
        row = (index % row_count);
        bool reverse =
            (col % 2) && (wiring == LedMatrixWiring::serpentine);

        if (((first_pixel == LedMatrixFirstPixel::bottom_left) ||
             (first_pixel == LedMatrixFirstPixel::bottom_right)) ^
            reverse)
            row = (row_count - 1) - row;
        if ((first_pixel == LedMatrixFirstPixel::top_right) ||
            (first_pixel == LedMatrixFirstPixel::bottom_right))
            col = (column_count - 1) - col;
    }
}

#define INV(size, value) (((size) - 1) - (value))

::std::size_t LedMatrixDefinition::coordinatesToIndex(
    ::std::size_t row,
    ::std::size_t col) const noexcept
{
    assert(row < row_count);
    assert(col < column_count);

    switch (first_pixel)
    {
    case LedMatrixFirstPixel::top_left:
    {
        if (arrangement == LedMatrixArrangement::rows)
        {
            if ((row % 2) && (wiring == LedMatrixWiring::serpentine))
                return (row * column_count) + INV(column_count, col);
            else
                return (row * column_count) + col;
        }
        else
        {
            if ((col % 2) && (wiring == LedMatrixWiring::serpentine))
                return (col * row_count) + INV(row_count, row);
            else
                return (col * row_count) + row;
        }
    }
    break;
    case LedMatrixFirstPixel::top_right:
    {
        if (arrangement == LedMatrixArrangement::rows)
        {
            if ((row % 2) && (wiring == LedMatrixWiring::serpentine))
                return (row * column_count) + col;
            else
                return (row * column_count) + INV(column_count, col);
        }
        else
        {
            if ((col % 2) && (wiring == LedMatrixWiring::serpentine))
                return (INV(column_count, col) * row_count) +
                       INV(row_count, row);
            else
                return (INV(column_count, col) * row_count) + row;
        }
    }
    break;
    case LedMatrixFirstPixel::bottom_left:
    {
        if (arrangement == LedMatrixArrangement::rows)
        {
            if ((row % 2) && (wiring == LedMatrixWiring::serpentine))
                return (INV(row_count, row) * column_count) +
                       INV(column_count, col);
            else
                return (INV(row_count, row) * column_count) + col;
        }
        else
        {
            if ((col % 2) && (wiring == LedMatrixWiring::serpentine))
                return (col * row_count) + row;
            else
                return (col * row_count) + INV(row_count, row);
        }
    }
    break;
    case LedMatrixFirstPixel::bottom_right:
    {
        if (arrangement == LedMatrixArrangement::rows)
        {
            if ((row % 2) && (wiring == LedMatrixWiring::serpentine))
                return (INV(row_count, row) * column_count) + col;
            else
                return (INV(row_count, row) * column_count) +
                       INV(column_count, col);
        }
        else
        {
            if ((col % 2) && (wiring == LedMatrixWiring::serpentine))
                return (INV(column_count, col) * row_count) + row;
            else
                return (INV(column_count, col) * row_count) +
                       INV(row_count, row);
        }
    }
    break;
    } // switch
    assert(false && "switch statement failure");
}

::std::size_t LedMatrixDefinition::canonicalIndex(
    ::std::size_t index) const noexcept
{
    ::std::size_t row, col;
    indexToCoordinates(index, row, col);
    return (row * column_count) + col;
}