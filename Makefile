include common.mk
version := $(strip $(shell cat VERSION))
series := $(strip $(shell cat SERIES))

ifeq ($(wildcard libfsemu),)
    libfsemu_dir := "../libfsemu"
else
    libfsemu_dir := "libfsemu"
endif

all: fs-uae fs-uae-device-helper mo

#cppflags = -DFSEMU -DFSUAE -D_FILE_OFFSET_BITS=64 $(CPPFLAGS)
cppflags = -DFSEMU=1 -DFSUAE=1 -D_FILE_OFFSET_BITS=64

strict := 0
ifeq ($(strict), 1)
cppflags += -Werror
endif

ifeq ($(debug), 1)
cppflags += -DDEBUG
endif

# libfsemu_dir = libfsemu
libfsemu-target:
	$(make) -C $(libfsemu_dir) debug=$(debug) devel=$(devel) \
		optimize=$(optimize) android=$(android) sdl=$(sdl)

#ifeq ($(wildcard libfs-capsimage), libfs-capsimage)
#libfs-capsimage_dir = libfs-capsimage
#libfs-capsimage/out/CAPSImg.dll:
#	make -C libfs-capsimage
#libfs-capsimage-target: libfs-capsimage/out/CAPSImg.dll
#else
#libfs-capsimage_dir = ../libfs-capsimage
#libfs-capsimage-target:
#endif

ifeq ($(android), 1)
use_glib := 0
warnings = 
errors = -Werror=implicit-function-declaration -Werror=return-type
cxxflags = $(warnings) $(errors) -Isrc/od-fs -Isrc/od-fs/include \
		-Isrc/include -Igen -Isrc \
		-I$(libfsemu_dir)/include \
		-Wno-write-strings -fpermissive

cflags = -std=gnu99 $(cxxflags)
ldflags =
libs = -L$(libfsemu_dir)/out -lfsemu -lpng -lz

else
use_glib := 1
use_freetype := 1
sdl := 2

#ifeq ($(os), macosx)
#	sdl = 2
#endif

ifeq ($(sdl), 2)
	sdl_config = sdl2-config
	use_sdl = USE_SDL2
else
	sdl_config = sdl-config
	use_sdl = USE_SDL
endif

common_flags = -Isrc/od-fs -Isrc/od-fs/include \
		-Isrc/include -Igen -Isrc/jit -Isrc \
		`pkg-config --cflags glib-2.0 gthread-2.0 libpng` \
		-I$(libfsemu_dir)/include \
		-I$(libfsemu_dir)/src/lua \
		`$(sdl_config) --cflags`
cflags = $(common_flags) -std=gnu99 $(CFLAGS)
#cxxflags = $(common_flags) -fpermissive $(CXXFLAGS)
cxxflags = $(common_flags) $(CXXFLAGS)
ldflags = $(LDFLAGS)
libs = -L$(libfsemu_dir)/out -lfsemu `$(sdl_config) --libs` \
		`pkg-config --libs libpng` -lz

ifeq ($(devel), 1)
	warnings = -Wno-unused-variable -Wno-unused-function -Wno-write-strings \
			-Wno-unused-but-set-variable
	errors = -Werror=implicit-function-declaration -Werror=return-type
	cflags += $(warnings) $(errors)
	cxxflags += $(warnings) $(errors)
endif

endif

ifeq ($(use_glib), 1)
	libs += `pkg-config --libs glib-2.0 gthread-2.0`
else ifeq ($(android), 1)
else
	libs += -lrt -lpthread
endif

ifeq ($(use_freetype), 1)
	libs += `pkg-config --libs freetype2`
endif

profile_generate := 0
profile_use := 0

ifeq ($(optimize), 1)
	cflags += -O2 -ggdb
	cxxflags += -O2 -ggdb
else ifneq ($(noflags), 1)
	cflags += -O0 -ggdb
	cxxflags += -O0 -ggdb
endif

ifeq ($(profile_generate), 1)
	cflags += -fprofile-generate
	cxxflags += -fprofile-generate
	ldflags += -fprofile-generate
	libs += -lgcov
endif

