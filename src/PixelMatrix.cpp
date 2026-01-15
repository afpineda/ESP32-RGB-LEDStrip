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

//------------------------------------------------------------------------------
// PixelMatrix
//------------------------------------------------------------------------------

void PixelMatrix::init(
    PixelMatrix::size_type rows,
    PixelMatrix::size_type columns,
    Pixel color) noexcept
{
    resize(rows);
    for (PixelMatrix::size_type i = 0; i < rows; i++)
        (*this)[i].resize(columns, color);
}

PixelMatrix::PixelMatrix() noexcept
{
    init(0, 0, 0);
}

PixelMatrix::PixelMatrix(
    PixelMatrix::size_type rows,
    PixelMatrix::size_type columns,
    Pixel color) noexcept
{
    init(rows, columns, color);
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
    PixelMatrix::size_type columns = 0;
    for (auto column_list : init_list)
        if (column_list.size() > columns)
            columns = column_list.size();

    // Resize
    init(init_list.size(), columns, 0);

    // Copy pixels
    PixelMatrix::size_type row = 0;
    for (auto column_list : init_list)
    {
        PixelMatrix::size_type col = 0;
        for (auto pixel : column_list)
            (*this)[row][col++] = pixel;
        row++;
    }
    return *this;
}

Pixel &PixelMatrix::at(
    PixelMatrix::size_type row,
    PixelMatrix::size_type col)
{
    return ::std::vector<PixelVector>::at(row).at(col);
}

PixelMatrix::size_type PixelMatrix::row_count() const noexcept
{
    return size();
}

PixelMatrix::size_type PixelMatrix::column_count() const noexcept
{
    if (size())
        return (*this)[0].size();
    else
        return 0;
}

void PixelMatrix::fill(const Pixel &color) noexcept
{
    PixelMatrix::size_type rows = row_count();
    PixelMatrix::size_type columns = column_count();
    for (PixelMatrix::size_type row = 0; row < rows; row++)
        for (PixelMatrix::size_type col = 0; col < columns; col++)
            (*this)[row][col] = color;
}

void PixelMatrix::operator<<(PixelMatrix::size_type count) noexcept
{
    count = count % column_count(); // Optimization
    PixelMatrix::size_type rows = row_count();
    for (PixelMatrix::size_type row = 0; row < rows; row++)
        (*this)[row] << count;
}

void PixelMatrix::operator>>(PixelMatrix::size_type count) noexcept
{
    count = count % column_count(); // Optimization
    PixelMatrix::size_type rows = row_count();
    for (PixelMatrix::size_type row = 0; row < rows; row++)
        (*this)[row] >> count;
}

void PixelMatrix::scroll_up(PixelMatrix::size_type count) noexcept
{
    PixelMatrix::size_type n = row_count();
    count = count % n;
    if ((n > 0) && (count > 0))
    {
        std::size_t num_cycles = ::std::gcd(n, count);
        for (PixelMatrix::size_type start = 0; start < num_cycles; ++start)
        {
            PixelVector temp = ::std::move((*this)[start]);
            PixelMatrix::size_type current = start;
            while (true)
            {
                PixelMatrix::size_type next_index = current + count;
                if (next_index >= n)
                    next_index -= n;
                if (next_index == start)
                    break;
                (*this)[current] = ::std::move((*this)[next_index]);
                current = next_index;
            }
            (*this)[current] = ::std::move(temp);
        }
    }
}

void PixelMatrix::scroll_down(PixelMatrix::size_type count) noexcept
{
    PixelMatrix::size_type n = row_count();
    count = count % n;
    if ((n > 0) && (count > 0))
    {
        PixelMatrix::size_type up_equivalent = (n - count) % n;
        scroll_up(up_equivalent);
    }
}