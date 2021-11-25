# High-DPI / Hi-DPI notes

## macOS

This information is not verified to be correct. Terminology might also be
incorrect.

Note: macOS operates with different resolutions:

- Logical points and sizes ("pre-hi-dpi sizes")
- Drawable pixels
- Real screen pixels

E.g. with the default scaled resolution on a 13.3" MacBook Pro, you'll get:

- Logical size: 1440x900
- Drawable size: 2880x1800 (2x of logical size)
- Real monitor size: 2560x1600

This means that FS-UAE will render at 2880x1800 (some UI elements might be in
"1440x900 resolution"), and macOS will scale this down to 2560x1600.

This might cause issues for OpenGL pixel scalers trying to get exact results.