ifeq ($(profile_use), 1)
	cflags += -fprofile-use
	cxxflags += -fprofile-use
endif

ifeq ($(force_32bit),1)
	cflags += -m32
	cxxflags += -m32
	ldflags += -m32
endif

#uae_warn = -Wno-unused-value -Wno-uninitialized -Wno-sign-compare
#uae_warn += -fpermissive -Wno-unused-function -Wno-format
#uae_warn +=  -Wmissing-braces -Wall -Wno-sign-compare
uae_warn = -Wall -Wno-sign-compare
#generate = 0
generate = 1

ifeq ($(os), android)
  cppflags += -DANDROID
  cxxflags += 
  libs += -lGLESv1_CM
else ifeq ($(os), windows)
  cppflags +=-DWINDOWS=1
  #cxxflags += -U_WIN32 -UWIN32
  #cxxflags += -D_WIN32 -DWIN32
  libs += -lOpenGL32 -lGLU32 -lgdi32 -lWinmm -lOpenAL32 -lWs2_32 -lWininet

  ifeq ($(devel), 1)
  libs += -mno-windows
  endif


else ifeq ($(os), macosx)
  uname_m := $(shell uname -m)
  ifneq ($(arch),)
  else ifneq ($(findstring i386,$(uname_m)),)
    arch = i386
  else ifneq ($(findstring Power,$(uname_m)),)
    arch = ppc
  else
    arch = x86_64
    # arch = i386
  endif
  cflags += -arch $(arch)
  cxxflags += -arch $(arch)
  ldflags += -arch $(arch) -headerpad_max_install_names
  cppflags += -DMACOSX=1 -I/System/Library/Frameworks/OpenAL.framework/Headers
  libs += -framework OpenGL -framework Carbon -framework OpenAL -framework IOKit
else ifeq ($(os), kfreebsd)
  cppflags += -DFREEBSD=1
  libs += -lGL -lGLU -lopenal -ldl -lX11
else ifeq ($(os), freebsd)
  cppflags += -DFREEBSD=1
  libs += -lGL -lGLU -lopenal -lX11 -lcompat
else ifeq ($(os), openbsd)
  cppflags += -DOPENBSD=1
  libs += -lGL -lGLU -lopenal -lX11 -lcompat
else
  cppflags += -DLINUX=1
  ldflags += -Wa,--execstack
  libs += -lGL -lGLU -lopenal -ldl -lX11
  #generate = 0
endif

ifneq ($(os), android)
	cppflags += -D$(use_sdl) -DUSE_GLIB=1
	#-DWITH_LUA=1
endif

device_helper_objects = obj/fs-uae/device-helper.o

objects = \
obj/fs-uae/config.o \
obj/fs-uae/input.o \
obj/fs-uae/joystick.o \
obj/fs-uae/keyboard.o \
obj/fs-uae/luascript.o \
obj/fs-uae/main.o \
obj/fs-uae/menu.o \
obj/fs-uae/mouse.o \
obj/fs-uae/paths.o \
obj/fs-uae/recording.o \
obj/fs-uae/uae_config.o \
obj/fs-uae/version.o \
obj/fs-uae/video.o

ifeq ($(os), windows)
objects += obj/fs-uae.res
endif

