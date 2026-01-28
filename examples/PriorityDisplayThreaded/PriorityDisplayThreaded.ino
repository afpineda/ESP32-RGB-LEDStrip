/**
 * @file PriorityDisplayThreaded.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Multi-threaded prioritized display
 *
 * @date 2025-08-31
 *
 * @copyright Under EUPL 1.2 License
 */

#include "LEDStrip.hpp"
#include "Arduino.h"
#include <thread>

//------------------------------------------------------------------
// Configurable globals
//------------------------------------------------------------------

// CONFIGURE TO YOUR NEEDS
// Number of pixels in the LED strip
#define PIXEL_COUNT 8

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

#define LOOP_DELAY 50
LED_STRIP_CLASS strip(PIXEL_COUNT, DATA_PIN, OPEN_DRAIN, true, REVERSED);

//------------------------------------------------------------------
// Auxiliary
//------------------------------------------------------------------

void whiteColorAnimation()
{
    RgbGuard guard(strip, 0);
    PixelVector pixels(PIXEL_COUNT);
    while (true)
    {
        for (int i = 0; i < PIXEL_COUNT; i++)
        {
            pixels.fill(0);
            pixels[i] = 0xFFFFFF;
            guard.show(pixels);
            delay(LOOP_DELAY);
        }
    }
}

void redColorAnimation()
{
    RgbGuard guard(strip, 8);
    PixelVector pixels(PIXEL_COUNT);
    for (int i = 0; i < PIXEL_COUNT; i++)
    {
        pixels.fill(0);
        pixels[i] = 0xFF0000;
        guard.show(pixels);
        delay(LOOP_DELAY);
    }
}

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    strip.brightness(127);
    strip.shutdown();
    Serial.begin(115200);
    Serial.println("==Display priority test #2==");
    Serial.println("A white color animation should display.");
    Serial.println("Type any character in the serial monitor to show the red color animation.");
    ::std::thread whiteThread(whiteColorAnimation);
    whiteThread.detach();
}

void loop()
{
    while (Serial.available())
    {
        // remove incoming bytes from the buffer
        while (Serial.read() >= 0)
            ;
        redColorAnimation();
    }
}
