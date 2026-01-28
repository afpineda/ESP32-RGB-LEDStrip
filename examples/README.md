# Arduino examples

You should edit the example sketches to match your hardware configuration.
All the examples assume that there are eight pixels on the LED strip.
Sketches using the serial monitor must be configured to 115200 bauds.

## [Basic colors](../examples/BasicColors/BasicColors.ino)

Shows the following colors (in ascending order),
one pixel each:
white, red, green, blue, magenta, aqua, yellow and black.
Then it makes them glow by changing the global brightness.

If you type anything into the serial monitor,
the LED strip will shut down for 5 seconds.

Exercised features:

- Pixel display without priorities.
- Global brightness.
- Shutdown.

## [Multi-threaded priority display](../examples/PriorityDisplayThreaded/PriorityDisplayThreaded.ino)

A spawned thread will show a moving white pixel
in an infinite loop.

Type anything into the serial monitor.
The main thread will run a short loop showing a moving red pixel having
higher display priority.
Once the animation is finished,
the moving white pixel will continue.
The spawned thread is never interrupted or blocked.

Exercised features:

- Prioritized display in a multi-threaded application.

## [Non-threaded priority display](../examples/PriorityDisplayNonThreaded/PriorityDisplayNonThreaded.ino)

The main thread will run two pixel animations in an infinite loop:
a moving red pixel and a moving white pixel.
The animation having the highest priority will be shown.

The white pixel animation has a fixed 5 priority.
The red pixel animation starts with 0 priority and is user-selectable.
Type a one-digit number (in the range [0,9]) into the serial monitor to
select another display priority for the red animation.

Exercised features:

- Prioritized display in a non-threaded application.

## [Rainbow colors](../examples/Rainbow/Rainbow.ino)

Rainbow colors will be shown at startup.
Then, those colors will shift up in an infinite loop animation.

Exercised features:

- Pixel display without priorities.
- Pixel displacement.
- Global brightness.

## [Led Matrix patterns](./LedMatrixPatterns/LedMatrixPatterns.ino)

Shows an user-selected pixel pattern in an LED matrix:

- Horizontal lines.
- Vertical lines.
- Chessboard pattern.
- All white
- All black
- Half yellow, half white, in diagonal.

Follow on-screen instructions to select a pattern via the serial monitor.
Type "?" to repeat on-screen instructions.

Exercised features:

- LED matrix constructor.
- Pixel display without priorities.
- Global brightness.
