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