uae_objects = \
obj/gen/blitfunc.o \
obj/gen/blittable.o \
obj/gen/cpudefs.o \
obj/gen/cpuemu_0.o \
obj/gen/cpuemu_11.o \
obj/gen/cpuemu_13.o \
obj/gen/cpuemu_20.o \
obj/gen/cpuemu_21.o \
obj/gen/cpuemu_22.o \
obj/gen/cpuemu_31.o \
obj/gen/cpuemu_32.o \
obj/gen/cpuemu_33.o \
obj/gen/cpustbl.o \
obj/a2091.o \
obj/akiko.o \
obj/amax.o \
obj/ar.o \
obj/arcadia.o \
obj/aros.rom.o \
obj/audio.o \
obj/autoconf.o \
obj/blitter.o \
obj/blkdev.o \
obj/blkdev_cdimage.o \
obj/bsdsocket.o \
obj/calc.o \
obj/cd32_fmv.o \
obj/cdrom.o \
obj/cdtv.o \
obj/cia.o \
obj/cfgfile.o \
obj/consolehook.o \
obj/cpummu.o \
obj/cpummu30.o \
obj/crc32.o \
obj/custom.o \
obj/debug.o \
obj/disk.o \
obj/diskutil.o \
obj/dongle.o \
obj/drawing.o \
obj/driveclick.o \
obj/enforcer.o \
obj/events.o \
obj/expansion.o \
obj/fdi2raw.o \
obj/filesys.o \
obj/fpp.o \
obj/fsdb.o \
obj/fsusage.o \
obj/gayle.o \
obj/gfxboard.o \
obj/gfxutil.o \
obj/hardfile.o \
obj/hrtmon.rom.o \
obj/identify.o \
obj/inputdevice.o \
obj/inputrecord.o \
obj/isofs.o \
obj/keybuf.o \
obj/luascript.o \
obj/main.o \
obj/memory.o \
obj/missing.o \
obj/native2amiga.o \
obj/newcpu.o \
obj/newcpu_common.o \
obj/ncr_scsi.o \
obj/readcpu.o \
obj/rommgr.o \
obj/savestate.o \
obj/scp.o \
obj/scsi.o \
obj/scsiemul.o \
obj/scsitape.o \
obj/specialmonitors.o \
obj/statusline.o \
obj/traps.o \
obj/uaeexe.o \
obj/uaelib.o \
obj/uaenative.o \
obj/uaeresource.o \
obj/uaeserial.o \
obj/zfile.o \
obj/zfile_archive.o \
obj/gen/compemu.o \
obj/jit/compemu_fpp.o \
obj/jit/compemu_support.o \
obj/gen/compstbl.o \
obj/archivers/dms/crc_csum.o \
obj/archivers/dms/getbits.o \
obj/archivers/dms/maketbl.o \
obj/archivers/dms/pfile.o \
obj/archivers/dms/tables.o \
obj/archivers/dms/u_deep.o \
obj/archivers/dms/u_heavy.o \
obj/archivers/dms/u_init.o \
obj/archivers/dms/u_medium.o \
obj/archivers/dms/u_quick.o \
obj/archivers/dms/u_rle.o \
obj/archivers/zip/unzip.o \
obj/od-fs/ahi_v2.o \
obj/od-fs/ahi_winuae.o \
obj/od-fs/audio.o \
obj/od-fs/bsdsocket_host.o \
obj/od-fs/blkdev-linux.o \
obj/od-fs/caps.o \
obj/od-fs/cda_play.o \
obj/od-fs/charset.o \
obj/od-fs/clock.o \
obj/od-fs/cdimage_stubs.o \
obj/od-fs/clipboard.o \
obj/od-fs/driveclick.o \
obj/od-fs/filesys_host.o \
obj/od-fs/fsdb_host.o \
obj/od-fs/hardfile_host.o \
obj/od-fs/gui.o \
obj/od-fs/input.o \
obj/od-fs/keymap.o \
obj/od-fs/libamiga.o \
obj/od-fs/logging.o \
obj/od-fs/mman.o \
obj/od-fs/parser.o \
obj/od-fs/random.o \
obj/od-fs/paths.o \
obj/od-fs/roms.o \
obj/od-fs/picasso96.o \
obj/od-fs/serial_host.o \
obj/od-fs/stubs.o \
obj/od-fs/support.o \
obj/od-fs/util.o \
obj/od-fs/threading.o \
obj/od-fs/uae_host.o \
obj/od-fs/uaemisc.o \
obj/od-fs/version.o \
obj/od-fs/video.o \
obj/qemuvga/cirrus_vga.o \
obj/qemuvga/lsi53c895a.o \
obj/qemuvga/qemuuaeglue.o \
obj/qemuvga/vga.o

#obj/chd-archiver-astring.o
#obj/chd-archiver-chdcdrom.o
#obj/chd-archiver-chdcodec.o
#obj/chd-archiver-chd.o
#obj/chd-archiver-flac.o
#obj/chd-archiver-hashing.o
#obj/chd-archiver-huffman.o
#obj/chd-archiver-md5.o
#obj/chd-archiver-sha1.o

