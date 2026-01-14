/**
 * @file PixelVectorTest.cpp
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Test arrays of pixels
 *
 * @date 2025-08-29
 *
 * @copyright Under EUPL 1.2 license
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "PixelVector.hpp"
#include <iostream>
#include <cassert>

using namespace std;

//-------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------

#define TEST_COLOR_R 0x01
#define TEST_COLOR_G 0x02
#define TEST_COLOR_B 0x03

#define RGB_TEST_COLOR 0x010203

//-------------------------------------------------------------------
// Test groups
//-------------------------------------------------------------------

void test1()
{
    cout << "- Array initialization -" << endl;
    PixelVector arr1;
    PixelVector arr2({0, 0, RGB_TEST_COLOR});
    PixelVector arr3(2, Pixel(RGB_TEST_COLOR));
    PixelVector arr4(2);

    assert(arr1.size() == 0);
    assert(arr2.size() == 3);
    assert(arr3.size() == 2);
    assert(arr4.size() == 2);
    assert(arr2[0] == 0);
    assert(arr2[1] == 0);
    assert(arr2[2] == RGB_TEST_COLOR);
    assert(arr3[0] == RGB_TEST_COLOR);
    assert(arr3[1] == RGB_TEST_COLOR);
    assert(arr4[0] == 0);
    assert(arr4[0] == 0);
}

void test2()
{
    cout << "- Shift up -" << endl;
    PixelVector arr({0, 1, 2, 3});
    PixelVector w(arr);
    w.shift(0, (::std::size_t)-1);

    assert(w[0] == 3 && "Part1");
    assert(w[1] == 0 && "Part1");
    assert(w[2] == 1 && "Part1");
    assert(w[3] == 2 && "Part1");

    w = arr;
    w >> 2;

    assert(w[0] == 2 && "Part2");
    assert(w[1] == 3 && "Part2");
    assert(w[2] == 0 && "Part2");
    assert(w[3] == 1 && "Part2");

    w = arr;
    w.shift(1, 2);

    assert(w[0] == 0 && "Part3");
    assert(w[1] == 2 && "Part3");
    assert(w[2] == 1 && "Part3");
    assert(w[3] == 3 && "Part3");
}

void test3()
{
    cout << "- Shift down -" << endl;
    PixelVector arr({0, 1, 2, 3});
    PixelVector w(arr);
    w.shift((::std::size_t)-1, 0);

    assert(w[0] == 1 && "Part1");
    assert(w[1] == 2 && "Part1");
    assert(w[2] == 3 && "Part1");
    assert(w[3] == 0 && "Part1");

    w = arr;
    w << 2;

    assert(w[0] == 2 && "Part2");
    assert(w[1] == 3 && "Part2");
    assert(w[2] == 0 && "Part2");
    assert(w[3] == 1 && "Part2");

    w = arr;
    w.shift(2, 1);

    assert(w[0] == 0 && "Part3");
    assert(w[1] == 2 && "Part3");
    assert(w[2] == 1 && "Part3");
    assert(w[3] == 3 && "Part3");
}

void test4()
{
    cout << "- Fill -" << endl;
    {
        PixelVector v1({0, 1, 2, 3});
        v1.fill(0xABABAB);
        assert(v1[0] == 0xABABAB && "Part1");
        assert(v1[1] == 0xABABAB && "Part1");
        assert(v1[2] == 0xABABAB && "Part1");
        assert(v1[3] == 0xABABAB && "Part1");
    }

    {
        PixelVector v2({0, 1, 2, 3});
        v2.fill(0xABABAB, 1, 2);
        assert(v2[0] == 0 && "Part2");
        assert(v2[1] == 0xABABAB && "Part2");
        assert(v2[2] == 0xABABAB && "Part2");
        assert(v2[3] == 3 && "Part2");
    }

    {
        PixelVector v3({0, 1, 2, 3});
        v3.fill(0xABABAB, 2, 1);
        assert(v3[0] == 0 && "Part3");
        assert(v3[1] == 0xABABAB && "Part3");
        assert(v3[2] == 0xABABAB && "Part3");
        assert(v3[3] == 3 && "Part3");
    }

    {
        PixelVector v4({0, 1, 2, 3});
        v4.fill(0xABABAB, 0, 0);
        assert(v4[0] == 0xABABAB && "Part4");
        assert(v4[1] == 1 && "Part4");
        assert(v4[2] == 2 && "Part4");
        assert(v4[3] == 3 && "Part4");
    }

    {
        PixelVector v5({0, 1, 2, 3});
        v5.fill(0xABABAB, 3, 3);

        assert(v5[0] == 0 && "Part5");
        assert(v5[1] == 1 && "Part5");
        assert(v5[2] == 2 && "Part5");
        assert(v5[3] == 0xABABAB && "Part5");
    }

    {
        PixelVector v6({0, 1, 2, 3});
        v6.fill(0xABABAB, 6, 9);
        assert(v6[0] == 0 && "Part6");
        assert(v6[1] == 1 && "Part6");
        assert(v6[2] == 2 && "Part6");
        assert(v6[3] == 3 && "Part6");
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
    return 0;
}