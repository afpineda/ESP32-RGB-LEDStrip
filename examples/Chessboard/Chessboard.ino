/**
 * @file Checkboard.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Show a chessboard-like pattern in an LED matrix and animate it
 *
 * @date 2026-01-17
 *
 * @copyright Under EUPL 1.2 License
 */

#include "LEDMatrix.hpp"

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
#define WIDTH 2

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
#define LED_STRIP_DRIVER WS2812

//------------------------------------------------------------------
// Non-configurable globals (do not touch)
//------------------------------------------------------------------

#define LOOP_DELAY 1000
LEDMatrix led_matrix(
    DATA_PIN,
    OPEN_DRAIN,
    true,
    LED_STRIP_DRIVER,
    params);
PixelMatrix pixels;
bool animate = false;

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    led_matrix.shutdown();
    led_matrix.brightness(127);

    pixels = led_matrix.pixelMatrix();

    for (size_t row = 0; row < pixels.row_count(); row++)
    {
        bool white = (row % 2) ? 0 : 1;
        for (size_t col = 0; col < pixels.column_count(); col++)
        {
            if (white)
                pixels.at(row, col) = 0xFFFFFF;
            else
                pixels.at(row, col) = 0x00FF00;
            white = !white;
        }
    }
    led_matrix.show(pixels);
}

void loop()
{
    if (Serial.available())
    {
        while (Serial.read() >= 0)
            ;
        animate = !animate;
    }
    if (animate)
    {
        delay(LOOP_DELAY);
        pixels.scroll_down(1);
        led_matrix.show(pixels);
    }
}
