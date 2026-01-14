/**
 * @file PriorityDisplayNonThreaded.ino
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Non-threaded priority display
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
uint8_t selectedPriority = 0;
LED_STRIP_CLASS strip(DATA_PIN, OPEN_DRAIN, true, REVERSED);

//------------------------------------------------------------------
// Arduino entry point
//------------------------------------------------------------------

void setup()
{
    strip.brightness(127);
    strip.shutdown(8);
    Serial.begin(115200);
    Serial.println("==Display priority test==");
    Serial.println("White color has display priority 5");
    Serial.println("Red color has display priority 0");
    Serial.println("Type a display priority for the red color (0-9) in the serial monitor.");
}

void loop()
{
    while (Serial.available())
    {
        int c = Serial.read();
        if ((c >= '0') && (c <= '9'))
        {
            Serial.printf("New priority for the red color: %c\n", c);
            selectedPriority = c - '0';
        }
    }
    RgbGuard white_guard(strip, 5);
    RgbGuard red_guard(strip, selectedPriority);
    PixelVector white_pixels(8);
    PixelVector red_pixels(8);
    for (int i = 0; i < 8; i++)
    {
        red_pixels.fill(0);
        red_pixels[i] = 0xFF0000;
        white_pixels.fill(0);
        white_pixels[i] = 0xFFFFFF;
        red_guard.show(red_pixels);
        white_guard.show(white_pixels);
        delay(LOOP_DELAY);
    }
}