
bin_PROGRAMS = \
	fs-uae \
	fs-uae-device-helper

noinst_PROGRAMS = \
	gen/build68k gen/genblitter \
	gen/gencomp \
	gen/gencpu \
	gen/genlinetoscr

BUILT_SOURCES = \
	gen/blit.h \
	gen/blitfunc.cpp \
	gen/blitfunc.h \
	gen/blittable.cpp \
	gen/compemu.cpp \
	gen/compstbl.cpp \
	gen/comptbl.h \
	gen/cpudefs.cpp \
	gen/cpuemu_0.cpp \
	gen/cpuemu_11.cpp \
	gen/cpuemu_13.cpp \
	gen/cpuemu_20.cpp \
	gen/cpuemu_21.cpp \
	gen/cpuemu_22.cpp \
	gen/cpuemu_31.cpp \
	gen/cpuemu_32.cpp \
	gen/cpuemu_33.cpp \
	gen/cpustbl.cpp \
	gen/cputbl.h \
	gen/linetoscr.cpp

gen:
	mkdir -p gen

gen/blit.h: gen/genblitter
	gen/genblitter i > gen/blit.h

gen/blitfunc.cpp: gen/genblitter gen/blitfunc.h
	gen/genblitter f > gen/blitfunc.cpp

gen/blitfunc.h: gen/genblitter
	gen/genblitter h > gen/blitfunc.h

gen/blittable.cpp: gen/genblitter gen/blitfunc.h
	gen/genblitter t > gen/blittable.cpp

gen/compemu.cpp: gen/gencomp
	gen/gencomp

gen/compstbl.cpp: gen/compemu.cpp

gen/comptbl.h: gen/compemu.cpp

gen/cpudefs.cpp: gen/build68k src/table68k
	./gen/build68k < src/table68k > gen/cpudefs.cpp

gen/cpuemu_0.cpp: gen/gencpu
	cd gen && ./gencpu --optimized-flags

gen/cpuemu_11.cpp: gen/cpuemu_0.cpp

gen/cpuemu_13.cpp: gen/cpuemu_0.cpp

gen/cpuemu_20.cpp: gen/cpuemu_0.cpp

gen/cpuemu_21.cpp: gen/cpuemu_0.cpp

gen/cpuemu_22.cpp: gen/cpuemu_0.cpp

gen/cpuemu_31.cpp: gen/cpuemu_0.cpp

gen/cpuemu_32.cpp: gen/cpuemu_0.cpp

gen/cpuemu_33.cpp: gen/cpuemu_0.cpp

gen/cpustbl.cpp: gen/cpuemu_0.cpp

gen/cputbl.h: gen/cpuemu_0.cpp

gen/linetoscr.cpp: gen/genlinetoscr
	gen/genlinetoscr > gen/linetoscr.cpp

COMMON_FLAGS =
COMMON_FLAGS += -fno-strict-overflow
#COMMON_FLAGS += -Wstrict-overflow
COMMON_FLAGS += -I. -Isrc/od-fs -Isrc/od-fs/include
COMMON_FLAGS += -Isrc/include -Igen -Isrc/jit -Isrc
COMMON_FLAGS += -Ilibfsemu/include -Ilibfsemu/src/lua
COMMON_FLAGS += @FREETYPE_CFLAGS@
COMMON_FLAGS += @GLIB_CFLAGS@
COMMON_FLAGS += @LIBMPEG2_CFLAGS@
COMMON_FLAGS += @LUA_CFLAGS@
COMMON_FLAGS += @MIR_CFLAGS@
COMMON_FLAGS += @OPENAL_CFLAGS@
COMMON_FLAGS += @OPENGL_CFLAGS@
# COMMON_FLAGS += @OS_CFLAGS@
COMMON_FLAGS += @PNG_CFLAGS@
COMMON_FLAGS += @SDL2_CFLAGS@
# COMMON_FLAGS += @X11_CFLAGS@
COMMON_FLAGS += @ZLIB_CFLAGS@

AM_CFLAGS = -std=gnu99 $(COMMON_FLAGS)
AM_CXXFLAGS = $(COMMON_FLAGS)
AM_CPPFLAGS = -DHAVE_CONFIG_H @OS_CPPFLAGS@ -DFSUAE -DFSEMU

