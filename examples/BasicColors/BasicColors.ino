/**
 * @file BasicColors.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Show basic colors in an LED strip and animate brightness
 *
 * @date 2025-08-31
 *
 * @copyright Under EUPL 1.2 License
 */

#include "LEDStrip.hpp"

//------------------------------------------------------------------
// Globals
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

//------------------------------------------------------------------

// CONFIGURE TO YOUR NEEDS
WS2812LEDStrip strip(DATA_PIN, OPEN_DRAIN, true, REVERSED);

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

#define LOOP_DELAY 50
PixelVector pixels(8);

void setup()
{
    Serial.begin(115200);
    pixels[0] = 0xFFFFFF; // white
    pixels[1] = 0xFF0000; // red
    pixels[2] = 0x00FF00; // green
    pixels[3] = 0x0000FF; // blue
    pixels[4] = 0xFF00FF; // magenta
    pixels[5] = 0x00FFFF; // aqua
    pixels[6] = 0xFFFF00; // yellow
    pixels[7] = 0;        // black
    strip.brightness(255);
    strip.show(pixels);
}

void loop()
{
    if (Serial.available())
    {
        while (Serial.read() >= 0)
            ;
        Serial.println("Shutdown for 5 seconds...");
        strip.shutdown(8);
        delay(5000);
        strip.brightness(255);
        Serial.println("Restarting...");
    }
    else
    {
        delay(LOOP_DELAY);
        strip.brightness(strip.brightness() - 1);
        strip.show(pixels);
    }
}