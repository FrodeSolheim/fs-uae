Example: 74 * 640 400 => * 20 * *

The option viewport specifies a crop rectangle to apply to the Amiga
video-out. The default, if not specified, is to calculate this automatically
(auto-scaling). To force an uncropped viewport (and effectively disable
autoscaling), you can specify:

  viewport = * * * * => 0 0 752 574

If auto-scaling does not work perfectly for a game, and you want a cropped
output, you can specify any crop rectangle (x y width height), The following
viewport is appropriate for many games:

  viewport = * * * * => 74 36 640 400

Often, Amiga games have viewports with varying size and position. The default
auto-scaling system works well with many games, but not all. You can modify
the auto-scaling algorithm by specifying patterns to replace. The viewport
setting will be consulted each time the auto-scaling algorithm chooses a
new viewport.

Here are some examples:

The following viewport pattern will match if the auto-scaling algorithm
thinks the viewport should be 640×400 and start at 74 from the left. The
top can be anything (*). Now, the top parameter will be forced to 20.
A * on the right side of => means to keep the existing value, while
a * on the left side means to match any value.

  viewport = 74 * 640 400 => * 20 * *

The following viewport specification does nothing:

  * * * * => * * * *

This following viewport is a simple translation
(74 40 640 400 => * 36 * * would behave the same):

  74 40 640 400 => 74 36 640 400

You can specify more than one pattern by separating them with commas.
In this case, the first matching pattern will be used. Here is a viewport
specification which works perfectly with Pinball Dreams: (this basically
says: keep the viewport 74 28 640 524 as it is, and all other viewports
will be forced to 74 36 640 512.

  viewport = 74 28 640 524 => * * * *, * * * * => 74 36 640 512

This one is great for Turrican II:

  viewport = 74 * 640 384 => 74 36 640 424

But rembember, many games work perfectly with the builtin viewport detection,
and if you do not want auto-scaling, you can disable it by specifying a fixed
viewport of your choice.
