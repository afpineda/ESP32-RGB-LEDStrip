/**
 * @file ConstructorTest.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2026-01-14
 *
 * @copyright Under EUPL 1.2 License
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "LEDStrip.hpp"
#include <iostream>
#include <cassert>
#include <utility>

using namespace std;

//-------------------------------------------------------------------
// Auxiliary
//-------------------------------------------------------------------

void onShow(const PixelVector &pixels)
{
    // Empty on purpose
}

//-------------------------------------------------------------------
// Test cases
//-------------------------------------------------------------------

void test1()
{
    cout << "-Pixels: create/copy/move constructors-" << endl;

    Pixel source(0xFFFFFF);
    Pixel dest1;
    assert(dest1 == 0);
    assert(source == 0xFFFFFF);

    Pixel dest2(source);
    assert(dest2 == 0xFFFFFF);

    Pixel dest3;
    dest3 = source;
    assert(dest3 == 0xFFFFFF);

    assert(Pixel(source) == 0xFFFFFF);
}

void test2()
{
    cout << "-DummyLEDStrip: create/move constructors-" << endl;
    {
        DummyLEDStrip source;
        source.onShow = onShow;
        assert(source.onShow);
        DummyLEDStrip dest(::std::move(source));
        assert(dest.onShow);
    }
    {
        DummyLEDStrip source;
        source.onShow = onShow;
        assert(source.onShow);
        DummyLEDStrip dest = ::std::move(source);
        assert(dest.onShow);
    }
}

//-------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------

int main()
{
    // Tests
    test1();
    test2();
    return 0;
}