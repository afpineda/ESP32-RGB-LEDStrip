/**
 * @file PixelVector.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2026-01-16
 *
 * @copyright Under EUPL 1.2 License
 */

//------------------------------------------------------------------------------
// Imports and globals
//------------------------------------------------------------------------------

#include "PixelVector.hpp"
#include <numeric> // std::gcd()

//------------------------------------------------------------------------------
// PixelVector
//------------------------------------------------------------------------------

void PixelVector::shift(
    PixelVector::size_type from_index,
    PixelVector::size_type to_index,
    PixelVector::size_type shift)
{
    if (from_index >= size())
        from_index = size() - 1;
    if (to_index >= size())
        to_index = size() - 1;
    if (from_index > to_index)
    {
        // -- Shift down --
        PixelVector::size_type n = from_index - to_index + 1;
        shift = shift % n;
        if (shift > 0)
        {
            PixelVector::size_type num_cycles = ::std::gcd(n, shift);
            for (
                PixelVector::size_type start = 0;
                start < num_cycles;
                ++start)
            {
                Pixel temp = ::std::move((*this)[start + to_index]);
                PixelVector::size_type current = start;
                while (true)
                {
                    PixelVector::size_type next_index = current + shift;
                    if (next_index >= n)
                        next_index -= n;
                    if (next_index == start)
                        break;
                    (*this)[current + to_index] =
                        ::std::move((*this)[next_index + to_index]);
                    current = next_index;
                }
                (*this)[current + to_index] = ::std::move(temp);
            }
        }
    }
    else if (from_index < to_index)
    {
        // -- Shift up --
        PixelVector::size_type n = to_index - from_index + 1;
        PixelVector::size_type equivalent = (n - shift) % n;
        PixelVector::shift(to_index, from_index, equivalent);
    }
}

void PixelVector::operator<<(PixelVector::size_type count)
{
    if (size() > 1)
        shift(size() - 1, 0, count);
}

void PixelVector::operator>>(PixelVector::size_type count)
{
    if (size() > 1)
        shift(0, size() - 1, count);
}

void PixelVector::fill(const Pixel &color)
{
    for (size_type i = 0; i < this->size(); i++)
        (*this)[i] = color;
}

void PixelVector::fill(
    const Pixel &color,
    PixelVector::size_type fromIndex,
    PixelVector::size_type toIndex)
{
    if (fromIndex <= toIndex)
        for (size_type i = fromIndex;
             (i < this->size()) && (i <= toIndex);
             i++)
        {
            (*this)[i] = color;
        }
    else
        fill(color, toIndex, fromIndex);
}
