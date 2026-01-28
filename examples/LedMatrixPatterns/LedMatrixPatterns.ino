/**
 * @file LedMatrixPatterns.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Show several pixel patterns in an LED matrix
 *
 * @date 2026-01-17
 *
 * @copyright Under EUPL 1.2 License
 */

#include "LEDStrip.hpp"
#include "HardwareSerial.h"

//------------------------------------------------------------------
// Configurable globals
//------------------------------------------------------------------

// CONFIGURE TO YOUR NEEDS
// Data pin
#define DATA_PIN 39

// CONFIGURE TO YOUR NEEDS
// Use open drain mode in the data pin
#define OPEN_DRAIN false

// CONFIGURE TO YOUR NEEDS
#define WIDTH 3

// CONFIGURE TO YOUR NEEDS
#define HEIGHT 3

// CONFIGURE TO YOUR NEEDS
static constexpr const LedMatrixParameters params{
    .row_count = HEIGHT,
    .column_count = WIDTH,
    .first_pixel = LedMatrixFirstPixel::top_left,
    .arrangement = LedMatrixArrangement::rows,
    .wiring = LedMatrixWiring::linear,
};

// CONFIGURE TO YOUR NEEDS
#define LED_STRIP_CLASS WS2812LEDStrip

//------------------------------------------------------------------
// Non-configurable globals (do not touch)
//------------------------------------------------------------------

LED_STRIP_CLASS led_matrix(params, DATA_PIN, OPEN_DRAIN, true);
PixelMatrix pixels;

//------------------------------------------------------------------
// Auxiliary
//------------------------------------------------------------------

void chessboard_pattern()
{
    // Create a suitable pixel matrix
    pixels = led_matrix.pixelMatrix();
    // For demonstration purposes (this assertion is not needed):
    assert(pixels.size() == led_matrix.parameters().size());

    for (size_t row = 0; row < pixels.row_count(); row++)
    {
        bool white = (row % 2) ? 0 : 1;
        for (size_t col = 0; col < pixels.column_count(); col++)
        {
            if (white)
                pixels.at(row, col) = 0xFFFFFF;
            else
                pixels.at(row, col) = 0;
            white = !white;
        }
    }
    led_matrix.show(pixels);
}

void horizontal_pattern()
{
    pixels = led_matrix.pixelMatrix();
    for (size_t row = 0; row < pixels.row_count(); row++)
    {
        bool white = (row % 2) ? 0 : 1;
        for (size_t col = 0; col < pixels.column_count(); col++)
        {
            if (white)
                pixels.at(row, col) = 0xFFFFFF;
            else
                pixels.at(row, col) = 0x0000FF;
        }
    }
    led_matrix.show(pixels);
}

void vertical_pattern()
{
    pixels = led_matrix.pixelMatrix();
    for (size_t col = 0; col < pixels.column_count(); col++)
    {
        bool white = (col % 2) ? 0 : 1;
        {
            for (size_t row = 0; row < pixels.row_count(); row++)
                if (white)
                    pixels.at(row, col) = 0xFFFFFF;
                else
                    pixels.at(row, col) = 0xFF0000;
        }
    }
    led_matrix.show(pixels);
}

void fill_pattern(Pixel color)
{
    pixels = led_matrix.pixelMatrix(color);
    led_matrix.show(pixels);
}

void diag1_pattern()
{
    pixels = led_matrix.pixelMatrix();
    for (size_t row = 0; row < pixels.row_count(); row++)
    {
        for (size_t col = 0; col < pixels.column_count(); col++)
        {
            bool white = (row <= col);
            if (white)
                pixels.at(row, col) = 0xFFFFFF;
            else
                pixels.at(row, col) = 0xFFFF00;
        }
    }
    led_matrix.show(pixels);
}

void print_options()
{
    Serial.println("Type a character to select a display pattern:");
    Serial.println("H = horizontal lines");
    Serial.println("V = vertical lines");
    Serial.println("C = chessboard pattern");
    Serial.println("D = fill yellow from top-left to right-bottom in diagonal");
    Serial.println(". = fill black (power off)");
    Serial.println(": = fill white");
}

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    led_matrix.shutdown();
    led_matrix.brightness(127);
    chessboard_pattern();
    print_options();
}

void loop()
{
    int input_chr = Serial.read();
    if ((input_chr == 'H') || (input_chr == 'h'))
        horizontal_pattern();
    if ((input_chr == 'V') || (input_chr == 'v'))
        vertical_pattern();
    if ((input_chr == 'C') || (input_chr == 'c'))
        chessboard_pattern();
    if ((input_chr == 'D') || (input_chr == 'd'))
        diag1_pattern();
    if ((input_chr == '.'))
        fill_pattern(0);
    if ((input_chr == ':'))
        fill_pattern(0xFFFFFF);

    if (input_chr >= ' ')
        print_options();
}