gen_deps = \
gen/blit.h \
gen/comptbl.h \
gen/linetoscr.cpp

ifeq ($(generate), 1)

gen/genblitter: obj/genblitter.o obj/blitops.o obj/writelog.o
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) obj/genblitter.o obj/blitops.o \
	obj/writelog.o -o gen/genblitter

gen/gencpu: gen/cpudefs.cpp obj/gencpu.o obj/readcpu.o obj/missing.o \
		gen/cpudefs.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) obj/gencpu.o obj/readcpu.o obj/missing.o \
	gen/cpudefs.cpp -o gen/gencpu

gen/genlinetoscr: src/genlinetoscr.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) src/genlinetoscr.cpp \
	 -o gen/genlinetoscr

gen/gencomp: src/jit/gencomp.cpp obj/readcpu.o obj/gen/cpudefs.o obj/missing.o
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) src/jit/gencomp.cpp obj/readcpu.o \
	obj/gen/cpudefs.o obj/missing.o -o gen/gencomp

gen/linetoscr.cpp: gen/genlinetoscr
	gen/genlinetoscr > gen/linetoscr.cpp

gen/blit.h: gen/genblitter
	gen/genblitter i > gen/blit.h

gen/blitfunc.cpp: gen/genblitter gen/blitfunc.h
	gen/genblitter f > gen/blitfunc.cpp

gen/blitfunc.h: gen/genblitter
	gen/genblitter h > gen/blitfunc.h

gen/blittable.cpp: gen/genblitter gen/blitfunc.h
	gen/genblitter t > gen/blittable.cpp

gen/comptbl.h: gen/compemu.cpp
gen/compstbl.cpp: gen/compemu.cpp

gen/compemu.cpp: gen/gencomp
	mkdir -p `dirname $@`
	gen/gencomp

gen/build68k: src/build68k.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) src/build68k.cpp src/writelog.cpp -o gen/build68k

gen/cpudefs.cpp: gen/build68k src/table68k
	./gen/build68k < src/table68k > gen/cpudefs.cpp
	#python util/fix_tchar.py gen/cpudefs.cpp

gen/cpustbl.cpp: gen/cpuemu_0.cpp
gen/cputbl.h: gen/cpuemu_0.cpp
gen/cpuemu_11.cpp: gen/cpuemu_0.cpp
gen/cpuemu_13.cpp: gen/cpuemu_0.cpp
gen/cpuemu_20.cpp: gen/cpuemu_0.cpp
gen/cpuemu_21.cpp: gen/cpuemu_0.cpp
gen/cpuemu_22.cpp: gen/cpuemu_0.cpp
gen/cpuemu_31.cpp: gen/cpuemu_0.cpp
gen/cpuemu_32.cpp: gen/cpuemu_0.cpp
gen/cpuemu_33.cpp: gen/cpuemu_0.cpp

gen/cpuemu_0.cpp: gen/gencpu
	cd gen && ./gencpu --optimized-flags

endif

# Using GCC optimization level O0 instead of O2 for cpuemu*, since a bug
# was found caused by the optimizer (where basically (1 ^ 0) & (1 ^ 0) was
# evaluated to 0). This fixes Tower of Babel (IPF).

obj/gen/cpuemu%.o: gen/cpuemu%.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -O0 -c $< -o $@

obj/gen/%.o: gen/%.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -c $< -o $@

ifeq ($(strict), 1)

obj/a2091.o: src/a2091.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=sometimes-uninitialized -c $< -o $@

obj/bsdsocket.o: src/bsdsocket.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=unneeded-internal-declaration -c $< -o $@

obj/cdtv.o: src/cdtv.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=tautological-compare -c $< -o $@

obj/custom.o: src/custom.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=tautological-compare -c $< -o $@

obj/debug.o: src/debug.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=constant-logical-operand -c $< -o $@

obj/disk.o: src/disk.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=switch -Wno-error=tautological-constant-out-of-range-compare -c $< -o $@

