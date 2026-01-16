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