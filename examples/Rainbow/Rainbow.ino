/**
 * @file Rainbow.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Show rainbow colors in an LED strip and animate them
 *
 * @date 2025-08-27
 *
 * @copyright Under EUPL 1.2 License
 */

#include <Arduino.h>
#include "LEDStrip.hpp"

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
// True to display pixels in reverse order
#define REVERSED false

// CONFIGURE TO YOUR NEEDS
#define LED_STRIP_CLASS WS2812LEDStrip

//------------------------------------------------------------------
// Non-configurable globals (do not touch)
//------------------------------------------------------------------

#define DEFAULT_DELAY 2000
LED_STRIP_CLASS strip(DATA_PIN, OPEN_DRAIN, true, REVERSED);
PixelVector pixels(8);

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    strip.brightness(125);
    pixels[0] = 0x0EE82E;
    pixels[1] = 0x4B0082;
    pixels[2] = 0x0000FF;
    pixels[3] = 0x008000;
    pixels[4] = 0xFFFF00;
    pixels[5] = 0xFFA500;
    pixels[6] = 0xFF0000;
    strip.show(pixels);
}

void loop()
{
    // Fancy animation
    delay(DEFAULT_DELAY);
    pixels >> 1;
    strip.show(pixels);
}