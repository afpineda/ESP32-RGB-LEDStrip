/**
 * @file PixelTest.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Test pixels
 *
 * @date 2025-08-29
 *
 * @copyright Under EUPL 1.2 license
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "Pixel.hpp"
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
#define AS_BGR_FORMAT 0x010203
#define AS_BRG_FORMAT 0x020103
#define AS_RBG_FORMAT 0x020301
#define AS_RGB_FORMAT 0x030201
#define AS_GBR_FORMAT 0x010302
#define AS_GRB_FORMAT 0x030102

//-------------------------------------------------------------------
// Auxiliary
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Test groups
//-------------------------------------------------------------------

void test1()
{
    cout << "- Assignment -" << endl;
    Pixel p1;
    Pixel p2(RGB_TEST_COLOR);
    Pixel p3 = RGB_TEST_COLOR;

    assert(p1 == 0);
    assert(p2 == RGB_TEST_COLOR);
    assert(p3 == RGB_TEST_COLOR);
    assert(p3.red == TEST_COLOR_R);
    assert(p3.green == TEST_COLOR_G);
    assert(p3.blue == TEST_COLOR_B);
}

void test2()
{
    cout << "- Byte order -" << endl;
    Pixel p{0x010203};
    uint8_t b;

    cout << "  byte 0" << endl;
    b = p.byte0(PixelFormat::BGR);
    assert(b == 0x03);
    b = p.byte0(PixelFormat::BRG);
    assert(b == 0x03);
    b = p.byte0(PixelFormat::GBR);
    assert(b == 0x02);
    b = p.byte0(PixelFormat::GRB);
    assert(b == 0x02);
    b = p.byte0(PixelFormat::RBG);
    assert(b == 0x01);
    b = p.byte0(PixelFormat::RGB);
    assert(b == 0x01);

    cout << "  byte 1" << endl;
    b = p.byte1(PixelFormat::BGR);
    assert(b == 0x02);
    b = p.byte1(PixelFormat::BRG);
    assert(b == 0x01);
    b = p.byte1(PixelFormat::GBR);
    assert(b == 0x03);
    b = p.byte1(PixelFormat::GRB);
    assert(b == 0x01);
    b = p.byte1(PixelFormat::RBG);
    assert(b == 0x03);
    b = p.byte1(PixelFormat::RGB);
    assert(b == 0x02);

    cout << "  byte 2" << endl;
    b = p.byte2(PixelFormat::BGR);
    assert(b == 0x01);
    b = p.byte2(PixelFormat::BRG);
    assert(b == 0x02);
    b = p.byte2(PixelFormat::GBR);
    assert(b == 0x01);
    b = p.byte2(PixelFormat::GRB);
    assert(b == 0x03);
    b = p.byte2(PixelFormat::RBG);
    assert(b == 0x02);
    b = p.byte2(PixelFormat::RGB);
    assert(b == 0x03);
}

void test3()
{
    cout << "- Brightness -" << endl;
    Pixel p1(0xFFFFFF);
    Pixel p2(0xFFFFFF);
    Pixel p3(0xFFFFFF);

    p1.dim(255);
    assert(p1 == 0xFFFFFF);
    p2.dim(0);
    assert(p2 == 0x000000);
    p3.dim(127);
    assert(p3 == 0x7F7F7F);
}

void test4()
{
    cout << "- Basic color information -" << endl;
    Pixel p;
    p.red = 200;
    p.green = 50;
    p.blue = 10;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);

    p.red = 200;
    p.green = 10;
    p.blue = 50;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);

    p.red = 10;
    p.green = 200;
    p.blue = 50;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);

    p.red = 50;
    p.green = 200;
    p.blue = 10;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);

    p.red = 50;
    p.green = 10;
    p.blue = 200;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);

    p.red = 10;
    p.green = 50;
    p.blue = 200;
    assert(p.max() == 200);
    assert(p.min() == 10);
    assert(p.avg() == 86);
}

void test5()
{
    // NOTE: the following test cases meet this criteria:
    // - chroma=0
    // - chroma<>0
    // - max=red
    // - max=green
    // - max=blue
    // Expected values computed at https://www.rapidtables.com/convert/color/rgb-to-hsl.html

    cout << "- RGB->HSL conversion -" << endl;
    Pixel p;

    p.red = 0;
    p.green = 0;
    p.blue = 124;
    assert(p.hue() == 240);
    assert(p.HSL_saturation() == 255);
    assert(p.luminance() == 62);

    p.red = 0;
    p.green = 30;
    p.blue = 0;
    assert(p.hue() == 120);
    assert(p.HSL_saturation() == 255);
    assert(p.luminance() == 15);

    p.red = 110;
    p.green = 62;
    p.blue = 33;
    assert(p.hue() == 22);
    assert(p.HSL_saturation() == 137);
    assert(p.luminance() == 71);

    p.red = 20;
    p.green = 190;
    p.blue = 62;
    assert(p.hue() == 134);
    assert(p.HSL_saturation() == 206);
    assert(p.luminance() == 105);

    p.red = 0;
    p.green = 127;
    p.blue = 190;
    assert(p.hue() == 200);
    assert(p.HSL_saturation() == 255);
    assert(p.luminance() == 95);

    p.red = 32;
    p.green = 32;
    p.blue = 32;
    assert(p.hue() == 0);
    assert(p.HSL_saturation() == 0);
    assert(p.luminance() == 32);
}

void test6()
{
    // Expected values taken from https://www.rapidtables.com/convert/color/hsl-to-rgb.html
    // taking rounding into account
    cout << "- HSL->RGB conversion -" << endl;
    Pixel p;
    // cout << "Pixel " << hex << (uint32_t)p << endl;
    p.hsl(0, 0, 0); // black
    assert(p == 0x000000);
    p.hsl(0, 0, 255); // white
    assert(p == 0xFFFFFF);
    p.hsl(0, 255, 127); // red
    assert(p == 0xFF0000);
    p.hsl(120, 255, 127); // lime
    assert(p == 0x00FF00);
    p.hsl(240, 255, 127); // blue
    assert(p == 0x0000FF);
    p.hsl(60, 255, 127); // yellow
    assert(p == 0xFFFF00);
    p.hsl(180, 255, 127); // cyan
    assert(p == 0x00FFFF);
    p.hsl(300, 255, 127); // magenta
    assert(p == 0xFF00FF);
    p.hsl(0, 0, 191); // silver
    assert(p == 0xBFBFBF);
    p.hsl(0, 0, 128); // gray
    assert(p == 0x808080);
    p.hsl(0, 255, 64); // maroon
    assert(p == 0x800000);
    p.hsl(60, 255, 64); // olive
    assert(p == 0x808000);
    p.hsl(120, 255, 64); // green
    assert(p == 0x008000);
    p.hsl(300, 255, 64); // purple
    assert(p == 0x800080);
    p.hsl(180, 255, 64); // teal
    assert(p == 0x008080);
    p.hsl(240, 255, 64); // navy
    assert(p == 0x000080);
    p.hsl(55, 64, 179); // random 1
    assert(p == 0xC6C2A0);
    p.hsl(325, 25, 25); // random 2
    assert(p == 0x1B1719);
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
    test6();
    return 0;
}