LDADD =
LIBS += @CARBON_LIBS@
LIBS += @FREETYPE_LIBS@
LIBS += @GLIB_LIBS@
LIBS += @IOKIT_LIBS@
LIBS += @LIBMPEG2_LIBS@
# LIBS += @MIR_LIBS@
LIBS += @OPENAL_LIBS@
LIBS += @OPENGL_LIBS@
# LIBS += @OS_LIBS@
LIBS += @PNG_LIBS@
LIBS += @SDL2_LIBS@
LIBS += @X11_LIBS@
LIBS += @ZLIB_LIBS@

AM_LDFLAGS = @OS_LDFLAGS@

fs_uae_SOURCES = \
	libfsemu/src/base.c\
	libfsemu/src/config.c\
	libfsemu/src/data.c\
	libfsemu/src/emu/actions.c\
	libfsemu/src/emu/audio_common.c\
	libfsemu/src/emu/audio_debug.c\
	libfsemu/src/emu/audio_dummy.c\
	libfsemu/src/emu/audio_dummy.c\
	libfsemu/src/emu/audio_openal.c\
	libfsemu/src/emu/audio_sdl.c\
	libfsemu/src/emu/dialog.c\
	libfsemu/src/emu/emu.c\
	libfsemu/src/emu/emu_lua.c\
	libfsemu/src/emu/font.c\
	libfsemu/src/emu/hud.c\
	libfsemu/src/emu/input.c\
	libfsemu/src/emu/menu.c\
	libfsemu/src/emu/netplay.c\
	libfsemu/src/emu/render.c\
	libfsemu/src/emu/scanlines.c\
	libfsemu/src/emu/texture.c\
	libfsemu/src/emu/theme.c\
	libfsemu/src/emu/util.c\
	libfsemu/src/emu/video_buffer.c\
	libfsemu/src/emu/video.c\
	libfsemu/src/emu/video_options.c\
	libfsemu/src/emu/xml_shader.c\
	libfsemu/src/eventlog.c\
	libfsemu/src/filesys.c\
	libfsemu/src/glee.c\
	libfsemu/src/hashtable.c\
	libfsemu/src/image.c\
	libfsemu/src/inifile.c\
	libfsemu/src/init.c\
	libfsemu/src/list.c\
	libfsemu/src/log.c\
	libfsemu/src/lua/lapi.c\
	libfsemu/src/lua/lauxlib.c\
	libfsemu/src/lua/lbaselib.c\
	libfsemu/src/lua/lbitlib.c\
	libfsemu/src/lua/lcode.c\
	libfsemu/src/lua/lcorolib.c\
	libfsemu/src/lua/lctype.c\
	libfsemu/src/lua/ldblib.c\
	libfsemu/src/lua/ldebug.c\
	libfsemu/src/lua/ldo.c\
	libfsemu/src/lua/ldump.c\
	libfsemu/src/lua/lfunc.c\
	libfsemu/src/lua/lgc.c\
	libfsemu/src/lua/linit.c\
	libfsemu/src/lua/liolib.c\
	libfsemu/src/lua/llex.c\
	libfsemu/src/lua/lmathlib.c\
	libfsemu/src/lua/lmem.c\
	libfsemu/src/lua/loadlib.c\
	libfsemu/src/lua/lobject.c\
	libfsemu/src/lua/lopcodes.c\
	libfsemu/src/lua/loslib.c\
	libfsemu/src/lua/lparser.c\
	libfsemu/src/lua/lstate.c\
	libfsemu/src/lua/lstring.c\
	libfsemu/src/lua/lstrlib.c\
	libfsemu/src/lua/ltable.c\
	libfsemu/src/lua/ltablib.c\
	libfsemu/src/lua/ltm.c\
	libfsemu/src/lua/lundump.c\
	libfsemu/src/lua/lvm.c\
	libfsemu/src/lua/lzio.c\
	libfsemu/src/ml/clock.c\
	libfsemu/src/ml/config.c\
	libfsemu/src/ml/input.c\
	libfsemu/src/ml/keyboard.c\
	libfsemu/src/ml/macosx.c\
	libfsemu/src/ml/manymouse/linux_evdev.c\
	libfsemu/src/ml/manymouse/macosx_hidmanager.c\
	libfsemu/src/ml/manymouse/macosx_hidutilities.c\
	libfsemu/src/ml/manymouse/manymouse.c\
	libfsemu/src/ml/manymouse/windows_wminput.c\
	libfsemu/src/ml/manymouse/x11_xinput2.c\
	libfsemu/src/ml/ml.c\
	libfsemu/src/ml/mouse.c\
	libfsemu/src/ml/opengl.c\
	libfsemu/src/ml/rawinput.c\
	libfsemu/src/ml/render.c\
	libfsemu/src/ml/sdl.c\
	libfsemu/src/ml/util.c\
	libfsemu/src/ml/video.c\
	libfsemu/src/ml/video.c\
	libfsemu/src/ml/video_mode.c\
	libfsemu/src/ml/video_sync.c\
	libfsemu/src/ml/windows.c\
	libfsemu/src/ml/x11.c\
	libfsemu/src/opengl.c\
	libfsemu/src/queue.c\
	libfsemu/src/random.c\
	libfsemu/src/ref.c\
	libfsemu/src/string.c\
	libfsemu/src/thread.c\
	libfsemu/src/time.c\
	libfsemu/src/unicode.c\
	src/a2091.cpp\
	src/akiko.cpp\
	src/amax.cpp\
	src/arcadia.cpp\
	src/archivers/dms/crc_csum.cpp\
	src/archivers/dms/getbits.cpp\
	src/archivers/dms/maketbl.cpp\
	src/archivers/dms/pfile.cpp\
	src/archivers/dms/tables.cpp\
	src/archivers/dms/u_deep.cpp\
	src/archivers/dms/u_heavy.cpp\
	src/archivers/dms/u_init.cpp\
	src/archivers/dms/u_medium.cpp\
	src/archivers/dms/u_quick.cpp\
	src/archivers/dms/u_rle.cpp\
	src/archivers/mp2/kjmp2.cpp\
	src/archivers/zip/unzip.cpp\
	src/ar.cpp\
	src/aros.rom.cpp\
	src/audio.cpp\
	src/autoconf.cpp\
	src/blitter.cpp\
	src/blkdev_cdimage.cpp\
	src/blkdev.cpp\
	src/bsdsocket.cpp\
	src/calc.cpp\
	src/cd32_fmv.cpp\
	src/cd32_fmv_genlock.cpp\
	src/cdrom.cpp\
	src/cdtv.cpp\
	src/cfgfile.cpp\
	src/cia.cpp\
	src/consolehook.cpp\
	src/cpummu30.cpp\
	src/cpummu.cpp\
	src/crc32.cpp\
	src/custom.cpp\
	src/debug.cpp\
	src/disk.cpp\
	src/diskutil.cpp\
	src/dongle.cpp\
	src/drawing.cpp\
	src/driveclick.cpp\
	src/enforcer.cpp\
	src/events.cpp\
	src/expansion.cpp\
	src/fdi2raw.cpp\
	src/filesys.cpp\
	src/fpp.cpp\
	src/fsdb.cpp\
	src/fs-uae/config.c\
	src/fs-uae/input.c\
	src/fs-uae/joystick.c\
	src/fs-uae/keyboard.c\
	src/fs-uae/luascript.c\
	src/fs-uae/main.c\
	src/fs-uae/menu.c\
	src/fs-uae/mouse.c\
	src/fs-uae/paths.c\
	src/fs-uae/plugins.c\
	src/fs-uae/recording.c\
	src/fs-uae/uae_config.c\
	src/fs-uae/video.c\
	src/fsusage.cpp\
	src/gayle.cpp\
	gen/blitfunc.cpp\
	gen/blittable.cpp\
	gen/compemu.cpp\
	gen/compstbl.cpp\
	gen/cpudefs.cpp\
	gen/cpuemu_0.cpp\
	gen/cpuemu_11.cpp\
	gen/cpuemu_13.cpp\
	gen/cpuemu_20.cpp\
	gen/cpuemu_21.cpp\
	gen/cpuemu_22.cpp\
	gen/cpuemu_31.cpp\
	gen/cpuemu_32.cpp\
	gen/cpuemu_33.cpp\
	gen/cpustbl.cpp\
	src/gfxboard.cpp\
	src/gfxutil.cpp\
	src/hardfile.cpp\
	src/hrtmon.rom.cpp\
	src/identify.cpp\
	src/inputdevice.cpp\
	src/isofs.cpp\
	src/jit/compemu_fpp.cpp\
	src/jit/compemu_support.cpp\
	src/keybuf.cpp\
	src/luascript.cpp\
	src/main.cpp\
	src/memory.cpp\
	src/missing.cpp\
	src/native2amiga.cpp\
	src/ncr_scsi.cpp\
	src/newcpu_common.cpp\
	src/newcpu.cpp\
	src/od-fs/ahidsound_new.cpp\
	src/od-fs/ahi_winuae.cpp\
	src/od-fs/audio.cpp\
	src/od-fs/blkdev-linux.cpp\
	src/od-fs/bsdsocket_host.cpp\
	src/od-fs/caps.cpp\
	src/od-fs/cda_play.cpp\
	src/od-fs/cdimage_stubs.cpp\
	src/od-fs/charset.cpp\
	src/od-fs/clipboard.cpp\
	src/od-fs/clock.cpp\
	src/od-fs/driveclick.cpp\
	src/od-fs/filesys_host.cpp\
	src/od-fs/fsdb_host.cpp\
	src/od-fs/gui.cpp\
	src/od-fs/hardfile_host.cpp\
	src/od-fs/input.cpp\
	src/od-fs/inputrecord.cpp\
	src/od-fs/keymap.cpp\
	src/od-fs/libamiga.cpp\
	src/od-fs/logging.cpp\
	src/od-fs/parser.cpp\
	src/od-fs/paths.cpp\
	src/od-fs/picasso96.cpp\
	src/od-fs/random.cpp\
	src/od-fs/roms.cpp\
	src/od-fs/serial_host.cpp\
	src/od-fs/stubs.cpp\
	src/od-fs/support.cpp\
	src/od-fs/threading.cpp\
	src/od-fs/uae_host.cpp\
	src/od-fs/uaemisc.cpp\
	src/od-fs/util.cpp\
	src/od-fs/version.cpp\
	src/od-fs/video.cpp\
	src/od-win32/mman.cpp\
	src/qemuvga/cirrus_vga.cpp\
	src/qemuvga/lsi53c895a.cpp\
	src/qemuvga/qemuuaeglue.cpp\
	src/qemuvga/vga.cpp\
	src/readcpu.cpp\
	src/rommgr.cpp\
	src/savestate.cpp\
	src/scp.cpp\
	src/scsi.cpp\
	src/scsiemul.cpp\
	src/scsitape.cpp\
	src/specialmonitors.cpp\
	src/statusline.cpp\
	src/traps.cpp\
	src/uaeexe.cpp\
	src/uaelib.cpp\
	src/uaenative.cpp\
	src/uaeresource.cpp\
	src/uaeserial.cpp\
	src/zfile_archive.cpp\
	src/zfile.cpp

