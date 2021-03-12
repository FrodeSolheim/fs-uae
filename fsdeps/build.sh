. fsdeps/system.sh

fsdeps/dep/gettext
fsdeps/dep/glib
fsdeps/dep/libmpeg2
fsdeps/dep/libpng
fsdeps/dep/libsamplerate
if [ "$SYSTEM_OS" = "macOS" ]; then
echo "Skipping OpenGL on macOS"
else
fsdeps/dep/openal
fi
fsdeps/dep/portmidi
fsdeps/dep/SDL2
fsdeps/dep/SDL2_ttf
