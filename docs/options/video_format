Description: "Video buffer format and color depth"
Default: bgra
Example: rgb565
Since: 1.1.4
Type: choice

This option allows you to configure whether video data is stored in RGBA or
BGRA byte order in the internal video buffers. This setting (together with
texture_format) may affect texture upload performance. The option should not
be shared between computers, since appropriate values may differ for other
operating system or OpenGL GPU vendors. Valid values are:

Value: bgra ("32-bit BGRA")
       This is the default setting as it has been verified to provide
       best 32-bit color performance with most OpenGL drivers.
Value: rgba ("32-bit RGBA")
       Depending on your system, this may give better 32-bit performance.
Value: rgb565 ("16-bit")
       Using this format means that less data needs to be uploaded to the
       GPU. May be quite a bit faster on slower systems.
