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
     * @brief Shift the contents of the vector one pixel
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
     */
    void shift(size_type from_index, size_type to_index)
    {
        {
            if (from_index >= this->size())
                from_index = this->size() - 1;
            if (to_index >= this->size())
                to_index = this->size() - 1;
            Pixel aux = (*this)[to_index];
            if (from_index < to_index)
                // shift up
                for (size_type i = to_index; (i > from_index); i--)
                    (*this)[i] = (*this)[i - 1];
            else if (to_index < from_index)
                // shift down
                for (size_type i = to_index; (i < from_index); i++)
                    (*this)[i] = (*this)[i + 1];
            else
                return;
            (*this)[from_index] = aux;
        }
    }

    /**
     * @brief Shift down (or left) all pixels
     *
     * @param count Shift count
     */
    void operator<<(size_type count)
    {
        while (count-- > 0)
            shift((size_type)-1, 0);
    }

    /**
     * @brief Shift up (or right) all pixels
     *
     * @param count Shift count
     */
    void operator>>(size_type count)
    {
        while (count-- > 0)
            shift(0, (size_type)-1);
    }

    /**
     * @brief Fill the entire vector with a pixel color
     *
     * @param color Pixel color
     */
    void fill(const Pixel &color)
    {
        for (size_type i = 0; i < this->size(); i++)
            (*this)[i] = color;
    }

    /**
     * @brief Fill a segment with a pixel color
     *
     * @param color Pixel color
     * @param fromIndex Segment start index (inclusive)
     * @param toIndex Segment end index (inclusive)
     */
    void fill(const Pixel &color, size_type fromIndex, size_type toIndex)
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

    // Do not hide constructors
    using ::std::vector<Pixel>::vector;

}; // PixelVector
