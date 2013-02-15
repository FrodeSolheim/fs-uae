uname := $(shell uname -a)
ifneq ($(findstring Msys,$(uname)),)
    os = windows
else ifneq ($(findstring Darwin,$(uname)),)
    os = macosx
else ifneq ($(findstring FreeBSD,$(uname)),)
    os = freebsd
else ifneq ($(findstring OpenBSD,$(uname)),)
    os = openbsd
else
    os = linux
endif

android := 0

ifeq ($(android), 1)
	os = android
	cc = arm-linux-androideabi-gcc
	cxx = arm-linux-androideabi-g++
	ar = arm-linux-androideabi-ar
else ifeq ($(os), windows)
	cc = gcc
	cxx = g++
	ar = ar
else ifeq ($(os), macosx)
	cc = gcc
	cxx = g++
	ar = ar
else
	cc = $(CC)
	cxx = $(CXX)
	ar = $(AR)
endif

arch :=
debug := 0
noflags := 0

cppflags = $(CXXFLAGS)
cflags = -std=c99 -Iinclude -Isrc/lua `sdl-config --cflags` \
		`pkg-config --cflags glib-2.0 gthread-2.0 libpng` \
		`pkg-config --cflags freetype2` \
		$(CFLAGS) -D_FILE_OFFSET_BITS=64
objects = obj/emu_emu.o obj/emu_video.o obj/emu_audio_openal.o \
		obj/emu_audio_common.o obj/emu_audio_dummy.o obj/emu_audio_sdl.o \
		obj/emu_audio_debug.o obj/emu_input.o \
		obj/emu_menu.o obj/emu_texture.o obj/emu_font.o \
		obj/emu_render.o obj/emu_dialog.o obj/emu_hud.o obj/emu_actions.o \
		obj/emu_xml_shader.o obj/emu_util.o obj/emu_netplay.o \
		obj/emu_scanlines.o obj/emu_theme.o obj/emu_video_buffer.o \
		obj/emu_video_options.o obj/emu_audio_dummy.o \
		obj/emu_emu_lua.o \
		\
		obj/ml_ml.o obj/ml_windows.o obj/ml_render.o \
		obj/ml_sdl.o obj/ml_rawinput.o obj/ml_macosx.o obj/ml_x11.o \
		\
		obj/base.o obj/log.o obj/config.o obj/image.o obj/ref.o \
		obj/glee.o obj/opengl.o obj/thread.o obj/string.o obj/filesys.o \
		obj/unicode.o obj/init.o obj/random.o obj/list.o obj/eventlog.o \
		obj/queue.o obj/hashtable.o obj/inifile.o obj/time.o \
		obj/lua_lapi.o \
		obj/lua_lauxlib.o \
		obj/lua_lbaselib.o \
		obj/lua_lbitlib.o \
		obj/lua_lcode.o \
		obj/lua_lcorolib.o \
		obj/lua_lctype.o \
		obj/lua_ldblib.o \
		obj/lua_ldebug.o \
		obj/lua_ldo.o \
		obj/lua_ldump.o \
		obj/lua_lfunc.o \
		obj/lua_lgc.o \
		obj/lua_linit.o \
		obj/lua_liolib.o \
		obj/lua_llex.o \
		obj/lua_lmathlib.o \
		obj/lua_lmem.o \
		obj/lua_loadlib.o \
		obj/lua_lobject.o \
		obj/lua_lopcodes.o \
		obj/lua_loslib.o \
		obj/lua_lparser.o \
		obj/lua_lstate.o \
		obj/lua_lstring.o \
		obj/lua_lstrlib.o \
		obj/lua_ltable.o \
		obj/lua_ltablib.o \
		obj/lua_ltm.o \
		obj/lua_lundump.o \
		obj/lua_lvm.o \
		obj/lua_lzio.o

ldflags = $(LDFLAGS)
libs =

ifeq ($(devel), 1)
	cflags += -Wall -Wno-deprecated-declarations
	cflags += -Werror -Wno-error=unused-function
	#-Werror=implicit-function-declaration
endif

ifeq ($(debug), 1)
	cflags += -g -O0 -fno-inline
else ifneq ($(noflags), 1)
	cflags += -g -O2
endif

