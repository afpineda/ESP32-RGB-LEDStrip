/**
 * @file PixelMatrixTest.cpp
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Test arrays of pixels
 *
 * @date 2026-01-15
 *
 * @copyright Under EUPL 1.2 license
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "PixelMatrix.hpp"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

//-------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Auxiliary
//-------------------------------------------------------------------

void assert_filled(
    const PixelMatrix &mtx,
    Pixel color,
    const ::std::string &msg)
{
    for (auto pixel : mtx)
        if ((pixel != color))
        {
            cout << msg << " (pixel mismatch)" << endl;
            assert(false);
        }
}

void print(const PixelMatrix &mtx)
{
    PixelMatrix::size_type col = 1;
    for (auto pixel : mtx)
    {
        cout << (unsigned int)pixel << " ";
        if (col >= mtx.column_count())
        {
            col = 1;
            cout << endl;
        }
        else
            col++;
    }
    cout << endl;
}

//-------------------------------------------------------------------
// Test groups
//-------------------------------------------------------------------

void test1()
{
    cout << "- Matrix initialization -" << endl;
    {
        PixelMatrix mtx;
        assert(mtx.size() == 0);
        assert(mtx.row_count() == 0);
        assert(mtx.column_count() == 0);
    }
    {
        PixelMatrix mtx(3, 2, 0xFFFFFF);
        assert(mtx.size() == 6);
        assert(mtx.row_count() == 3);
        assert(mtx.column_count() == 2);
        assert_filled(mtx, 0xFFFFFF, "mtx(3, 2, 0xFFFFFF)");
    }
    {
        PixelMatrix mtx({
            {3, 3},
            {3, 3},
        });
        assert(mtx.size() == 4);
        assert(mtx.row_count() == 2);
        assert(mtx.column_count() == 2);
        assert_filled(mtx, 3, "mtx(1st initializer list)");
    }
    {
        PixelMatrix mtx({
            {1},
            {2, 3},
        });
        assert(mtx.size() == 4);
        assert(mtx.row_count() == 2);
        assert(mtx.column_count() == 2);
        assert(mtx.at(0, 0) == 1);
        assert(mtx.at(0, 1) == 0);
        assert(mtx.at(1, 0) == 2);
        assert(mtx.at(1, 1) == 3);
    }
}

void test2()
{
    cout << "- Copy -" << endl;
    PixelMatrix source(2, 3, 0xFFFFFF);
    {
        PixelMatrix dest(source);
        assert(dest.row_count() == 2);
        assert(dest.column_count() == 3);
        assert_filled(dest, 0xFFFFFF, "copy-constructor");
    }
    {
        PixelMatrix dest;
        dest = source;
        assert(dest.row_count() == 2);
        assert(dest.column_count() == 3);
        assert_filled(dest, 0xFFFFFF, "copy-assign");
    }
}

void test3()
{
    cout << "- Fill -" << endl;
    {
        PixelMatrix mtx(2, 2);
        assert_filled(mtx, 0, "before fill()");
        mtx.fill(1);
        assert_filled(mtx, 1, "after fill()");
    }
}

void test4()
{
    cout << "- Scroll left/right -" << endl;
    PixelMatrix test({
        {1, 2, 3},
        {3, 4, 5},
    });
    {
        PixelMatrix mtx(test);
        mtx >> 3;
        assert(mtx == test);
    }
    {
        PixelMatrix mtx(test);
        mtx >> 2;
        PixelMatrix result({
            {2, 3, 1},
            {4, 5, 3},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx >> 1;
        PixelMatrix result({
            {3, 1, 2},
            {5, 3, 4},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx << 3;
        assert(mtx == test);
    }
    {
        PixelMatrix mtx(test);
        mtx << 2;
        PixelMatrix result({
            {3, 1, 2},
            {5, 3, 4},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx << 1;
        PixelMatrix result({
            {2, 3, 1},
            {4, 5, 3},
        });
        assert(mtx == result);
    }
}

void test5()
{
    cout << "- Scroll up/down -" << endl;
    PixelMatrix test({
        {1, 2},
        {3, 4},
        {5, 6},
    });
    // Scroll up
    {
        PixelMatrix mtx(test);
        mtx.scroll_up(1);
        PixelMatrix result({
            {3, 4},
            {5, 6},
            {1, 2},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx.scroll_up(2);
        PixelMatrix result({
            {5, 6},
            {1, 2},
            {3, 4},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx.scroll_up(3);
        assert(mtx == test);
    }
    // Scroll down
    {
        PixelMatrix mtx(test);
        mtx.scroll_down(1);
        PixelMatrix result({
            {5, 6},
            {1, 2},
            {3, 4},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx.scroll_down(2);
        PixelMatrix result({
            {3, 4},
            {5, 6},
            {1, 2},
        });
        assert(mtx == result);
    }
    {
        PixelMatrix mtx(test);
        mtx.scroll_down(3);
        assert(mtx == test);
    }
}

//-------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}
