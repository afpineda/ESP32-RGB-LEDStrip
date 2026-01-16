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
        while (count-- > 0)
            for (PixelMatrix::size_type row = 0; row < rows; row++)
                shift(Idx(row, columns - 1), Idx(row, 0));
    }
}

void PixelMatrix::operator>>(PixelMatrix::size_type count) noexcept
{
    if ((rows > 0) && (columns > 1))
    {
        count = count % columns; // optimization
        while (count-- > 0)
            for (PixelMatrix::size_type row = 0; row < rows; row++)
                shift(Idx(row, 0), Idx(row, columns - 1));
    }
}

void PixelMatrix::scroll_up(PixelMatrix::size_type count) noexcept
{
    // PixelMatrix::size_type n = row_count();
    // count = count % n;
    // if ((n > 0) && (count > 0))
    // {
    //     std::size_t num_cycles = ::std::gcd(n, count);
    //     for (PixelMatrix::size_type start = 0; start < num_cycles; ++start)
    //     {
    //         PixelVector temp = ::std::move((*this)[start]);
    //         PixelMatrix::size_type current = start;
    //         while (true)
    //         {
    //             PixelMatrix::size_type next_index = current + count;
    //             if (next_index >= n)
    //                 next_index -= n;
    //             if (next_index == start)
    //                 break;
    //             (*this)[current] = ::std::move((*this)[next_index]);
    //             current = next_index;
    //         }
    //         (*this)[current] = ::std::move(temp);
    //     }
    // }
}

void PixelMatrix::scroll_down(PixelMatrix::size_type count) noexcept
{
    // PixelMatrix::size_type n = row_count();
    // count = count % n;
    // if ((n > 0) && (count > 0))
    // {
    //     PixelMatrix::size_type up_equivalent = (n - count) % n;
    //     scroll_up(up_equivalent);
    // }
}