obj/filesys.o: src/filesys.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=tautological-compare -c $< -o $@

obj/hardfile.o: src/hardfile.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=unused-label -c $< -o $@

obj/inputdevice.o: src/inputdevice.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=logical-op-parentheses -c $< -o $@

obj/inputrecord.o: src/inputrecord.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=unused-variable -Wno-error=format -c $< -o $@

obj/isofs.o: src/isofs.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=tautological-constant-out-of-range-compare -Wno-error=format -Wno-error=tautological-compare -c $< -o $@

obj/memory.o: src/memory.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=constant-logical-operand -Wno-error=tautological-compare -c $< -o $@

obj/rommgr.o: src/rommgr.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=missing-braces -Wno-error=deprecated-writable-strings -c $< -o $@

obj/scsiemul.o: src/scsiemul.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=unused-variable -c $< -o $@

obj/zfile.o: src/zfile.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=logical-op-parentheses -Wno-error=tautological-compare -c $< -o $@

obj/zfile_archive.o: src/zfile_archive.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -Wno-error=tautological-compare -c $< -o $@

obj/od-fs/ahi_v2.o: src/od-fs/ahi_v2.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error -c $< -o $@

obj/od-fs/ahi_winuae.o: src/od-fs/ahi_winuae.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error=unused-variable -c $< -o $@

obj/od-fs/clipboard.o: src/od-fs/clipboard.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-unused-variable -c $< -o $@

obj/od-fs/mman.o: src/od-fs/mman.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error -c $< -o $@

obj/od-fs/picasso96.o: src/od-fs/picasso96.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error -c $< -o $@

obj/od-fs/video.o: src/od-fs/video.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error=unused-variable -Wno-error=unused-function -c $< -o $@

obj/qemuvga/%.o: src/qemuvga/%.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) -Wno-error=missing-braces -c $< -o $@

endif

obj/readcpu.o: src/readcpu.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/genblitter.o: src/genblitter.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/%.o: src/%.cpp gen/blit.h gen/comptbl.h gen/linetoscr.cpp
	mkdir -p `dirname $@`
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/fs-uae.res: src/fs-uae/fs-uae.rc
	mkdir -p `dirname $@`
	windres $< -O coff -o $@

obj/fs-uae/%.o: src/fs-uae/%.c
	mkdir -p `dirname $@`
	$(cc) $(cppflags) $(cflags) -c $< -o $@

share/locale/%/LC_MESSAGES/fs-uae.mo: po/%.po
	mkdir -p share/locale/$*/LC_MESSAGES
	msgfmt --verbose $< -o $@

catalogs = \
	share/locale/cs/LC_MESSAGES/fs-uae.mo \
	share/locale/da/LC_MESSAGES/fs-uae.mo \
	share/locale/de/LC_MESSAGES/fs-uae.mo \
	share/locale/es/LC_MESSAGES/fs-uae.mo \
	share/locale/fi/LC_MESSAGES/fs-uae.mo \
	share/locale/fr/LC_MESSAGES/fs-uae.mo \
	share/locale/it/LC_MESSAGES/fs-uae.mo \
	share/locale/nb/LC_MESSAGES/fs-uae.mo \
	share/locale/pl/LC_MESSAGES/fs-uae.mo \
	share/locale/pt/LC_MESSAGES/fs-uae.mo \
	share/locale/sr/LC_MESSAGES/fs-uae.mo \
	share/locale/tr/LC_MESSAGES/fs-uae.mo

mo: $(catalogs)

objects: $(uae_objects) $(objects)

gen-deps-target: $(gen_deps)

fs-uae: libfsemu-target gen-deps-target $(uae_objects) $(objects)
	rm -f fs-uae
	$(cxx) $(ldflags) $(objects) $(uae_objects) $(libs) -o fs-uae

fs-uae-device-helper: libfsemu-target $(device_helper_objects)
	rm -f fs-uae-device-helper
	$(cxx) $(ldflags) $(device_helper_objects) $(libs) -o fs-uae-device-helper

build_dir := "."
dist_name = fs-uae-$(version)
dist_dir := $(build_dir)/$(dist_name)
# dist_file := $(build_dir)/$(dist_name).tar.gz

