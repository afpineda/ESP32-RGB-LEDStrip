/**
 * @file PriorityDisplayTest.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Test prioritized display
 *
 * @date 2025-10-20
 *
 * @copyright Under EUPL 1.2 license
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "LEDStrip.hpp"
#include <iostream>
#include <cassert>

using namespace std;

//-------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------

DummyLEDStrip strip;
uint8_t shownPixels = 0xFF;
PixelVector pixels1{0x000000};
PixelVector pixels2{0xFFFFFF};
#define RESULT_PIXELS1 0
#define RESULT_PIXELS2 1

//-------------------------------------------------------------------
// Auxiliary
//-------------------------------------------------------------------

void onShow(const PixelVector &pixels)
{
    shownPixels = 0;
    for (int i = 0; (i < 8) && (i < pixels.size()); i++)
        if (static_cast<uint32_t>(pixels.at(i)))
            shownPixels |= (1 << i);
}

//-------------------------------------------------------------------
// Test cases
//-------------------------------------------------------------------

void test1()
{
    cout << "-Test 1-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    g1.show(pixels1);
    assert(shownPixels == RESULT_PIXELS1);
}

void test2()
{
    cout << "-Test 2-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    RgbGuard g2(strip, 1);
    g2.show(pixels2);
    g1.show(pixels1);
    assert(shownPixels == RESULT_PIXELS2);
}

void test3()
{
    cout << "-Test 3-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    RgbGuard g2(strip, 1);
    g1.show(pixels1);
    g2.show(pixels2);
    assert(shownPixels == RESULT_PIXELS2);
}

void test4()
{
    cout << "-Test 4-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    {
        RgbGuard g2(strip, 1);
        g2.show(pixels2);
        assert(shownPixels == RESULT_PIXELS2);
    }
    g1.show(pixels1);
    assert(shownPixels == RESULT_PIXELS1);
}

void test5()
{
    cout << "-Test 5-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 2);
    g1.show(pixels1);
    {
        RgbGuard g2(strip, 1);
        g2.show(pixels2);
        assert(shownPixels == RESULT_PIXELS1);
    }
    g1.show(pixels2);
    assert(shownPixels == RESULT_PIXELS2);
}

void test6()
{
    cout << "-Test 6-" << endl;
    RgbGuard g1(strip, 0);
    RgbGuard g2(strip, 1);
    g1.show(pixels1);
    g2.show(pixels2);
    assert(shownPixels == RESULT_PIXELS2);
    shownPixels = 0xFF;
    g1.reacquire(3);
    g1.show(pixels1);
    g2.show(pixels2);
    assert(shownPixels == RESULT_PIXELS1);
}

void test7()
{
    cout << "-Test 7-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    RgbGuard g2(strip, 0);
    g1.show(pixels1);
    g2.show(pixels2);
    assert(shownPixels == RESULT_PIXELS1);
}

void test8()
{
    cout << "-Test 8-" << endl;
    shownPixels = 0xFF;
    RgbGuard g1(strip, 0);
    g1.show(pixels1);
    {
        RgbGuard g2(strip, 0);
        g2.show(pixels2);
        assert(shownPixels == RESULT_PIXELS1);
    }
    g1.show(pixels2);
    assert(shownPixels == RESULT_PIXELS2);
}

void test9()
{
    cout << "-Test 9-" << endl;
    shownPixels = 0xFF;
    {
        RgbGuard g1(strip, 0);
        {
            RgbGuard g2(strip, 1);
            {
                RgbGuard g3(strip, 2);
                g3.show(pixels2);
                g2.show(pixels1);
                g1.show(pixels1);
                assert(shownPixels == RESULT_PIXELS2);
            }
            shownPixels = 0xFF;
            g2.show(pixels1);
            g1.show(pixels2);
            assert(shownPixels == RESULT_PIXELS1);
        }
        shownPixels = 0xFF;
        g1.show(pixels2);
        assert(shownPixels == RESULT_PIXELS2);
    }
}

//-------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------

int main()
{
    // Initialization
    strip.onShow = onShow;

    // Tests
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    return 0;
}