ifeq ($(os), windows)
	cppflags += -DWINDOWS -DWIN32
	cppflags += -DUSE_SDL
	cppflags += -DUSE_PNG
	cppflags += -DUSE_GLIB
	cppflags += -DUSE_SDL_VIDEO
	cppflags += -DUSE_OPENGL
	#cppflags += -DUSE_SDL_INPUT
	cppflags += -DUSE_OPENAL
	cppflags += -DUSE_FREETYPE
	library = libfsemu.a

else ifeq ($(android), 1)
	cppflags += -DANDROID
	cppflags += -DUSE_OPENGL
	cppflags += -DUSE_GLES
	cppflags += -DUSE_PTHREADS
	library = libfsemu.a

else ifeq ($(os), macosx)
	uname_m := $(shell uname -m)
	ifneq ($(arch),)
	else ifneq ($(findstring i386,$(uname_m)),)
	arch = i386
	else ifneq ($(findstring Power,$(uname_m)),)
	arch = ppc
	else
	arch = x86_64
	endif
	cflags += -arch $(arch)
	cxxflags += -arch $(arch)
	ldflags += -arch $(arch)
	cppflags += -DMACOSX
	cppflags += -I/System/Library/Frameworks/OpenAL.framework/Headers
	cppflags += -DUSE_SDL
	cppflags += -DUSE_PNG
	cppflags += -DUSE_GLIB
	cppflags += -DUSE_SDL_VIDEO
	cppflags += -DUSE_OPENGL
	cppflags += -DUSE_SDL_INPUT
	cppflags += -DUSE_OPENAL
	cppflags += -DUSE_FREETYPE
	library = libfsemu.a

else
	ifeq ($(os), linux)
		cppflags += -DLINUX
	else ifeq ($(os), freebsd)
		cppflags += -DFREEBSD
	else ifeq ($(os), openbsd)
		cppflags += -DOPENBSD
	endif
	cppflags += -DUSE_SDL
	cppflags += -DUSE_PNG
	cppflags += -DUSE_GLIB
	cppflags += -DUSE_X11
	cppflags += -DUSE_SDL_VIDEO
	cppflags += -DUSE_OPENGL
	cppflags += -DUSE_SDL_INPUT
	cppflags += -DUSE_OPENAL
	cppflags += -DUSE_FREETYPE
	library = libfsemu.a
	shared = libfsemu.so.1.0
	shared_soname = libfsemu.so.1
	shared_linkname = libfsemu.so
	shared_ldflags += -shared -Wl,--as-needed -Wl,-soname=$(library_soname)

endif

ifeq ($(os), android)
else
	cppflags += -DWITH_NETPLAY -DWITH_XML_SHADER -DWITH_LUA
	#cppflags += -DWITH_NETPLAY -DUSE_GLES -DFAKE_GLES
endif

all: libfsemu

libfsemu: out/$(library)

obj/%.o: src/%.c
	$(cc) $(cppflags) $(cflags) -c $< -o $@

obj/emu_%.o: src/emu/%.c
	$(cc) $(cppflags) $(cflags) -c $< -o $@

obj/ml_%.o: src/ml/%.c
	$(cc) $(cppflags) $(cflags) -c $< -o $@

obj/lua_%.o: src/lua/%.c
	$(cc) $(cppflags) $(cflags) -c $< -o $@

out/$(library): $(objects)
ifeq ($(os), macosx)
	rm -f $@
endif
	$(ar) cru $@ $(objects)
ifeq ($(os), macosx)
	ranlib $@
endif

out/$(shared): $(objects)
	$(cc) $(shared_ldflags) $(objects) $(libs) -o $@

fs-test:
	make -C test

clean:
	rm -f obj/*.o
	rm -f out/*.a
	rm -f out/*.so*
	#make -C test clean

prefix := /usr

install:
	mkdir -p $(DESTDIR)$(prefix)/include
	cp -a include/* $(DESTDIR)$(prefix)/include/
	mkdir -p $(DESTDIR)$(prefix)/lib
	cp -a out/$(shared) $(DESTDIR)$(prefix)/lib/
	cp -a out/$(library) $(DESTDIR)$(prefix)/lib/
	ln -s $(shared) $(DESTDIR)$(prefix)/lib/$(shared_linkname)
	mkdir -p $(DESTDIR)$(prefix)/lib/pkgconfig/
	cp -a *.pc $(DESTDIR)$(prefix)/lib/pkgconfig/

