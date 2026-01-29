# Change log and release notes

## 2.1.0

- `RgbGuard::show()` now returns `true` if the guard had the highest display
  priority. `false` otherwise.
- Improved API documentation.

## 2.0.0

- Led matrix support.
  There is a new constructor for them in the `LEDStrip` class.
- The pixel count parameter in `LEDStrip::shutdown()` has been moved
  to the class constructor.

## 1.0.0

- First release.
