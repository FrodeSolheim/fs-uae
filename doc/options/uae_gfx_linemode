Description: Controls how lines are doubled and interlaced modes are handled
Example: double2
Type: choice

Value: none (Single / Single)

Value: double (Double / Double Frames)
value: double2 (Double / Double Fields)
Value: double3 (Double / Double Fields+)

Value: scanlines (Scanlines / Double Frames)
Value: scanlines2 (Scanlines / Double Fields)
Value: scanlines3 (Scanlines / Double Fields+)

Value: scanlines2p (Double Fields / Double Frames)
value: scanlines2p2 (Double Fields / Double Fields)
Value: scanlines2p3 (Double Fields / Double Fields+)

Value: scanlines3p (Double Fields+ / Double Frames)
Value: scanlines3p2 (Double Fields+ / Double Fields)
Value: scanlines3p3 (Double Fields+ / Double Fields+)

Single:
Non-interlaced lines are not doubled.

Double:
Non-interlaced lines are doubled in height.

Scanlines:
Black lines are inserted between each non-interlaced lines.

Double Frames:
Odd and even field pairs are combined to single frame, display update rate
becomes effectively halved (50Hz -> 25Hz).

Double Fields:
Odd and even fields are shown alternately. When odd field is shown, previous
even field is not changed or erased and vice versa.

Double Fields+:
Same as above but instead of not changing previous field, previous field is
blanked. Sort of emulates really really short persistence CRT display. 