fs_uae_device_helper_SOURCES = \
	libfsemu/src/base.c\
	libfsemu/src/data.c\
	libfsemu/src/filesys.c\
	libfsemu/src/hashtable.c\
	libfsemu/src/list.c\
	libfsemu/src/ml/manymouse/linux_evdev.c\
	libfsemu/src/ml/manymouse/macosx_hidmanager.c\
	libfsemu/src/ml/manymouse/macosx_hidutilities.c\
	libfsemu/src/ml/manymouse/manymouse.c\
	libfsemu/src/ml/manymouse/windows_wminput.c\
	libfsemu/src/ml/manymouse/x11_xinput2.c\
	libfsemu/src/ml/util.c\
	libfsemu/src/string.c\
	src/fs-uae/device-helper.c

gen_build68k_SOURCES = \
	src/build68k.cpp\
	src/writelog.cpp

gen_genblitter_SOURCES = \
	src/blitops.cpp\
	src/genblitter.cpp\
	src/writelog.cpp

gen_gencomp_SOURCES = \
	gen/cpudefs.cpp\
	src/jit/gencomp.cpp\
	src/missing.cpp\
	src/readcpu.cpp

gen_gencpu_SOURCES = \
	gen/cpudefs.cpp\
	src/gencpu.cpp\
	src/missing.cpp\
	src/readcpu.cpp

gen_genlinetoscr_SOURCES = \
	src/genlinetoscr.cpp