distdir-base:
	rm -Rf $(dist_dir)/*
	mkdir -p $(dist_dir)
	# cp -a $(dist_dir_launcher) $(dist_dir)/launcher

	mkdir -p $(dist_dir)/obj
	touch $(dist_dir)/obj/.dummy
	mkdir -p $(dist_dir)/out
	touch $(dist_dir)/out/.dummy
	cp -a INSTALL README COPYING VERSION SERIES ChangeLog $(dist_dir)
	cp -a common.mk targets.mk $(dist_dir)
	# windows.mk macosx.mk debian.mk
	cp -a Makefile fs-uae.spec example.conf $(dist_dir)
	cp -a src contrib share licenses $(dist_dir)
	rm -Rf $(dist_dir)/src/od-win32
	rm -Rf $(dist_dir)/src/prowizard
	rm -Rf $(dist_dir)/src/archivers/lha
	rm -Rf $(dist_dir)/src/archivers/lzx
	rm -Rf $(dist_dir)/src/archivers/xfd
	rm -Rf $(dist_dir)/src/jit2
	rm -f $(dist_dir)/src/akiko2.cpp
	rm -f $(dist_dir)/src/custom2.cpp
	rm -f $(dist_dir)/src/core.cw4.cpp
	rm -f $(dist_dir)/src/catweasel.cpp

	find $(dist_dir)/share -name "*.mo" -delete
	#mkdir -p $(dist_dir)/gensrc
	#cp -a gensrc/*.cpp gensrc/*.h $(dist_dir)/gensrc

	mkdir -p $(dist_dir)/libfsemu
	cp -a $(libfsemu_dir)/COPYING $(dist_dir)/libfsemu
	cp -a $(libfsemu_dir)/README $(dist_dir)/libfsemu
	cp -a $(libfsemu_dir)/Makefile $(dist_dir)/libfsemu
	cp -a $(libfsemu_dir)/include $(dist_dir)/libfsemu
	cp -a $(libfsemu_dir)/src $(dist_dir)/libfsemu
	mkdir -p $(dist_dir)/libfsemu/obj
	touch $(dist_dir)/libfsemu/obj/.dummy
	mkdir -p $(dist_dir)/libfsemu/out
	touch $(dist_dir)/libfsemu/out/.dummy

	mkdir -p $(dist_dir)/po
	cp -a po/*.po $(dist_dir)/po/
	cp -a po/update.py $(dist_dir)/po/update.py
	cp -a po/update.sh $(dist_dir)/po/update.sh

	mkdir -p $(dist_dir)/doc
	cp -a doc/Default.fs-uae $(dist_dir)/doc/

	mkdir -p $(dist_dir)/macosx
	cp -a macosx/Makefile $(dist_dir)/macosx/
	cp -a macosx/fs-make-standalone-app.py $(dist_dir)/macosx/
	cp -a macosx/template $(dist_dir)/macosx/

	mkdir -p $(dist_dir)/windows
	cp -a windows/Makefile $(dist_dir)/windows/
	# cp -a windows/launcher-proxy.exe $(dist_dir)/windows/
	# cp -a windows/game-center-proxy.exe $(dist_dir)/windows/
	cp -a windows/replace_icon.py $(dist_dir)/windows/
	cp -a windows/fs-uae.iss $(dist_dir)/windows/
	# cp -a windows/launcher.iss $(dist_dir)/windows/
	# cp -a windows/game-center.iss $(dist_dir)/windows/

	mkdir -p $(dist_dir)/debian
	cp -a debian/changelog $(dist_dir)/debian/
	cp -a debian/compat $(dist_dir)/debian/
	cp -a debian/control $(dist_dir)/debian/
	cp -a debian/copyright $(dist_dir)/debian/
	cp -a debian/rules $(dist_dir)/debian/
	cp -a debian/source $(dist_dir)/debian/

	mkdir -p $(dist_dir)/util
	#cp -a util/fix_64_bit.py $(dist_dir)/util/
	#cd $(dist_dir) && python util/fix_64_bit.py
	cp -a util/update-version.py $(dist_dir)/util/

	mkdir -p $(dist_dir)/icon
	cp icon/fs-uae.ico $(dist_dir)/icon/
	cp icon/fs-uae.icns $(dist_dir)/icon/
	cp icon/fs-uae-launcher.ico $(dist_dir)/icon/
	cp icon/fs-uae-launcher.icns $(dist_dir)/icon/
	cp icon/fs-uae-config.icns $(dist_dir)/icon/
	
	find $(dist_dir) -name "*~" -delete

distdir: distdir-base
	cd $(dist_dir) && python util/update-version.py

distcheck: distdir
	cd $(dist_dir) && $(make)

po-dist:
	mkdir -p dist/$(series)/po/fs-uae
	cp po/*.po dist/$(series)/po/fs-uae/

	# mkdir -p dist/$(series)/po/fs-uae-launcher
	# cp launcher/po/*.po dist/$(series)/po/fs-uae-launcher/

#dist: distdir pubfiles-source po-dist
dist: distdir po-dist
	# find $(dist_dir_launcher) -exec touch \{\} \;
	find $(dist_dir) -exec touch \{\} \;

	# tar zcfv fs-uae-launcher-$(version).tar.gz $(dist_dir_launcher)
	cd "$(build_dir)" && tar zcfv $(dist_name).tar.gz $(dist_name)
	# mkdir -p dist/$(series)/$(version)
	# mv fs-uae-$(version).tar.gz dist/$(series)/$(version)/
	# mv fs-uae-launcher-$(version).tar.gz dist/$(series)/$(version)/
	# mkdir -p dist/files/
	# cp doc/Default.fs-uae dist/files/
	# cp server/fs_uae_netplay_server/game.py \
	# 	dist/files/fs-uae-netplay-server.py
	# cp server/fs_uae_netplay_server/game.py \
	# 	dist/$(series)/$(version)/fs-uae-game-server-$(version).py

install:
	install -d $(DESTDIR)$(prefix)/bin
	install fs-uae $(DESTDIR)$(prefix)/bin/fs-uae
	install fs-uae-device-helper $(DESTDIR)$(prefix)/bin/fs-uae-device-helper
	install -d $(DESTDIR)$(prefix)/share
	cp -R share/* $(DESTDIR)$(prefix)/share

	install -d $(DESTDIR)$(docdir)
	cp README COPYING example.conf $(DESTDIR)$(docdir)

debsrc: dist
	# test -f $(build_dir)/fs-uae_$(version).orig.tar.gz || cp $(build_dir)/fs-uae-$(version).tar.gz $(build_dir)/fs-uae_$(version).orig.tar.gz
	mv $(build_dir)/fs-uae-$(version).tar.gz $(build_dir)/fs-uae_$(version).orig.tar.gz

	sed -i "s/-0[)] unstable;/-$(debversion)) $(debseries);/g" $(dist_dir)/debian/changelog
	cd $(dist_dir) && dpkg-buildpackage -S -us -uc

deb: debsrc
	# cd $(dist_dir) && fakeroot debian/rules binary
	# mkdir -p dist/$(series)/$(version)
	# mv build/fs-uae_$(version)-*deb dist/$(series)/$(version)/
	cd $(dist_dir) && dpkg-buildpackage -us -uc

windows-dist: distdir
	cd $(dist_dir)/windows && make
	mv $(dist_dir)/fs-uae_*windows* .
	rm -Rf $(dist_dir)

macosx-dist: distdir
	cd $(dist_dir)/macosx && make
	mv $(dist_dir)/fs-uae_*macosx* .
	rm -Rf $(dist_dir)

dist_dir_launcher := fs-uae-launcher-$(version)

debseries := unstable
debversion := $(shell date +"%s")

include targets.mk

clean-dist:
	rm -Rf build dist fs-uae-[0-9]* fs-uae_*

clean:
	$(make) -C $(libfsemu_dir) clean
	rm -Rf gen obj
	rm -f fs-uae fs-uae.exe
	rm -f fs-uae-device-helper fs-uae-device-helper.exe

distclean: clean clean-dist
