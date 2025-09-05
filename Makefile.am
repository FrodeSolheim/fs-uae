# Makefile.am for FS-UAE

# LIBS="$PTHREAD_LIBS $LIBS"
# AM_CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
# CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
# CC="$PTHREAD_CC"
# CXX="$PTHREAD_CXX"

SUBDIRS =

WARNINGS =

CM_CFLAGS = -Werror=return-type

AM_CFLAGS = $(PTHREAD_CFLAGS)
AM_CPPFLAGS =
AM_CXXFLAGS = $(PTHREAD_CFLAGS) -std=gnu++17
AM_LDFLAGS =

# FIXME: DEBUG BUILDS ONLY
# CM_CFLAGS += -DDEBUG

CM_CFLAGS += @ALSA_CFLAGS@
CM_CFLAGS += @FLAC_CFLAGS@
CM_CFLAGS += @FREETYPE_CFLAGS@
CM_CFLAGS += @GLEW_CFLAGS@
CM_CFLAGS += @GLIB_CFLAGS@
CM_CFLAGS += @MIR_CFLAGS@
CM_CFLAGS += @MPG123_CFLAGS@
CM_CFLAGS += @OPENGL_CFLAGS@
# CM_CFLAGS += @OS_CFLAGS@
CM_CFLAGS += @PNG_CFLAGS@
CM_CFLAGS += @PYTHON3_CFLAGS@
CM_CFLAGS += @SDL3_CFLAGS@
CM_CFLAGS += @SDL3_IMAGE_CFLAGS@
CM_CFLAGS += @SDL3_TTF_CFLAGS@
CM_CFLAGS += @X11_CFLAGS@
CM_CFLAGS += @ZLIB_CFLAGS@
CM_CFLAGS += @ZSTD_CFLAGS@
CM_CFLAGS += $(WARNINGS)

if F_NO_STRICT_OVERFLOW
CM_CFLAGS += -fno-strict-overflow
endif

if F_NO_STRICT_ALIASING
CM_CFLAGS += -fno-strict-aliasing
endif

CM_CFLAGS += -fvisibility=hidden

# CM_CFLAGS += $PTHREAD_CFLAGS

AM_CFLAGS += $(CM_CFLAGS)
AM_CFLAGS += -std=gnu11

AM_CXXFLAGS += $(CM_CFLAGS)

# Pre-processor ---------------------------------------------------------------

AM_CPPFLAGS += @OS_CPPFLAGS@
AM_CPPFLAGS += -DUAE -DFSUAE -DFSEMU

# WinUAE/CHD code checks for PTR64 in a header
if IS_64BIT
AM_CPPFLAGS += -DPTR64
endif

if JIT
if FNO_PIE
AM_CPPFLAGS += -fno-pie
else
if FNO_PIE_CAP
AM_CPPFLAGS += -fno-PIE
endif
endif
if NO_PIE
AM_LDFLAGS += -no-pie
endif
# if WL_TDATA
# AM_LDFLAGS += -Wl,-Tdata=0x10000000
# endif
endif

# Warnings --------------------------------------------------------------------

AM_CFLAGS += -Wall
# AM_CFLAGS += -Werror=missing-declarations

AM_CXXFLAGS += -Wall
AM_CXXFLAGS += -Wno-unused-function
AM_CXXFLAGS += -Wno-unused-variable
AM_CXXFLAGS += -Wno-unused-but-set-variable
AM_CXXFLAGS += -Wno-sign-compare

# if W_CPP11_NARROWING
# AM_CXXFLAGS += -Wno-c++11-narrowing
# endif

# if W_FORMAT_OVERFLOW
# AM_CXXFLAGS += -Wno-format-overflow
# endif

# if W_NO_FORMAT_TRUNCATION
# if MACOS
# # For some reason clang on mac at least passes the configure test for
# # -Wno-format-truncation but annoyingly warns about unknown option. So
# # don't try to use this on macOS for now.
# else
# CM_CFLAGS += -Wno-format-truncation
# endif
# endif

# if W_ADDRESS
# CM_CFLAGS += -Waddress
# endif

# if W_MISSING_FORMAT_ATTRIBUTE
# CM_CFLAGS += -Wmissing-format-attribute
# endif

# if W_MISSING_DECLARATIONS
# CM_CFLAGS += -Wmissing-declarations
# endif

# if W_POINTER_ARITH
# CM_CFLAGS += -Wpointer-arith
# endif

# -----------------------------------------------------------------------------

fs_uae_LDADD =


# AM_LDFLAGS += -rdynamic
AM_LDFLAGS += @OS_LDFLAGS@

MSGFMT = @MSGFMT@
ZIP = @ZIP@

bin_PROGRAMS = fs-uae

libexec_PROGRAMS =

dist_doc_DATA =

noinst_PROGRAMS =
#noinst_PROGRAMS += \
#	gen/build68k \
#	gen/genblitter \
#	gen/gencomp \
#	gen/gencpu \
#	gen/genlinetoscr

noinst_LIBRARIES =

BUILT_SOURCES =
#BUILT_SOURCES += \
#	gen/linetoscr.cpp

nodist_fs_uae_SOURCES =

AM_CPPFLAGS += -I$(srcdir)/od-fs/fsapp
AM_CPPFLAGS += -I$(srcdir)/od-fs/fsgui
AM_CPPFLAGS += -I$(srcdir)/od-fs/fslib
AM_CPPFLAGS += -I$(srcdir)/od-fs/fsuae

EXTRA_DIST =

fs_uae_SOURCES = \
	od-fs/FS/FS.c \
	od-fs/FS/FS.h \
	od-fs/fsapp/fsapp-channel.c \
	od-fs/fsapp/fsapp-channel.h \
	od-fs/fsapp/fsapp-channel.h \
	od-fs/fsapp/fsapp-config.h \
	od-fs/fsapp/fsapp-events.c \
	od-fs/fsapp/fsapp-events.h \
	od-fs/fsapp/fsapp-instance.c \
	od-fs/fsapp/fsapp-instance.h \
	od-fs/fsapp/fsapp-main.c \
	od-fs/fsapp/fsapp-main.h \
	od-fs/fsapp/fsapp-mainloop.c \
	od-fs/fsapp/fsapp-mainloop.h \
	od-fs/fsapp/fsapp-pythonthread.c \
	od-fs/fsapp/fsapp-pythonthread.h \
	od-fs/fsapp/fsapp-surface.c \
	od-fs/fsapp/fsapp-surface.h \
	od-fs/fsapp/fsapp.h \
	od-fs/fsgui/fsgui-config.h \
	od-fs/fsgui/fsgui-font.c \
	od-fs/fsgui/fsgui-font.h \
	od-fs/fsgui/fsgui-image.c \
	od-fs/fsgui/fsgui-image.h \
	od-fs/fsgui/fsgui-window.c \
	od-fs/fsgui/fsgui-window.h \
	od-fs/fsgui/fsgui-windows.c \
	od-fs/fsgui/fsgui-windows.h \
	od-fs/fslib/fslib-config.h \
	od-fs/fslib/fslib-log.c \
	od-fs/fslib/fslib-log.h \
	od-fs/fslib/fslib-os.c \
	od-fs/fslib/fslib-os.h \
	od-fs/fslib/fslib-path.c \
	od-fs/fslib/fslib-path.h \
	od-fs/fslib/fslib-refable.c \
	od-fs/fslib/fslib-refable.h \
	od-fs/fslib/fslib-stream.c \
	od-fs/fslib/fslib-stream.h \
	od-fs/fsuae/fsuae-bridge.c \
	od-fs/fsuae/fsuae-bridge.h \
	od-fs/fsuae/fsuae-config.h \
	od-fs/fsuae/fsuae-constants.h \
	od-fs/fsuae/fsuae-events.c \
	od-fs/fsuae/fsuae-events.h \
	od-fs/fsuae/fsuae-extras.cpp \
	od-fs/fsuae/fsuae-extras.h \
	od-fs/fsuae/fsuae-hack.c \
	od-fs/fsuae/fsuae-hack.h \
	od-fs/fsuae/fsuae-init.c \
	od-fs/fsuae/fsuae-init.h \
	od-fs/fsuae/fsuae-main.c \
	od-fs/fsuae/fsuae-main.h \
	od-fs/fsuae/fsuae-mainthread.c \
	od-fs/fsuae/fsuae-mainthread.h \
	od-fs/fsuae/fsuae-messages.cpp \
	od-fs/fsuae/fsuae-messages.h \
	od-fs/fsuae/fsuae-path.c \
	od-fs/fsuae/fsuae-path.h \
	od-fs/fsuae/fsuae-plugins.c \
	od-fs/fsuae/fsuae-plugins.h \
	od-fs/fsuae/fsuae-renderer.c \
	od-fs/fsuae/fsuae-renderer.h \
	od-fs/fsuae/fsuae.h \
	od-fs/libuae/libuae-config.h \
	od-fs/libuae/libuae-main.cpp \
	od-fs/libuae/libuae-main.h \
	od-fs/libuae/libuae-python.cpp \
	od-fs/libuae/libuae-python.h

if WINDOWS
nodist_fs_uae_SOURCES += od-fs/windows/fs-uae.rc
.rc.o:
	cp $(srcdir)/od-fs/windows/fs-uae.ico $(builddir)/od-fs/windows/fs-uae.ico
	$(WINDRES) $< -o $@ -I$(srcdir)/od-fs/include
else
AM_CPPFLAGS += -I$(srcdir)/od-fs/non-windows
endif

# -----------------------------------------------------------------------------

noinst_LIBRARIES += libuae.a
AM_CPPFLAGS += -I$(srcdir)/include
AM_CPPFLAGS += -I$(srcdir)/jit
AM_CPPFLAGS += -I$(srcdir)/od-fs
AM_CPPFLAGS += -I$(srcdir)/od-fs/archivers
AM_CPPFLAGS += -I$(srcdir)/od-fs/include
# AM_CPPFLAGS += -I$(srcdir)/od-fs/flac/include
AM_CPPFLAGS += -I$(srcdir)/od-win32

if WITH_CHD
AM_CPPFLAGS += -I$(srcdir)/mame/src/osd
AM_CPPFLAGS += -I$(srcdir)/mame/src/lib/util

# FIXME: Only for libarchivers (lzma)
AM_CPPFLAGS += -D UTF8PROC_STATIC -D_7ZIP_ST -DZ7_ST

AM_CPPFLAGS += -I${srcdir}/mame/3rdparty
AM_CPPFLAGS += -I${srcdir}/mame/3rdparty/utf8proc
AM_CPPFLAGS += -I${srcdir}/mame/src/lib
AM_CPPFLAGS += -I${srcdir}/mame/src/lib/util
AM_CPPFLAGS += -I${srcdir}/mame/src/osd
endif

fs_uae_LDADD += libuae.a
# libuae_a_CXXFLAGS = $(CM_CFLAGS) -fpermissive
libuae_a_SOURCES = \
	a2065.cpp \
	a2091.cpp \
	akiko.cpp \
	amax.cpp \
	ar.cpp \
	arcadia.cpp \
	aros.rom.cpp \
	audio.cpp \
	autoconf.cpp \
	blit.h \
	blitfunc.cpp \
	blitfunc.h \
	blittable.cpp \
	blitter.cpp \
	blkdev_cdimage.cpp \
	blkdev.cpp \
	bsdsocket.cpp \
	calc.cpp \
	cd32_fmv_genlock.cpp \
	cd32_fmv.cpp \
	cdrom.cpp \
	cdtv.cpp \
	cdtvcr.cpp \
	cfgfile.cpp \
	cia.cpp \
	consolehook.cpp \
	cpuboard.cpp \
	cpudefs.cpp \
	cpummu.cpp \
	cpummu30.cpp \
	cpustbl.cpp \
	cputbl.h \
	crc32.cpp \
	custom.cpp \
	debug.cpp \
	debuginfo.cpp \
	debugmem.cpp \
	def_icons.cpp \
	devices.cpp \
	disasm.cpp \
	disk.cpp \
	diskutil.cpp \
	dlopen.cpp \
	dongle.cpp \
	drawing.cpp \
	driveclick.cpp \
	enforcer.cpp \
	ethernet.cpp \
	events.cpp \
	expansion.cpp \
	fdi2raw.cpp \
	filesys.cpp \
	flashrom.cpp \
	fpp_native.cpp \
	fpp_softfloat.cpp \
	fpp.cpp \
	framebufferboards.cpp \
	fsdb.cpp \
	fsusage.cpp \
	gayle.cpp \
	gfxboard.cpp \
	gfxutil.cpp \
	hardfile.cpp \
	hrtmon.rom.cpp \
	ide.cpp \
	idecontrollers.cpp \
	identify.cpp \
	include/a2065.h \
	include/a2091.h \
	include/akiko.h \
	include/amax.h \
	include/ar.h \
	include/arcadia.h \
	include/audio.h \
	include/autoconf.h \
	include/blitter.h \
	include/blkdev.h \
	include/bsdsocket.h \
	include/calc.h \
	include/catweasel.h \
	include/cd32_fmv.h \
	include/cdtv.h \
	include/cdtvcr.h \
	include/cia.h \
	include/clipboard.h \
	include/commpipe.h \
	include/consolehook.h \
	include/cpu_prefetch.h \
	include/cpuboard.h \
	include/cpummu.h \
	include/cpummu030.h \
	include/cputest.h \
	include/crc32.h \
	include/custom.h \
	include/debug.h \
	include/debugmem.h \
	include/devices.h \
	include/disasm.h \
	include/disk.h \
	include/diskutil.h \
	include/dongle.h \
	include/draco.h \
	include/drawing.h \
	include/driveclick.h \
	include/enforcer.h \
	include/epsonprinter.h \
	include/ersatz.h \
	include/ethernet.h \
	include/events.h \
	include/execio.h \
	include/execlib.h \
	include/fdi2raw.h \
	include/filesys.h \
	include/flashrom.h \
	include/fpp.h \
	include/framebufferboards.h \
	include/fsdb.h \
	include/fsusage.h \
	include/gayle.h \
	include/genblitter.h \
	include/gensound.h \
	include/gfxboard.h \
	include/gfxfilter.h \
	include/gui.h \
	include/ide.h \
	include/idecontrollers.h \
	include/identify.h \
	include/ini.h \
	include/inputdevice.h \
	include/inputrecord.h \
	include/isofs_api.h \
	include/isofs.h \
	include/keyboard_mcu.h \
	include/keyboard.h \
	include/keybuf.h \
	include/luascript.h \
	include/mackbd.h \
	include/memory.h \
	include/memory.h \
	include/midiemu.h \
	include/mmu_common.h \
	include/moduleripper.h \
	include/mos6502.h \
	include/native2amiga_api.h \
	include/native2amiga.h \
	include/ncr_scsi.h \
	include/ncr9x_scsi.h \
	include/newcpu.h \
	include/options.h \
	include/osemu.h \
	include/packed.h \
	include/parallel.h \
	include/pci_hw.h \
	include/pci.h \
	include/picasso96.h \
	include/readcpu.h \
	include/rommgr.h \
	include/rtc.h \
	include/rtgmodes.h \
	include/sampler.h \
	include/sana2.h \
	include/savestate.h \
	include/scp.h \
	include/scsi.h \
	include/scsidev.h \
	include/serial.h \
	include/sndboard.h \
	include/specialmonitors.h \
	include/statusline.h \
	include/sysdeps.h \
	include/tabletlibrary.h \
	include/tinyxml2.h \
	include/traps.h \
	include/uae.h \
	include/uae/ahi.h \
	include/uae/api.h \
	include/uae/asm.h \
	include/uae/assert.h \
	include/uae/atomic.h \
	include/uae/attributes.h \
	include/uae/byteswap.h \
	include/uae/cdrom.h \
	include/uae/compat/windows.h \
	include/uae/compat/windows2.h \
	include/uae/debuginfo.h \
	include/uae/declarations.h \
	include/uae/dlopen.h \
	include/uae/endian.h \
	include/uae/enum.h \
	include/uae/fs.h \
	include/uae/funcattr.h \
	include/uae/glib.h \
	include/uae/inline.h \
	include/uae/io.h \
	include/uae/jitconfig.h \
	include/uae/likely.h \
	include/uae/limits.h \
	include/uae/log.h \
	include/uae/maccess.h \
	include/uae/memory.h \
	include/uae/mman.h \
	include/uae/parser.h \
	include/uae/patch.h \
	include/uae/ppc.h \
	include/uae/qemu.h \
	include/uae/segtracker.h \
	include/uae/seh.h \
	include/uae/slirp.h \
	include/uae/socket.h \
	include/uae/string.h \
	include/uae/tchar.h \
	include/uae/time.h \
	include/uae/types.h \
	include/uae/vm.h \
	include/uaeexe.h \
	include/uaeipc.h \
	include/uaenative.h \
	include/uaeresource.h \
	include/uaeserial.h \
	include/uni_common.h \
	include/unpacked.h \
	include/videograb.h \
	include/x86.h \
	include/xwin.h \
	include/zarchive.h \
	include/zfile.h \
	ini.cpp \
	inputdevice.cpp \
	$(srcdir)/inputrecord.cpp \
	isofs.cpp \
	kbmcu/8048/co8048.cpp \
	kbmcu/8048/co8048.h \
	kbmcu/keyboard_mcu_6500_1.cpp \
	kbmcu/keyboard_mcu_6805.cpp \
	kbmcu/keyboard_mcu_d8039hlc.cpp \
	kbmcu/m6805/m68_internal.h \
	kbmcu/m6805/m68_ops.cpp \
	kbmcu/m6805/m68_optab_hc05.h \
	kbmcu/m6805/m68emu.cpp \
	kbmcu/m6805/m68emu.h \
	keybuf.cpp \
	logging.cpp \
	main.cpp \
	mame/a2410.cpp \
	mame/mameglue.h \
	mame/tm34010/tms34010.cpp \
	mame/tm34010/tms34010.h \
	memory.cpp \
	mos6502.cpp \
	native2amiga.cpp \
	ncr_scsi.cpp \
	ncr9x_scsi.cpp \
	newcpu_common.cpp \
	newcpu.cpp \
	od-fs/blkdev-linux.cpp \
	od-fs/bridge.cpp \
	od-fs/bsdsocket_posix.cpp \
	od-fs/callbacks.h \
	od-fs/cdimage_stubs.cpp \
	od-fs/charset.cpp \
	od-fs/clock.cpp \
	od-fs/defines.h \
	od-fs/driveclick.cpp \
	od-fs/filesys_host.cpp \
	od-fs/flac/include/FLAC/all.h \
	od-fs/flac/include/FLAC/assert.h \
	od-fs/flac/include/FLAC/callback.h \
	od-fs/flac/include/FLAC/export.h \
	od-fs/flac/include/FLAC/format.h \
	od-fs/flac/include/FLAC/metadata.h \
	od-fs/flac/include/FLAC/ordinals.h \
	od-fs/flac/include/FLAC/stream_decoder.h \
	od-fs/flac/include/FLAC/stream_encoder.h \
	od-fs/fsdb_host.cpp \
	od-fs/fsdb_host.h \
	od-fs/gui.cpp \
	od-fs/hardfile_host.cpp \
	od-fs/include/resource.h \
	od-fs/include/uae/uae_inputevents_def.h \
	od-fs/include/uae/uae.h \
	od-fs/input-fs.cpp \
	od-fs/inputrecord.cpp \
	od-fs/ioport.cpp \
	od-fs/ioport.h \
	od-fs/joystick-fs.cpp \
	od-fs/keyboard-fs.cpp \
	od-fs/libamiga.cpp \
	od-fs/libuae.cpp \
	od-fs/libuae.h \
	od-fs/machdep/arm/maccess.h \
	od-fs/machdep/generic/maccess.h \
	od-fs/machdep/m68k.h \
	od-fs/machdep/maccess.h \
	od-fs/machdep/machdep.h \
	od-fs/machdep/ppc/maccess.h \
	od-fs/machdep/rpt.h \
	od-fs/machdep/x86/maccess.h \
	od-fs/midi.cpp \
	od-fs/midi.h \
	od-fs/mouse-fs.cpp \
	od-fs/mp3decoder.cpp \
	od-fs/non-windows/tchar.h \
	od-fs/parser.cpp \
	od-fs/paths.cpp \
	od-fs/random.cpp \
	od-fs/roms.cpp \
	od-fs/sleep.h \
	od-fs/sound-fs.cpp \
	od-fs/spin.cpp \
	od-fs/statusline-fs.cpp \
	od-fs/stubs.cpp \
	od-fs/support.cpp \
	od-fs/sysconfig-fs.h \
	od-fs/target.h \
	od-fs/threaddep/thread.h \
	od-fs/threading-fs.cpp \
	od-fs/uae_host.cpp \
	od-fs/uae-fs.h \
	od-fs/uaemisc.cpp \
	od-fs/video-fs.cpp \
	od-fs/videograb-fs.cpp \
	od-fs/vpar.cpp \
	od-fs/vpar.h \
	od-fs/winuae_compat.h \
	od-fs/writelog-fs.cpp \
	od-win32/ahidsound_new.h \
	od-win32/ahidsound.h \
	od-win32/avioutput.h \
	od-win32/bsdsock.cpp \
	od-win32/caps/caps_win32.cpp \
	od-win32/caps/caps_win32.h \
	od-win32/caps/CapsAPI.h \
	od-win32/caps/Comtype.h \
	od-win32/cda_play.cpp \
	od-win32/cda_play.h \
	od-win32/clipboard_win32.cpp \
	od-win32/clipboard_win32.h \
	od-win32/darkmode.h \
	od-win32/debug_win32.h \
	od-win32/direct3d.h \
	od-win32/lcd.h \
	od-win32/midi.h \
	od-win32/mman.cpp \
	od-win32/mp3decoder.h \
	od-win32/parser.h \
	od-win32/picasso96_win.cpp \
	od-win32/picasso96_win.h \
	od-win32/registry.h \
	od-win32/render.h \
	od-win32/serial_win32.cpp \
	od-win32/sounddep/sound.h \
	od-win32/sysconfig.h \
	od-win32/win32_scaler.cpp \
	od-win32/win32_uaenet.cpp \
	od-win32/win32_uaenet.h \
	od-win32/win32.cpp \
	od-win32/win32.h \
	od-win32/win32gfx.h \
	od-win32/win32gui.cpp \
	od-win32/win32gui.h \
	patch.cpp \
	pci.cpp \
	ppc/ppc.cpp \
	qemuvga/cirrus_vga_rop.h \
	qemuvga/cirrus_vga_rop2.h \
	qemuvga/cirrus_vga_template.h \
	qemuvga/cirrus_vga.cpp \
	qemuvga/es1370.cpp \
	qemuvga/esp.cpp \
	qemuvga/lsi53c710.cpp \
	qemuvga/lsi53c895a.cpp \
	qemuvga/ne2000.cpp \
	qemuvga/ne2000.h \
	qemuvga/pixel_ops.h \
	qemuvga/qemu.cpp \
	qemuvga/qemuaudio.h \
	qemuvga/qemumemory.h \
	qemuvga/qemuuaeglue.cpp \
	qemuvga/qemuuaeglue.h \
	qemuvga/queue.h \
	qemuvga/scsi/esp.h \
	qemuvga/scsi/scsi.h \
	qemuvga/vga_int.h \
	qemuvga/vga_template.h \
	qemuvga/vga.cpp \
	qemuvga/vga.h \
	readcpu.cpp \
	rommgr.cpp \
	rtc.cpp \
	sampler.cpp \
	sana2.cpp \
	savestate.cpp \
	scp.cpp \
	scsi.cpp \
	scsiemul.cpp \
	scsitape.cpp \
	segtracker.cpp \
	slirp_uae.cpp \
	specialmonitors.cpp \
	statusline.cpp \
	support/socket.cpp \
	support/time.cpp \
	test_card.cpp \
	tinyxml2.cpp \
	traps.cpp \
	uaeexe.cpp \
	uaelib.cpp \
	uaenative.cpp \
	uaeresource.cpp \
	uaeserial.cpp \
	vm.cpp \
	x86.cpp \
	zfile_archive.cpp \
	zfile.cpp
EXTRA_DIST += \
	aks.def \
	filesys_bootrom.cpp \
	inputevents.def \
	linetoscr_aga_fast.cpp \
	linetoscr_aga_fm0_genlock.cpp \
	linetoscr_aga_fm0.cpp \
	linetoscr_aga_fm1_genlock.cpp \
	linetoscr_aga_fm1.cpp \
	linetoscr_aga_fm2_genlock.cpp \
	linetoscr_aga_fm2.cpp \
	linetoscr_aga_genlock_fast.cpp \
	linetoscr_common.cpp \
	linetoscr_ecs_fast.cpp \
	linetoscr_ecs_genlock_fast.cpp \
	linetoscr_ecs_shres.cpp \
	linetoscr_ocs_ecs_genlock.cpp \
	linetoscr_ocs_ecs_ntsc_genlock.cpp \
	linetoscr_ocs_ecs_ntsc.cpp \
	linetoscr_ocs_ecs.cpp \
	mame/tm34010/34010fld.c \
	mame/tm34010/34010gfx.c \
	mame/tm34010/34010ops.c \
	mame/tm34010/34010ops.h \
	mame/tm34010/34010tbl.c \
	od-fs/windows/fs-uae.rc.in \
	od-win32/sounddep/sound.cpp \
	od-win32/win32gfx.cpp \
	p96_blit.cpp \
	sinctable.cpp \
	table68k

#nodist__libs_libuae_a_SOURCES = \
#	gen/blit.h \
#	gen/blitfunc.cpp \
#	gen/blitfunc.h \
#	gen/blittable.cpp \
#	gen/compemu.cpp \
#	gen/compstbl.cpp \
#	gen/comptbl.h \
#	gen/cpudefs.cpp \
#	gen/cpustbl.cpp \
#	gen/cputbl.h
BUILT_SOURCES += $(nodist__libs_libuae_a_SOURCES)

# archivers -------------------------------------------------------------------

noinst_LIBRARIES += libarchivers.a
fs_uae_LDADD +=  libarchivers.a
libarchivers_a_SOURCES = \
	archivers/dms/cdata.h \
	archivers/dms/crc_csum.cpp \
	archivers/dms/crc_csum.h \
	archivers/dms/getbits.cpp \
	archivers/dms/getbits.h \
	archivers/dms/maketbl.cpp \
	archivers/dms/maketbl.h \
	archivers/dms/pfile.cpp \
	archivers/dms/pfile.h \
	archivers/dms/tables.cpp \
	archivers/dms/tables.h \
	archivers/dms/u_deep.cpp \
	archivers/dms/u_deep.h \
	archivers/dms/u_heavy.cpp \
	archivers/dms/u_heavy.h \
	archivers/dms/u_init.cpp \
	archivers/dms/u_init.h \
	archivers/dms/u_medium.cpp \
	archivers/dms/u_medium.h \
	archivers/dms/u_quick.cpp \
	archivers/dms/u_quick.h \
	archivers/dms/u_rle.cpp \
	archivers/dms/u_rle.h \
	archivers/lha/crcio.cpp \
	archivers/lha/dhuf.cpp \
	archivers/lha/header.cpp \
	archivers/lha/huf.cpp \
	archivers/lha/larc.cpp \
	archivers/lha/lha_macro.h \
	archivers/lha/lha.h \
	archivers/lha/lhamaketbl.cpp \
	archivers/lha/lharc.cpp \
	archivers/lha/shuf.cpp \
	archivers/lha/shuf.cpp \
	archivers/lha/slide.cpp \
	archivers/lha/uae_lha.cpp \
	archivers/lha/util.cpp \
	archivers/lzx/unlzx.cpp \
	archivers/mp2/kjmp2.cpp \
	archivers/mp2/kjmp2.h \
	archivers/wrp/warp.cpp \
	archivers/wrp/warp.h \
	archivers/zip/unzip.cpp \
	archivers/zip/unzip.h
if WITH_CHD
# libarchivers_a_CXXFLAGS = 
libarchivers_a_SOURCES += \
	od-fs/7z/7zAlloc.c \
	od-fs/7z/7zAlloc.h \
	od-fs/7z/7zArcIn.c \
	od-fs/7z/7zBuf2.c \
	od-fs/7z/7zBuf.c \
	od-fs/7z/7zBuf.h \
	od-fs/7z/7zCrc.c \
	od-fs/7z/7zCrc.h \
	od-fs/7z/7zCrcOpt.c \
	od-fs/7z/7zDec.c \
	od-fs/7z/7zFile.c \
	od-fs/7z/7zFile.h \
	od-fs/7z/7z.h \
	od-fs/7z/7zStream.c \
	od-fs/7z/7zTypes.h \
	od-fs/7z/7zVersion.h \
	od-fs/7z/Aes.c \
	od-fs/7z/Aes.h \
	od-fs/7z/AesOpt.c \
	od-fs/7z/Alloc.c \
	od-fs/7z/Alloc.h \
	od-fs/7z/Bcj2.c \
	od-fs/7z/Bcj2.h \
	od-fs/7z/Bra86.c \
	od-fs/7z/Bra.c \
	od-fs/7z/Bra.h \
	od-fs/7z/BraIA64.c \
	od-fs/7z/Compiler.h \
	od-fs/7z/CpuArch.c \
	od-fs/7z/CpuArch.h \
	od-fs/7z/Delta.c \
	od-fs/7z/Delta.h \
	od-fs/7z/LzFind.c \
	od-fs/7z/LzFind.h \
	od-fs/7z/LzHash.h \
	od-fs/7z/Lzma2Dec.c \
	od-fs/7z/Lzma2Dec.h \
	od-fs/7z/Lzma2Enc.c \
	od-fs/7z/Lzma2Enc.h \
	od-fs/7z/Lzma86Dec.c \
	od-fs/7z/Lzma86Enc.c \
	od-fs/7z/Lzma86.h \
	od-fs/7z/LzmaDec.c \
	od-fs/7z/LzmaDec.h \
	od-fs/7z/LzmaEnc.c \
	od-fs/7z/LzmaEnc.h \
	od-fs/7z/LzmaLib.c \
	od-fs/7z/LzmaLib.h \
	od-fs/7z/Ppmd7.c \
	od-fs/7z/Ppmd7Dec.c \
	od-fs/7z/Ppmd7Enc.c \
	od-fs/7z/Ppmd7.h \
	od-fs/7z/Ppmd.h \
	od-fs/7z/Precomp.h \
	od-fs/7z/RotateDefs.h \
	od-fs/7z/Sha256.c \
	od-fs/7z/Sha256.h \
	od-fs/7z/Sort.c \
	od-fs/7z/Sort.h \
	od-fs/7z/Xz.c \
	od-fs/7z/XzCrc64.c \
	od-fs/7z/XzCrc64.h \
	od-fs/7z/XzCrc64Opt.c \
	od-fs/7z/XzDec.c \
	od-fs/7z/XzEnc.c \
	od-fs/7z/XzEnc.h \
	od-fs/7z/Xz.h \
	od-fs/7z/XzIn.c \
	mame/3rdparty/utf8proc/utf8proc.c \
	mame/3rdparty/utf8proc/utf8proc.h \
	mame/src/lib/util/avhuff.cpp \
	mame/src/lib/util/avhuff.h \
	mame/src/lib/util/bitmap.cpp \
	mame/src/lib/util/bitmap.h \
	mame/src/lib/util/cdrom.cpp \
	mame/src/lib/util/cdrom.h \
	mame/src/lib/util/chd.cpp \
	mame/src/lib/util/chd.h \
	mame/src/lib/util/chdcd.cpp \
	mame/src/lib/util/chdcd.h \
	mame/src/lib/util/chdcodec.cpp \
	mame/src/lib/util/chdcodec.h \
	mame/src/lib/util/corealloc.cpp \
	mame/src/lib/util/corealloc.h \
	mame/src/lib/util/corefile.cpp \
	mame/src/lib/util/corefile.h \
	mame/src/osd/osdfile.h \
	mame/src/lib/util/corestr.cpp \
	mame/src/lib/util/corestr.h \
	mame/src/lib/util/coretmpl.h \
	mame/src/lib/util/eminline.h \
	mame/src/lib/util/eigcc.h \
	mame/src/lib/util/eigccarm.h \
	mame/src/lib/util/eigccx86.h \
	mame/src/lib/util/flac.cpp \
	mame/src/lib/util/flac.h \
	mame/src/lib/util/unicode.h \
	mame/src/lib/util/path.h \
	mame/src/lib/util/harddisk.cpp \
	mame/src/lib/util/harddisk.h \
	mame/src/lib/util/hashing.cpp \
	mame/src/lib/util/hashing.h \
	mame/src/lib/util/huffman.cpp \
	mame/src/lib/util/huffman.h \
	mame/src/lib/util/bitstream.h \
	mame/src/lib/util/md5.cpp \
	mame/src/lib/util/md5.h \
	mame/src/lib/util/palette.cpp \
	mame/src/lib/util/palette.h \
	mame/src/lib/util/strformat.cpp \
	mame/src/lib/util/unicode.cpp \
	mame/src/lib/util/vecstream.cpp \
	mame/src/lib/util/utilfwd.h \
	mame/src/lib/util/ioprocs.h \
	mame/src/lib/util/strformat.h \
	mame/src/lib/util/vecstream.h \
	mame/src/osd/modules/file/posixdir.cpp \
	mame/src/osd/modules/file/posixfile.cpp \
	mame/src/osd/modules/file/posixfile.h \
	mame/src/osd/modules/lib/osdlib_unix.cpp \
	mame/src/osd/osdlib.h \
	mame/src/osd/osdcomm.h \
	mame/src/osd/osdcore.cpp \
	mame/src/osd/osdcore.h \
	mame/src/osd/osdsync.cpp \
	mame/src/osd/osdsync.h \
	mame/src/osd/strconv.cpp \
	mame/src/osd/strconv.h

#	mame/3rdparty/lzma/C/7zAlloc.c \
#	mame/3rdparty/lzma/C/7zArcIn.c \
#	mame/3rdparty/lzma/C/7zBuf.c \
#	mame/3rdparty/lzma/C/7zBuf2.c \
#	mame/3rdparty/lzma/C/7zCrc.c \
#	mame/3rdparty/lzma/C/7zCrcOpt.c \
#	mame/3rdparty/lzma/C/7zDec.c \
#	mame/3rdparty/lzma/C/7zFile.c \
#	mame/3rdparty/lzma/C/7zStream.c \
#	mame/3rdparty/lzma/C/Aes.c \
#	mame/3rdparty/lzma/C/AesOpt.c \
#	mame/3rdparty/lzma/C/Alloc.c \
#	mame/3rdparty/lzma/C/Bcj2.c \
#	mame/3rdparty/lzma/C/Bra.c \
#	mame/3rdparty/lzma/C/Bra86.c \
#	mame/3rdparty/lzma/C/BraIA64.c \
#	mame/3rdparty/lzma/C/CpuArch.c \
#	mame/3rdparty/lzma/C/Delta.c \
#	mame/3rdparty/lzma/C/LzFind.c \
#	mame/3rdparty/lzma/C/Lzma2Dec.c \
#	mame/3rdparty/lzma/C/Lzma2Enc.c \
#	mame/3rdparty/lzma/C/Lzma86Dec.c \
#	mame/3rdparty/lzma/C/Lzma86Enc.c \
#	mame/3rdparty/lzma/C/LzmaDec.c \
#	mame/3rdparty/lzma/C/LzmaEnc.c \
#	mame/3rdparty/lzma/C/LzmaLib.c \
#	mame/3rdparty/lzma/C/Ppmd7.c \
#	mame/3rdparty/lzma/C/Ppmd7Dec.c \
#	mame/3rdparty/lzma/C/Ppmd7Enc.c \
#	mame/3rdparty/lzma/C/Sha256.c \
#	mame/3rdparty/lzma/C/Sort.c \
#	mame/3rdparty/lzma/C/Xz.c \
#	mame/3rdparty/lzma/C/XzCrc64.c \
#	mame/3rdparty/lzma/C/XzCrc64Opt.c \
#	mame/3rdparty/lzma/C/XzDec.c \
#	mame/3rdparty/lzma/C/XzEnc.c \
#	mame/3rdparty/lzma/C/XzIn.c \
#
#	od-fs/archivers/7z/7zAlloc.c \
#	od-fs/archivers/7z/7zArcIn.c \
#	od-fs/archivers/7z/7zBuf.c \
#	od-fs/archivers/7z/7zBuf2.c \
#	od-fs/archivers/7z/7zCrc.c \
#	od-fs/archivers/7z/7zCrcOpt.c \
#	od-fs/archivers/7z/7zDec.c \
#	od-fs/archivers/7z/7zFile.c \
#	od-fs/archivers/7z/7zStream.c \
#	od-fs/archivers/7z/Aes.c \
#	od-fs/archivers/7z/AesOpt.c \
#	od-fs/archivers/7z/Alloc.c \
#	od-fs/archivers/7z/Bcj2.c \
#	od-fs/archivers/7z/Bra.c \
#	od-fs/archivers/7z/Bra86.c \
#	od-fs/archivers/7z/BraIA64.c \
#	od-fs/archivers/7z/CpuArch.c \
#	od-fs/archivers/7z/Delta.c \
#	od-fs/archivers/7z/LzFind.c \
#	od-fs/archivers/7z/Lzma2Dec.c \
#	od-fs/archivers/7z/Lzma2Enc.c \
#	od-fs/archivers/7z/Lzma86Dec.c \
#	od-fs/archivers/7z/Lzma86Enc.c \
#	od-fs/archivers/7z/LzmaDec.c \
#	od-fs/archivers/7z/LzmaEnc.c \
#	od-fs/archivers/7z/LzmaLib.c \
#	od-fs/archivers/7z/Ppmd7.c \
#	od-fs/archivers/7z/Ppmd7Dec.c \
#	od-fs/archivers/7z/Ppmd7Enc.c \
#	od-fs/archivers/7z/Sha256.c \
#	od-fs/archivers/7z/Sort.c \
#	od-fs/archivers/7z/Xz.c \
#	od-fs/archivers/7z/XzCrc64.c \
#	od-fs/archivers/7z/XzCrc64Opt.c \
#	od-fs/archivers/7z/XzDec.c \
#	od-fs/archivers/7z/XzEnc.c \
#	od-fs/archivers/7z/XzIn.c \
# 
#	mame/src/osd/modules/file/posixptty.cpp \
#	mame/src/osd/modules/file/posixsocket.cpp \
#	mame/src/lib/util/osdcore.cpp \
#

#	od-fs/archivers/chd/avhuff.cpp \
#	od-fs/archivers/chd/bitmap.cpp \
#	od-fs/archivers/chd/cdrom.cpp \
#	od-fs/archivers/chd/chd.cpp \
#	od-fs/archivers/chd/chdcd.cpp \
#	od-fs/archivers/chd/chdcodec.cpp \
#	od-fs/archivers/chd/corealloc.cpp \
#	od-fs/archivers/chd/corefile.cpp \
#	od-fs/archivers/chd/corestr.cpp \
#	od-fs/archivers/chd/flac.cpp \
#	od-fs/archivers/chd/harddisk.cpp \
#	od-fs/archivers/chd/hashing.cpp \
#	od-fs/archivers/chd/huffman.cpp \
#	od-fs/archivers/chd/md5.cpp \
#	od-fs/archivers/chd/osdcore.cpp \
#	od-fs/archivers/chd/osdlib_unix.cpp \
#	od-fs/archivers/chd/osdsync.cpp \
#	od-fs/archivers/chd/palette.cpp \
#	od-fs/archivers/chd/posixdir.cpp \
#	od-fs/archivers/chd/posixfile.cpp \
#	od-fs/archivers/chd/posixptty.cpp \
#	od-fs/archivers/chd/posixsocket.cpp \
#	od-fs/archivers/chd/strconv.cpp \
#	od-fs/archivers/chd/strformat.cpp \
#	od-fs/archivers/chd/unicode.cpp \
#	od-fs/archivers/chd/vecstream.cpp \
#	od-fs/archivers/chd/utf8proc.c

#	archivers/chd/astring.cpp \
#	archivers/chd/astring.h \
#	archivers/chd/bitstream.h \
#	archivers/chd/chd.cpp \
#	archivers/chd/chd.h \
#	archivers/chd/chdcdrom.cpp \
#	archivers/chd/chdcdrom.h \
#	archivers/chd/chdcodec.cpp \
#	archivers/chd/chdcodec.h \
#	archivers/chd/chdglue.cpp \
#	archivers/chd/chdtypes.h \
#	archivers/chd/corealloc.cpp \
#	archivers/chd/corealloc.h \
#	archivers/chd/corefile.h \
#	archivers/chd/coretmpl.h \
#	archivers/chd/flac.cpp \
#	archivers/chd/flac.h \
#	archivers/chd/harddisk.cpp \
#	archivers/chd/harddisk.h \
#	archivers/chd/hashing.h \
#	archivers/chd/huffman.cpp \
#	archivers/chd/huffman.h \
#	archivers/chd/md5.h \
#	archivers/chd/osdcore.h \
#	archivers/chd/sha1.h
endif
EXTRA_DIST += \
	mame/3rdparty/utf8proc/utf8proc_data.c

# -----------------------------------------------------------------------------

noinst_LIBRARIES += libcpuemu.a
fs_uae_LDADD += libcpuemu.a
libcpuemu_a_SOURCES = \
	cpuemu_0.cpp \
	cpuemu_11.cpp \
	cpuemu_13.cpp \
	cpuemu_20.cpp \
	cpuemu_21.cpp \
	cpuemu_22.cpp \
	cpuemu_23.cpp \
	cpuemu_24.cpp \
	cpuemu_31.cpp \
	cpuemu_32.cpp \
	cpuemu_33.cpp \
	cpuemu_34.cpp \
	cpuemu_35.cpp \
	cpuemu_40.cpp \
	cpuemu_50.cpp
#nodist_src_libcpuemu_a_SOURCES = \
#	gen/cpuemu_0.cpp \
#	gen/cpuemu_11.cpp \
#	gen/cpuemu_13.cpp \
#	gen/cpuemu_20.cpp \
#	gen/cpuemu_21.cpp \
#	gen/cpuemu_22.cpp \
#	gen/cpuemu_23.cpp \
#	gen/cpuemu_24.cpp \
#	gen/cpuemu_31.cpp \
#	gen/cpuemu_32.cpp \
#	gen/cpuemu_33.cpp \
#	gen/cpuemu_34.cpp \
#	gen/cpuemu_35.cpp \
#	gen/cpuemu_40.cpp \
#	gen/cpuemu_50.cpp
#BUILT_SOURCES += $(nodist_src_libcpuemu_a_SOURCES)

# -----------------------------------------------------------------------------

if FLOPPYBRIDGE
libuae_a_SOURCES += \
	floppybridge/floppybridge_abstract.h \
	floppybridge/floppybridge_lib.cpp \
	floppybridge/floppybridge_lib.h
endif

# jit -------------------------------------------------------------------------

noinst_LIBRARIES += libjit.a
fs_uae_LDADD += libjit.a
libjit_a_SOURCES = \
	jit/codegen_udis86.h \
	jit/codegen_x86.h \
	jit/compemu_fpp.cpp \
	jit/compemu_midfunc_arm.h \
	jit/compemu_midfunc_x86.h \
	jit/compemu_support.cpp \
	jit/compemu.cpp \
	jit/compemu.h \
	jit/compstbl.cpp \
	jit/comptbl.h \
	jit/flags_x86.h \
	jit/memory-uae.h
EXTRA_DIST += \
	jit/codegen_x86.cpp \
	jit/compemu_midfunc_x86.cpp \
	jit/compemu_prefs.cpp \
	jit/exception_handler.cpp

# fsemu0 ----------------------------------------------------------------------

noinst_LIBRARIES += libfsemu0.a
AM_CPPFLAGS += -I$(srcdir)/od-fs/libfsemu/include
fs_uae_LDADD += libfsemu0.a
libfsemu0_a_SOURCES = \
	od-fs/libfsemu/include/fs/base.h \
	od-fs/libfsemu/include/fs/data.h \
	od-fs/libfsemu/include/fs/defines.h \
	od-fs/libfsemu/include/fs/endian.h \
	od-fs/libfsemu/include/fs/filesys.h \
	od-fs/libfsemu/include/fs/fs.h \
	od-fs/libfsemu/include/fs/glib.h \
	od-fs/libfsemu/include/fs/i18n.h \
	od-fs/libfsemu/include/fs/init.h \
	od-fs/libfsemu/include/fs/lazyness.h \
	od-fs/libfsemu/include/fs/main.h \
	od-fs/libfsemu/include/fs/thread.h \
	od-fs/libfsemu/include/fs/time.h \
	od-fs/libfsemu/include/fs/util.h \
	od-fs/libfsemu/src/filesys.c \
	od-fs/libfsemu/src/init.c \
	od-fs/libfsemu/src/thread.c \
	od-fs/libfsemu/src/time.c

# fsemu -----------------------------------------------------------------------

if FSEMU

noinst_LIBRARIES += libfsemu.a
AM_CPPFLAGS += -I$(srcdir)/od-fs/fsemu
fs_uae_LDADD += libfsemu.a
libfsemu_a_SOURCES = \
	od-fs/fsemu/fsemu-action.c \
	od-fs/fsemu/fsemu-action.h \
	od-fs/fsemu/fsemu-actions.h \
	od-fs/fsemu/fsemu-all.h \
	od-fs/fsemu/fsemu-alsaaudio.c \
	od-fs/fsemu/fsemu-alsaaudio.h \
	od-fs/fsemu/fsemu-application.c \
	od-fs/fsemu/fsemu-application.h \
	od-fs/fsemu/fsemu-audio.c \
	od-fs/fsemu/fsemu-audio.h \
	od-fs/fsemu/fsemu-audiobuffer.c \
	od-fs/fsemu/fsemu-audiobuffer.h \
	od-fs/fsemu/fsemu-axis.h \
	od-fs/fsemu/fsemu-background.c \
	od-fs/fsemu/fsemu-background.h \
	od-fs/fsemu/fsemu-button.h \
	od-fs/fsemu/fsemu-color.h \
	od-fs/fsemu/fsemu-common.c \
	od-fs/fsemu/fsemu-common.h \
	od-fs/fsemu/fsemu-compat.h \
	od-fs/fsemu/fsemu-config.c \
	od-fs/fsemu/fsemu-config.h \
	od-fs/fsemu/fsemu-constants.h \
	od-fs/fsemu/fsemu-control.c \
	od-fs/fsemu/fsemu-control.h \
	od-fs/fsemu/fsemu-controller.c \
	od-fs/fsemu/fsemu-controller.h \
	od-fs/fsemu/fsemu-data.c \
	od-fs/fsemu/fsemu-data.h \
	od-fs/fsemu/fsemu-emuthread.c \
	od-fs/fsemu/fsemu-emuthread.h \
	od-fs/fsemu/fsemu-events.c \
	od-fs/fsemu/fsemu-events.h \
	od-fs/fsemu/fsemu-fade.c \
	od-fs/fsemu/fsemu-fade.h \
	od-fs/fsemu/fsemu-font.c \
	od-fs/fsemu/fsemu-font.h \
	od-fs/fsemu/fsemu-fontcache.c \
	od-fs/fsemu/fsemu-fontcache.h \
	od-fs/fsemu/fsemu-frame.c \
	od-fs/fsemu/fsemu-frame.h \
	od-fs/fsemu/fsemu-frameinfo.c \
	od-fs/fsemu/fsemu-frameinfo.h \
	od-fs/fsemu/fsemu-gamemode.c \
	od-fs/fsemu/fsemu-gamemode.h \
	od-fs/fsemu/fsemu-glib.h \
	od-fs/fsemu/fsemu-glvideo.c \
	od-fs/fsemu/fsemu-glvideo.h \
	od-fs/fsemu/fsemu-gui.c \
	od-fs/fsemu/fsemu-gui.h \
	od-fs/fsemu/fsemu-helpbar.c \
	od-fs/fsemu/fsemu-helpbar.h \
	od-fs/fsemu/fsemu-helper.c \
	od-fs/fsemu/fsemu-helper.h \
	od-fs/fsemu/fsemu-hud.c \
	od-fs/fsemu/fsemu-hud.h \
	od-fs/fsemu/fsemu-image.c \
	od-fs/fsemu/fsemu-image.h \
	od-fs/fsemu/fsemu-inifile.c \
	od-fs/fsemu/fsemu-inifile.h \
	od-fs/fsemu/fsemu-input.c \
	od-fs/fsemu/fsemu-input.h \
	od-fs/fsemu/fsemu-inputdevice.c \
	od-fs/fsemu/fsemu-inputdevice.h \
	od-fs/fsemu/fsemu-inputmode.c \
	od-fs/fsemu/fsemu-inputmode.h \
	od-fs/fsemu/fsemu-inputport.c \
	od-fs/fsemu/fsemu-inputport.h \
	od-fs/fsemu/fsemu-internal.h \
	od-fs/fsemu/fsemu-key.h \
	od-fs/fsemu/fsemu-keyboard.c \
	od-fs/fsemu/fsemu-keyboard.h \
	od-fs/fsemu/fsemu-layer.h \
	od-fs/fsemu/fsemu-layout.c \
	od-fs/fsemu/fsemu-layout.h \
	od-fs/fsemu/fsemu-led.c \
	od-fs/fsemu/fsemu-led.h \
	od-fs/fsemu/fsemu-leds.c \
	od-fs/fsemu/fsemu-leds.h \
	od-fs/fsemu/fsemu-log.c \
	od-fs/fsemu/fsemu-log.h \
	od-fs/fsemu/fsemu-main.c \
	od-fs/fsemu/fsemu-main.h \
	od-fs/fsemu/fsemu-mainloop.c \
	od-fs/fsemu/fsemu-mainloop.h \
	od-fs/fsemu/fsemu-mainmenu.c \
	od-fs/fsemu/fsemu-mainmenu.h \
	od-fs/fsemu/fsemu-manymouse.c \
	od-fs/fsemu/fsemu-manymouse.h \
	od-fs/fsemu/fsemu-media.c \
	od-fs/fsemu/fsemu-media.h \
	od-fs/fsemu/fsemu-menu.c \
	od-fs/fsemu/fsemu-menu.h \
	od-fs/fsemu/fsemu-module.c \
	od-fs/fsemu/fsemu-module.h \
	od-fs/fsemu/fsemu-monitor.c \
	od-fs/fsemu/fsemu-monitor.h \
	od-fs/fsemu/fsemu-mouse.c \
	od-fs/fsemu/fsemu-mouse.h \
	od-fs/fsemu/fsemu-mutex.c \
	od-fs/fsemu/fsemu-mutex.h \
	od-fs/fsemu/fsemu-opengl.c \
	od-fs/fsemu/fsemu-opengl.h \
	od-fs/fsemu/fsemu-opengl.h \
	od-fs/fsemu/fsemu-option.c \
	od-fs/fsemu/fsemu-option.h \
	od-fs/fsemu/fsemu-options.h \
	od-fs/fsemu/fsemu-oskeyboard.c \
	od-fs/fsemu/fsemu-oskeyboard.c \
	od-fs/fsemu/fsemu-oskeyboard.h \
	od-fs/fsemu/fsemu-oskeyboard.h \
	od-fs/fsemu/fsemu-osmenu.c \
	od-fs/fsemu/fsemu-osmenu.h \
	od-fs/fsemu/fsemu-path.c \
	od-fs/fsemu/fsemu-path.h \
	od-fs/fsemu/fsemu-perfgui.c \
	od-fs/fsemu/fsemu-perfgui.h \
	od-fs/fsemu/fsemu-python.c \
	od-fs/fsemu/fsemu-python.h \
	od-fs/fsemu/fsemu-quit.c \
	od-fs/fsemu/fsemu-quit.h \
	od-fs/fsemu/fsemu-recording.c \
	od-fs/fsemu/fsemu-recording.h \
	od-fs/fsemu/fsemu-render.c \
	od-fs/fsemu/fsemu-render.h \
	od-fs/fsemu/fsemu-savestate.c \
	od-fs/fsemu/fsemu-savestate.h \
	od-fs/fsemu/fsemu-screenshot.c \
	od-fs/fsemu/fsemu-screenshot.h \
	od-fs/fsemu/fsemu-sdl.h \
	od-fs/fsemu/fsemu-sdlaudio.c \
	od-fs/fsemu/fsemu-sdlaudio.h \
	od-fs/fsemu/fsemu-sdlgamecontrollerdb.c \
	od-fs/fsemu/fsemu-sdlgamecontrollerdb.h \
	od-fs/fsemu/fsemu-sdlinput.c \
	od-fs/fsemu/fsemu-sdlinput.h \
	od-fs/fsemu/fsemu-sdlvideo.c \
	od-fs/fsemu/fsemu-sdlvideo.h \
	od-fs/fsemu/fsemu-sdlwindow.c \
	od-fs/fsemu/fsemu-sdlwindow.h \
	od-fs/fsemu/fsemu-semaphore.c \
	od-fs/fsemu/fsemu-semaphore.h \
	od-fs/fsemu/fsemu-shader.c \
	od-fs/fsemu/fsemu-shader.h \
	od-fs/fsemu/fsemu-startupinfo.c \
	od-fs/fsemu/fsemu-startupinfo.h \
	od-fs/fsemu/fsemu-stream.c \
	od-fs/fsemu/fsemu-stream.h \
	od-fs/fsemu/fsemu-theme.c \
	od-fs/fsemu/fsemu-theme.h \
	od-fs/fsemu/fsemu-thread.c \
	od-fs/fsemu/fsemu-thread.h \
	od-fs/fsemu/fsemu-time.c \
	od-fs/fsemu/fsemu-time.h \
	od-fs/fsemu/fsemu-titlebar.c \
	od-fs/fsemu/fsemu-titlebar.h \
	od-fs/fsemu/fsemu-types.h \
	od-fs/fsemu/fsemu-util.c \
	od-fs/fsemu/fsemu-util.h \
	od-fs/fsemu/fsemu-video.c \
	od-fs/fsemu/fsemu-video.h \
	od-fs/fsemu/fsemu-videothread.c \
	od-fs/fsemu/fsemu-videothread.h \
	od-fs/fsemu/fsemu-widget.c \
	od-fs/fsemu/fsemu-widget.h \
	od-fs/fsemu/fsemu-window.c \
	od-fs/fsemu/fsemu-window.h \
	od-fs/fsemu/fsemu-xmlshader.c \
	od-fs/fsemu/fsemu-xmlshader.h \
	od-fs/fsemu/fsemu.c \
	od-fs/fsemu/fsemu.h \
	od-fs/gamemode/lib/gamemode_client.h

#	od-fs/manymouse/linux_evdev.c \
#	od-fs/manymouse/macosx_hidmanager.c \
#	od-fs/manymouse/macosx_hidutilities.c \
#	od-fs/manymouse/manymouse.c \
#	od-fs/manymouse/manymouse.h \
#	od-fs/manymouse/windows_wminput.c \
#	od-fs/manymouse/x11_xinput2.c

fsemu_data_files =

fsuae_data_files =

# FIXME: EXTRA_DIST ?
libfsemu_a_SOURCES += $(fsemu_data_files)

#data/.fsemu: $(fsemu_data_files) ${fsuae_data_files}
#	mkdir -p $(builddir)/data/
#	cp $(srcdir)/fsemu/fonts/saira/* $(builddir)/data/
#	cp $(srcdir)/fsemu/icons/fontawesome/* $(builddir)/data/
#	cp $(srcdir)/fsemu/images/* $(builddir)/data/
#	cp $(srcdir)/src/fsuae/images/* $(builddir)/data/
#	touch data/.fsemu

fsemu-data: 
# data/.fsemu

else

fsemu-data:

endif

# glad ------------------------------------------------------------------------

if WITH_GLAD
noinst_LIBRARIES += libglad.a
AM_CPPFLAGS += -I$(srcdir)/od-fs/glad/include
fs_uae_LDADD += libglad.a
libglad_a_SOURCES = \
	od-fs/glad/include/glad/glad.h \
	od-fs/glad/include/KHR/khrplatform.h \
	od-fs/glad/src/glad.c
endif

# manymouse -------------------------------------------------------------------

if BUILTIN_MANYMOUSE
# noinst_LIBRARIES += libmanymouse.a
# AM_CPPFLAGS += -I$(srcdir)/manymouse
# fs_uae_LDADD += libmanymouse.a
# libmanymouse_a_SOURCES = \
# 	manymouse/linux_evdev.c \
# 	manymouse/macosx_hidmanager.c \
# 	manymouse/macosx_hidutilities.c \
# 	manymouse/manymouse.c \
# 	manymouse/manymouse.h \
# 	manymouse/windows_wminput.c \
# 	manymouse/x11_xinput2.c
endif

# pcem ------------------------------------------------------------------------

if WITH_X86
noinst_LIBRARIES += libpcem.a
fs_uae_LDADD += libpcem.a
libpcem_a_SOURCES = \
	pcem/386_common.cpp \
	pcem/386_common.h \
	pcem/386_dynarec.cpp \
	pcem/386_ops.h \
	pcem/386.cpp \
	pcem/8087.h \
	pcem/808x.cpp \
	pcem/codegen_backend_arm_defs.h \
	pcem/codegen_backend_arm.h \
	pcem/codegen_backend_arm64_defs.h \
	pcem/codegen_backend_arm64.h \
	pcem/codegen_backend_x86_defs.h \
	pcem/codegen_backend_x86-64_defs.h \
	pcem/codegen_backend_x86-64.h \
	pcem/codegen_backend_x86.h \
	pcem/codegen_backend.h \
	pcem/codegen.h \
	pcem/config.h \
	pcem/cpu.cpp \
	pcem/cpu.h \
	pcem/device.h \
	pcem/dma.cpp \
	pcem/dma.h \
	pcem/dosbox/dbopl.cpp \
	pcem/dosbox/dbopl.h \
	pcem/dosbox/nukedopl.cpp \
	pcem/dosbox/nukedopl.h \
	pcem/f82c710_upc.h \
	pcem/fdc.h \
	pcem/filters.h \
	pcem/i440bx.h \
	pcem/ibm.h \
	pcem/io.h \
	pcem/keyboard_at.cpp \
	pcem/keyboard_at.h \
	pcem/keyboard.cpp \
	pcem/keyboard.h \
	pcem/mca.h \
	pcem/mem.cpp \
	pcem/mem.h \
	pcem/model.h \
	pcem/mouse_ps2.cpp \
	pcem/mouse_ps2.h \
	pcem/mouse_serial.cpp \
	pcem/mouse_serial.h \
	pcem/mouse.h \
	pcem/nmi.h \
	pcem/nvr_tc8521.h \
	pcem/nvr.cpp \
	pcem/nvr.h \
	pcem/paths.h \
	pcem/pcemglue.cpp \
	pcem/pcemglue.h \
	pcem/pcemrtc.cpp \
	pcem/pci.h \
	pcem/pic.cpp \
	pcem/pic.h \
	pcem/pit.cpp \
	pcem/pit.h \
	pcem/plat-keyboard.h \
	pcem/plat-midi.h \
	pcem/plat-mouse.h \
	pcem/ps2_mca.h \
	pcem/rom.h \
	pcem/rtc.h \
	pcem/serial.cpp \
	pcem/serial.h \
	pcem/sound_cms.cpp \
	pcem/sound_cms.h \
	pcem/sound_dbopl.cpp \
	pcem/sound_dbopl.h \
	pcem/sound_emu8k.h \
	pcem/sound_mpu401_uart.cpp \
	pcem/sound_mpu401_uart.h \
	pcem/sound_opl.cpp \
	pcem/sound_opl.h \
	pcem/sound_sb_dsp.cpp \
	pcem/sound_sb_dsp.h \
	pcem/sound_sb.cpp \
	pcem/sound_sb.h \
	pcem/sound_speaker.cpp \
	pcem/sound_speaker.h \
	pcem/sound.h \
	pcem/t1000.h \
	pcem/t3100e.h \
	pcem/thread.h \
	pcem/timer.cpp \
	pcem/timer.h \
	pcem/vid_cl5429.cpp \
	pcem/vid_cl5429.h \
	pcem/vid_et4000.h \
	pcem/vid_et4000.h \
	pcem/vid_inmos.h \
	pcem/vid_ncr.h \
	pcem/vid_permedia2.cpp \
	pcem/vid_permedia2.h \
	pcem/vid_s3_virge.cpp \
	pcem/vid_s3_virge.h \
	pcem/vid_s3.cpp \
	pcem/vid_s3.h \
	pcem/vid_sdac_ramdac.cpp \
	pcem/vid_sdac_ramdac.h \
	pcem/vid_svga_render_remap.h \
	pcem/vid_svga_render.cpp \
	pcem/vid_svga_render.h \
	pcem/vid_svga.cpp \
	pcem/vid_svga.h \
	pcem/vid_unk_ramdac.h \
	pcem/vid_vga.h \
	pcem/vid_voodoo_banshee.h \
	pcem/video.h \
	pcem/x86_flags.h \
	pcem/x86_ops_3dnow.h \
	pcem/x86_ops_arith.h \
	pcem/x86_ops_atomic.h \
	pcem/x86_ops_bcd.h \
	pcem/x86_ops_bit.h \
	pcem/x86_ops_bitscan.h \
	pcem/x86_ops_call.h \
	pcem/x86_ops_cyrix.h \
	pcem/x86_ops_flag.h \
	pcem/x86_ops_fpu.h \
	pcem/x86_ops_inc_dec.h \
	pcem/x86_ops_int.h \
	pcem/x86_ops_io.h \
	pcem/x86_ops_jump.h \
	pcem/x86_ops_misc.h \
	pcem/x86_ops_mmx_arith.h \
	pcem/x86_ops_mmx_cmp.h \
	pcem/x86_ops_mmx_logic.h \
	pcem/x86_ops_mmx_mov.h \
	pcem/x86_ops_mmx_pack.h \
	pcem/x86_ops_mmx_shift.h \
	pcem/x86_ops_mmx.h \
	pcem/x86_ops_mov_ctrl.h \
	pcem/x86_ops_mov_seg.h \
	pcem/x86_ops_mov.h \
	pcem/x86_ops_movx.h \
	pcem/x86_ops_msr.h \
	pcem/x86_ops_mul.h \
	pcem/x86_ops_pmode.h \
	pcem/x86_ops_prefix.h \
	pcem/x86_ops_rep.h \
	pcem/x86_ops_ret.h \
	pcem/x86_ops_set.h \
	pcem/x86_ops_shift.h \
	pcem/x86_ops_stack.h \
	pcem/x86_ops_string.h \
	pcem/x86_ops_xchg.h \
	pcem/x86_ops.h \
	pcem/x86.h \
	pcem/x86seg.cpp \
	pcem/x87_ops_arith.h \
	pcem/x87_ops_loadstore.h \
	pcem/x87_ops_misc.h \
	pcem/x87_ops.h \
	pcem/x87_timings.cpp \
	pcem/x87_timings.h \
	pcem/x87.cpp \
	pcem/x87.h \
	pcem/xi8088.h
endif

# ppc -------------------------------------------------------------------------

if WITH_PPC
fs_uae_SOURCES += \
	ppc/ppcd.cpp \
	ppc/ppcd.h
endif

# -----------------------------------------------------------------------------

if PROWIZARD
noinst_LIBRARIES += libprowizard.a
AM_CPPFLAGS += -I$(srcdir)/prowizard/include
fs_uae_LDADD += libprowizard.a
fs_uae_SOURCES += moduleripper.cpp
libprowizard_a_CFLAGS = $(AM_CFLAGS)
libprowizard_a_CFLAGS += -Wno-unused-variable
libprowizard_a_CFLAGS += -Wno-unused-but-set-variable
libprowizard_a_SOURCES = \
	prowizard/include/extern.h \
	prowizard/include/globals.h \
	prowizard/include/ptktable.h \
	prowizard/include/tuning.h \
	prowizard/include/vars.h \
	prowizard/misc/misc.c \
	prowizard/misc/testbag.c \
	prowizard/prowiz.c \
	prowizard/rippers/AC1D_packer.c \
	prowizard/rippers/AMOS-MusicBank.c \
	prowizard/rippers/BP-Soundmonitor.c \
	prowizard/rippers/BSI-FutureComposer.c \
	prowizard/rippers/ChipTracker.c \
	prowizard/rippers/DigiBooster17.c \
	prowizard/rippers/DigitalIllusion.c \
	prowizard/rippers/EurekaPacker.c \
	prowizard/rippers/FastTracker2.c \
	prowizard/rippers/FC-M-Packer.c \
	prowizard/rippers/FuchsTracker.c \
	prowizard/rippers/FutureComposer13.c \
	prowizard/rippers/FutureComposer14.c \
	prowizard/rippers/FuzzacPacker.c \
	prowizard/rippers/GMC.c \
	prowizard/rippers/GnuPlayer.c \
	prowizard/rippers/GPMO.c \
	prowizard/rippers/HeatseekerMC10.c \
	prowizard/rippers/HornetPacker.c \
	prowizard/rippers/JamCracker.c \
	prowizard/rippers/KefrensSoundMachine.c \
	prowizard/rippers/MED-Octamed.c \
	prowizard/rippers/MOD-compatible.c \
	prowizard/rippers/ModuleProtector.c \
	prowizard/rippers/Mugician.c \
	prowizard/rippers/Newtron.c \
	prowizard/rippers/NewtronOld.c \
	prowizard/rippers/NoiseFromHeaven.c \
	prowizard/rippers/NoisePacker1.c \
	prowizard/rippers/NoisePacker2.c \
	prowizard/rippers/NoiseRunner.c \
	prowizard/rippers/NP3.c \
	prowizard/rippers/PhaPacker.c \
	prowizard/rippers/PolkaPacker.c \
	prowizard/rippers/PowerMusic.c \
	prowizard/rippers/Promizer01.c \
	prowizard/rippers/Promizer10c.c \
	prowizard/rippers/Promizer18a.c \
	prowizard/rippers/Promizer20.c \
	prowizard/rippers/Promizer40.c \
	prowizard/rippers/ProPacker10.c \
	prowizard/rippers/ProPacker21.c \
	prowizard/rippers/ProPacker30.c \
	prowizard/rippers/Prorunner10.c \
	prowizard/rippers/Prorunner20.c \
	prowizard/rippers/QuadraComposer.c \
	prowizard/rippers/SGT-Packer.c \
	prowizard/rippers/skizzo.c \
	prowizard/rippers/SkytPacker.c \
	prowizard/rippers/SoundFX.c \
	prowizard/rippers/SoundTracker.c \
	prowizard/rippers/SoundTracker26.c \
	prowizard/rippers/StartrekkerPack.c \
	prowizard/rippers/StimPacker.c \
	prowizard/rippers/StoneArtsPlayer.c \
	prowizard/rippers/TheDarkDemon.c \
	prowizard/rippers/ThePlayer22a.c \
	prowizard/rippers/ThePlayer30a.c \
	prowizard/rippers/ThePlayer40.c \
	prowizard/rippers/ThePlayer41a.c \
	prowizard/rippers/ThePlayer50a.c \
	prowizard/rippers/ThePlayer60a.c \
	prowizard/rippers/ThePlayer61a.c \
	prowizard/rippers/TitanicsPlayer.c \
	prowizard/rippers/TMK.c \
	prowizard/rippers/TrackerPacker1.c \
	prowizard/rippers/TrackerPacker2.c \
	prowizard/rippers/TrackerPacker3.c \
	prowizard/rippers/UnicTracker.c \
	prowizard/rippers/UnicTracker2.c \
	prowizard/rippers/WantonPacker.c \
	prowizard/rippers/XannPlayer.c \
	prowizard/rippers/ZenPacker.c
endif

# slirp -----------------------------------------------------------------------

if WITH_SLIRP
if WITH_BUILTIN_SLIRP
noinst_LIBRARIES += libslirp.a
fs_uae_LDADD += libslirp.a
fs_uae_LDADD += @SLIRP_LIBS@
libslirp_a_SOURCES = \
	od-fs/slirp/src/arp_table.c \
	od-fs/slirp/src/bootp.c \
	od-fs/slirp/src/bootp.h \
	od-fs/slirp/src/cksum.c \
	od-fs/slirp/src/debug.h \
	od-fs/slirp/src/dhcpv6.c \
	od-fs/slirp/src/dhcpv6.h \
	od-fs/slirp/src/dnssearch.c \
	od-fs/slirp/src/if.c \
	od-fs/slirp/src/if.h \
	od-fs/slirp/src/ip_icmp.c \
	od-fs/slirp/src/ip_icmp.h \
	od-fs/slirp/src/ip_input.c \
	od-fs/slirp/src/ip_output.c \
	od-fs/slirp/src/ip.h \
	od-fs/slirp/src/ip6_icmp.c \
	od-fs/slirp/src/ip6_icmp.h \
	od-fs/slirp/src/ip6_input.c \
	od-fs/slirp/src/ip6_output.c \
	od-fs/slirp/src/ip6.h \
	od-fs/slirp/src/libslirp-version.h \
	od-fs/slirp/src/libslirp.h \
	od-fs/slirp/src/main.h \
	od-fs/slirp/src/mbuf.c \
	od-fs/slirp/src/mbuf.h \
	od-fs/slirp/src/misc.c \
	od-fs/slirp/src/misc.h \
	od-fs/slirp/src/ncsi-pkt.h \
	od-fs/slirp/src/ncsi.c \
	od-fs/slirp/src/ndp_table.c \
	od-fs/slirp/src/sbuf.c \
	od-fs/slirp/src/sbuf.h \
	od-fs/slirp/src/slirp.c \
	od-fs/slirp/src/slirp.h \
	od-fs/slirp/src/socket.c \
	od-fs/slirp/src/socket.h \
	od-fs/slirp/src/state.c \
	od-fs/slirp/src/stream.c \
	od-fs/slirp/src/stream.h \
	od-fs/slirp/src/tcp_input.c \
	od-fs/slirp/src/tcp_output.c \
	od-fs/slirp/src/tcp_subr.c \
	od-fs/slirp/src/tcp_timer.c \
	od-fs/slirp/src/tcp_timer.h \
	od-fs/slirp/src/tcp_var.h \
	od-fs/slirp/src/tcp.h \
	od-fs/slirp/src/tcpip.h \
	od-fs/slirp/src/tftp.c \
	od-fs/slirp/src/tftp.h \
	od-fs/slirp/src/udp.c \
	od-fs/slirp/src/udp.h \
	od-fs/slirp/src/udp6.c \
	od-fs/slirp/src/util.c \
	od-fs/slirp/src/util.h \
	od-fs/slirp/src/version.c \
	od-fs/slirp/src/vmstate.c \
	od-fs/slirp/src/vmstate.h
endif
endif
EXTRA_DIST += \
	od-fs/slirp/CHANGELOG.md \
	od-fs/slirp/COPYRIGHT \
	od-fs/slirp/README.md

# -----------------------------------------------------------------------------

if WITH_SOFTFLOAT
noinst_LIBRARIES += libsoftfloat.a
fs_uae_LDADD += libsoftfloat.a
libsoftfloat_a_SOURCES = \
	softfloat/softfloat_decimal.cpp \
	softfloat/softfloat_decimal.cpp \
	softfloat/softfloat_fpsp_tables.h \
	softfloat/softfloat_fpsp.cpp \
	softfloat/softfloat_fpsp.cpp \
	softfloat/softfloat-macros.h \
	softfloat/softfloat-specialize.h \
	softfloat/softfloat.cpp \
	softfloat/softfloat.cpp \
	softfloat/softfloat.h
endif

# -----------------------------------------------------------------------------

if WITH_TOCCATA
fs_uae_SOURCES += \
	sndboard.cpp
endif

# -----------------------------------------------------------------------------

if WITH_UDIS86
noinst_LIBRARIES += libudis86.a
AM_CPPFLAGS += -I$(srcdir)/od-fs/libudis86/include
fs_uae_LDADD += libudis86.a
libudis86_a_SOURCES = \
	od-fs/libudis86/include/udis86.h \
	od-fs/libudis86/src/decode.c \
	od-fs/libudis86/src/decode.h \
	od-fs/libudis86/src/extern.h \
	od-fs/libudis86/src/itab.c \
	od-fs/libudis86/src/itab.h \
	od-fs/libudis86/src/syn-att.c \
	od-fs/libudis86/src/syn-intel.c \
	od-fs/libudis86/src/syn.c \
	od-fs/libudis86/src/syn.h \
	od-fs/libudis86/src/types.h \
	od-fs/libudis86/src/udint.h \
	od-fs/libudis86/src/udis86.c
endif

# --------------------------------------------------------------------------------------------------

fs_uae_LDADD += @ALSA_LIBS@
fs_uae_LDADD += @CARBON_LIBS@
fs_uae_LDADD += @FLAC_LIBS@
fs_uae_LDADD += @FREETYPE_LIBS@
fs_uae_LDADD += @GLEW_LIBS@
fs_uae_LDADD += @GLIB_LIBS@
fs_uae_LDADD += @IOKIT_LIBS@
fs_uae_LDADD += @MIDI_LIBS@
fs_uae_LDADD += @MPG123_LIBS@
fs_uae_LDADD += @OPENGL_LIBS@
fs_uae_LDADD += @PNG_LIBS@
fs_uae_LDADD += @PYTHON3_LIBS@
fs_uae_LDADD += @SDL3_IMAGE_LIBS@
fs_uae_LDADD += @SDL3_LIBS@
fs_uae_LDADD += @SDL3_TTF_LIBS@
fs_uae_LDADD += @X11_LIBS@
fs_uae_LDADD += @ZLIB_LIBS@
fs_uae_LDADD += @ZSTD_LIBS@

fs_uae_LDADD += $(PTHREAD_LIBS)

if WITH_LIBMPEG2
CM_CFLAGS += @LIBMPEG2_CFLAGS@
fs_uae_LDADD += @LIBMPEG2_LIBS@
endif

if WINDOWS
fs_uae_LDADD += -lwininet
endif

# fs_uae_LDADD += @MIR_LIBS@
# fs_uae_LDADD += @OS_LIBS@

# --------------------------------------------------------------------------------------------------

# if BUILTIN_LIBMPEG2
# SUBDIRS += slirp
# fs_uae_LDADD += slirp/libslirp.a
# endif

#noinst_LIBRARIES += libdummy.a
#nodist_libdummy_a_SOURCES = \
#	gen/cdummy.c \
#	gen/cppdummy.cpp
#BUILT_SOURCES += $(nodist_libdummy_a_SOURCES)
#
#gen/cdummy.c:
#	echo "int cdummy;" > $(builddir)/gen/cdummy.c
#
#gen/cppdummy.cpp:
#	echo "int cppdummy;" > $(builddir)/gen/cppdummy.cpp

#gen_build68k_SOURCES = \
#	src/build68k.cpp \
#	src/writelog.cpp

#gen_genblitter_SOURCES = \
#	src/blitops.cpp \
#	src/genblitter.cpp \
#	src/writelog.cpp

#gen_gencomp_SOURCES = \
#	src/jit/gencomp.cpp \
#	src/missing.cpp \
#	src/readcpu.cpp

#nodist_gen_gencomp_SOURCES = \
#	gen/cpudefs.cpp

#gen_gencpu_SOURCES = \
#	src/gencpu.cpp \
#	src/missing.cpp \
#	src/readcpu.cpp

#nodist_gen_gencpu_SOURCES = \
#	gen/cpudefs.cpp

#gen_genlinetoscr_SOURCES = \
#	src/genlinetoscr.cpp

#TESTS = \
#	tests/dummy-test

#EXTRA_TESTS = \
#	tests/cppcheck-fs-uae \
#	tests/cppcheck-libfsemu \
#	tests/cppcheck-libmpeg2 \
#	tests/cppcheck-manymouse \
#	tests/cppcheck-od-fs \
#	tests/cppcheck-pearpc \
#	tests/cppcheck-ppc \
#	tests/cppcheck-slirp \
#	tests/cppcheck-uae

#	licenses/fs-uae.txt \
#	licenses/glib.txt \
#	licenses/ipflib.txt \
#	licenses/libgcc.txt \
#	licenses/libintl.txt \
#	licenses/libpng.txt \
#	licenses/libstdc++.txt \
#	licenses/zlib.txt \
#	po

EXTRA_DIST += $(TESTS) $(EXTRA_TESTS) \
	$(fsuae_data_files) \
	docs/action-replay.md \
	docs/amiga-forever.md \
	docs/amigaos-4-1.md \
	docs/amikit.md \
	docs/bridgeboard.md \
	docs/compiling.md \
	docs/cyberstorm-ppc.md \
	docs/examples \
	docs/examples/Default.fs-uae \
	docs/examples/Keyboard.ini \
	docs/faq.md \
	docs/g-sync.md \
	docs/getting-started.md \
	docs/issues.md \
	docs/jit-compiler.md \
	docs/joysticks.md \
	docs/keyboard.md \
	docs/kickstarts.md \
	docs/linux.md \
	docs/macos.md \
	docs/mouse.md \
	docs/old-files.md \
	docs/portable.md \
	docs/serial-port.md \
	docs/todo.md \
	docs/variable-refresh-rate.md \
	docs/vpar.md \
	docs/vsync.md \
	docs/windows.md \
	docs/x-arcade.md \
	od-fs/ChangeLog \
	od-fs/gamemode/LICENSE.txt \
	od-fs/gamemode/README.md \
	od-fs/glad/LICENSE \
	od-fs/libinih/ini.c \
	od-fs/libinih/ini.h \
	od-fs/libinih/LICENSE.txt \
	od-fs/libudis86/README \
	od-fs/README.md \
	od-fs/windows/fs-uae.ico

#	share/applications \
#	share/fs-uae \
#	share/icons \
#	share/mime

CLEANFILES = $(BUILT_SOURCES)

if CODEGEN

#gen/blit.h: gen/genblitter$(EXEEXT)
#	$(builddir)/gen/genblitter$(EXEEXT) i > $(builddir)/gen/blit.h

#gen/blitfunc.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
#	$(builddir)/gen/genblitter$(EXEEXT) f > $(builddir)/gen/blitfunc.cpp

#gen/blitfunc.h: gen/genblitter$(EXEEXT)
#	$(builddir)/gen/genblitter$(EXEEXT) h > $(builddir)/gen/blitfunc.h

#gen/blittable.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
#	$(builddir)/gen/genblitter$(EXEEXT) t > $(builddir)/gen/blittable.cpp

#gen/compemu.cpp: gen/gencomp$(EXEEXT)
#	$(builddir)/gen/gencomp$(EXEEXT)

#gen/compstbl.cpp: gen/compemu.cpp

#gen/comptbl.h: gen/compemu.cpp

#gen/cpudefs.cpp: gen/build68k$(EXEEXT) src/table68k
#	$(builddir)/gen/build68k$(EXEEXT) < $(srcdir)/src/table68k > $(builddir)/gen/cpudefs.cpp

#gen/cpuemu_0.cpp: gen/gencpu$(EXEEXT)
#	cd $(builddir)/gen && ./gencpu$(EXEEXT) --optimized-flags

#gen/cpuemu_11.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_13.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_20.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_21.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_22.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_23.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_24.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_31.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_32.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_33.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_34.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_35.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_40.cpp: gen/cpuemu_0.cpp

#gen/cpuemu_50.cpp: gen/cpuemu_0.cpp

#gen/cpustbl.cpp: gen/cpuemu_0.cpp

#gen/cputbl.h: gen/cpuemu_0.cpp

#gen/linetoscr.cpp: gen/genlinetoscr$(EXEEXT)
#	$(builddir)/gen/genlinetoscr$(EXEEXT) > $(builddir)/gen/linetoscr.cpp

endif

#catalogs =
#
#share/locale/cs/LC_MESSAGES/fs-uae.mo: po/cs.po
#	mkdir -p share/locale/cs/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/cs.po -o $@
#
#share/locale/da/LC_MESSAGES/fs-uae.mo: po/da.po
#	mkdir -p share/locale/da/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/da.po -o $@
#
#share/locale/de/LC_MESSAGES/fs-uae.mo: po/de.po
#	mkdir -p share/locale/de/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/de.po -o $@
#
#share/locale/eo/LC_MESSAGES/fs-uae.mo: po/eo.po
#	mkdir -p share/locale/eo/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/eo.po -o $@
#
#share/locale/es/LC_MESSAGES/fs-uae.mo: po/es.po
#	mkdir -p share/locale/es/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/es.po -o $@
#
#share/locale/fi/LC_MESSAGES/fs-uae.mo: po/fi.po
#	mkdir -p share/locale/fi/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/fi.po -o $@
#
#share/locale/fr/LC_MESSAGES/fs-uae.mo: po/fr.po
#	mkdir -p share/locale/fr/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/fr.po -o $@
#
#share/locale/hu/LC_MESSAGES/fs-uae.mo: po/hu.po
#	mkdir -p share/locale/hu/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/hu.po -o $@
#
#share/locale/it/LC_MESSAGES/fs-uae.mo: po/it.po
#	mkdir -p share/locale/it/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/it.po -o $@
#
#share/locale/nb/LC_MESSAGES/fs-uae.mo: po/nb.po
#	mkdir -p share/locale/nb/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/nb.po -o $@
#
#share/locale/nl/LC_MESSAGES/fs-uae.mo: po/nl.po
#	mkdir -p share/locale/nl/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/nl.po -o $@
#
#share/locale/pl/LC_MESSAGES/fs-uae.mo: po/pl.po
#	mkdir -p share/locale/pl/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/pl.po -o $@
#
#share/locale/pt/LC_MESSAGES/fs-uae.mo: po/pt.po
#	mkdir -p share/locale/pt/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/pt.po -o $@
#
#share/locale/pt_BR/LC_MESSAGES/fs-uae.mo: po/pt_BR.po
#	mkdir -p share/locale/pt_BR/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/pt_BR.po -o $@
#
#share/locale/sr/LC_MESSAGES/fs-uae.mo: po/sr.po
#	mkdir -p share/locale/sr/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/sr.po -o $@
#
#share/locale/sv/LC_MESSAGES/fs-uae.mo: po/sv.po
#	mkdir -p share/locale/sv/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/sv.po -o $@
#
#share/locale/tr/LC_MESSAGES/fs-uae.mo: po/tr.po
#	mkdir -p share/locale/tr/LC_MESSAGES
#	$(MSGFMT) --verbose $(srcdir)/po/tr.po -o $@

#mo: $(catalogs)

#fs-uae.dat: $(catalogs)
#	rm -f fs-uae.dat
#	for d in $(srcdir) $(builddir) ; do \
#	  ( cd $$d && $(ZIP) -X $(abs_builddir)/fs-uae.dat $$(find share | LC_ALL=C sort ) ) ; \
#	done

#all-local: mo fs-uae.dat fsemu-data

#install-data-hook: mo fs-uae.dat
#	chmod -R u+w $(srcdir)/share  	# FIXME: Why?
#
#	mkdir -p $(DESTDIR)$(datadir)/applications
#	cp -R $(srcdir)/share/applications/* $(DESTDIR)$(datadir)/applications/
#	mkdir -p $(DESTDIR)$(datadir)/fs-uae
#	touch $(DESTDIR)$(datadir)/fs-uae/share-dir
#	cp fs-uae.dat $(DESTDIR)$(datadir)/fs-uae/
#	cp -R data/* $(DESTDIR)$(datadir)/fs-uae/
#	mkdir -p $(DESTDIR)$(datadir)/icons
#	cp -R $(srcdir)/share/icons/* $(DESTDIR)$(datadir)/icons/
#	mkdir -p $(DESTDIR)$(datadir)/locale
#	cp -R $(builddir)/share/locale/* $(DESTDIR)$(datadir)/locale/
#	mkdir -p $(DESTDIR)$(datadir)/mime
#	cp -R $(srcdir)/share/mime/* $(DESTDIR)$(datadir)/mime/

#uninstall-local:
#	rm -f $(DESTDIR)$(datadir)/applications/net.fs_uae.FS-UAE.desktop
#	rm -Rf $(DESTDIR)$(datadir)/fs-uae
#	find $(DESTDIR)$(datadir)/icons/hicolor -name net.fs_uae.FS-UAE.png -delete
#	find $(DESTDIR)$(datadir)/locale -name fs-uae.mo -delete
#	rm -f $(DESTDIR)$(datadir)/mime/packages/net.fs_uae.FS-UAE.xml

#clean-local:
#	rm -f fs-uae.dat
#	rm -f data/*.ttf
#	rm -f data/*.png
#	rm -f data/.fsemu
#	find share -name fs-uae.mo -delete

# --------------------------------------------------------------------------------------------------

if WINDOWS

noinst_PROGRAMS += proxy

proxy_SOURCES = od-fs/windows/proxy.c
proxy_CFLAGS = -DUNICODE -D_UNICODE
proxy_LDFLAGS = -mwindows -static
proxy_LDADD = -lshlwapi
nodist_proxy_SOURCES = od-fs/windows/fs-uae.rc

endif

# --------------------------------------------------------------------------------------------------

EXTRA_DIST += \
	od-fs/python/app/__init__.py \
	od-fs/python/app/app_init.py \
	od-fs/python/app/map_ports.py \
	od-fs/python/app/testwindow.py \
	od-fs/python/fsapp/__init__.py \
	od-fs/python/fsapp/_frame.py \
	od-fs/python/fsapp/constants.py \
	od-fs/python/fsapp/event.py \
	od-fs/python/fsapp/eventhandler.py \
	od-fs/python/fsapp/eventname.py \
	od-fs/python/fsapp/eventservice.py \
	od-fs/python/fsapp/main.py \
	od-fs/python/fsapp/signal.py \
	od-fs/python/fsapp/surface.py \
	od-fs/python/fsapp/tickservice.py \
	od-fs/python/fsemu/__init__.py \
	od-fs/python/fsemu/actions.py \
	od-fs/python/fsemu/constants.py \
	od-fs/python/fsemu/inputdevice.py \
	od-fs/python/fsemu/inputdeviceservice.py \
	od-fs/python/fsemu/inputport.py \
	od-fs/python/fsemu/inputportmode.py \
	od-fs/python/fsemu/inputportservice.py \
	od-fs/python/fsemu/signal.py \
	od-fs/python/fsgui/__init__.py \
	od-fs/python/fsgui/_frame.py \
	od-fs/python/fsgui/borderlesswindow.py \
	od-fs/python/fsgui/button.py \
	od-fs/python/fsgui/checkbox.py \
	od-fs/python/fsgui/choice.py \
	od-fs/python/fsgui/container.py \
	od-fs/python/fsgui/dataimage.py \
	od-fs/python/fsgui/decoratedwindow.py \
	od-fs/python/fsgui/drawingcontext.py \
	od-fs/python/fsgui/filedialog.py \
	od-fs/python/fsgui/font.py \
	od-fs/python/fsgui/fontmanager.py \
	od-fs/python/fsgui/heading.py \
	od-fs/python/fsgui/image.py \
	od-fs/python/fsgui/imagelabel.py \
	od-fs/python/fsgui/internal/__init__.py \
	od-fs/python/fsgui/internal/flags.py \
	od-fs/python/fsgui/internal/id.py \
	od-fs/python/fsgui/internal/surface.py \
	od-fs/python/fsgui/item.py \
	od-fs/python/fsgui/label.py \
	od-fs/python/fsgui/layout.py \
	od-fs/python/fsgui/lineheightwidget.py \
	od-fs/python/fsgui/listview.py \
	od-fs/python/fsgui/mainwindow.py \
	od-fs/python/fsgui/mainwindowborder.py \
	od-fs/python/fsgui/mainwindowtitlebar.py \
	od-fs/python/fsgui/parentstack.py \
	od-fs/python/fsgui/slider.py \
	od-fs/python/fsgui/spacer.py \
	od-fs/python/fsgui/specialwindow.py \
	od-fs/python/fsgui/style.py \
	od-fs/python/fsgui/test/__init__.py \
	od-fs/python/fsgui/test/widgettestwindow.py \
	od-fs/python/fsgui/textfield.py \
	od-fs/python/fsgui/transparentwindow.py \
	od-fs/python/fsgui/types.py \
	od-fs/python/fsgui/valuewidget.py \
	od-fs/python/fsgui/widget.py \
	od-fs/python/fsgui/widgetorlayout.py \
	od-fs/python/fsgui/widgetpainter.py \
	od-fs/python/fsgui/window.py \
	od-fs/python/fsgui/windowmanager.py \
	od-fs/python/fsgui/windowtitlebar.py \
	od-fs/python/fsuae/__init__.py \
	od-fs/python/fsuae/aboutwindow.py \
	od-fs/python/fsuae/actions.py \
	od-fs/python/fsuae/advchipsetwindow.py \
	od-fs/python/fsuae/amigacontext.py \
	od-fs/python/fsuae/chipsetwindow.py \
	od-fs/python/fsuae/config/__init__.py \
	od-fs/python/fsuae/cpuwindow.py \
	od-fs/python/fsuae/displaywindow.py \
	od-fs/python/fsuae/emulatorwindow.py \
	od-fs/python/fsuae/f12window.py \
	od-fs/python/fsuae/floppycontrolwindow.py \
	od-fs/python/fsuae/fsuaemainwindow.py \
	od-fs/python/fsuae/gui/notificationservice.py \
	od-fs/python/fsuae/gui/notificationtype.py \
	od-fs/python/fsuae/init.py \
	od-fs/python/fsuae/input/__init__.py \
	od-fs/python/fsuae/input/devicerobotservice.py \
	od-fs/python/fsuae/input/inputcontrolwindow.py \
	od-fs/python/fsuae/input/inputdeviceselector.py \
	od-fs/python/fsuae/input/inputdevicewindow.py \
	od-fs/python/fsuae/input/inputmodeselector.py \
	od-fs/python/fsuae/message.py \
	od-fs/python/fsuae/message2.py \
	od-fs/python/fsuae/messages.py \
	od-fs/python/fsuae/misc/__init__.py \
	od-fs/python/fsuae/misc/clockapp.py \
	od-fs/python/fsuae/quickstartwindow.py \
	od-fs/python/fsuae/reactive.py \
	od-fs/python/fsuae/reactive2.py \
	od-fs/python/fsuae/roms/__init__.py \
	od-fs/python/fsuae/roms/rom.py \
	od-fs/python/fsuae/roms/rommanagerwindow.py \
	od-fs/python/fsuae/roms/romscanner.py \
	od-fs/python/fsuae/roms/romservice.py \
	od-fs/python/fsuae/servicecontainer.py \
	od-fs/python/fsuae/services/__init__.py \
	od-fs/python/fsuae/services/amigaforeverservice.py \
	od-fs/python/fsuae/services/pathservice.py \
	od-fs/python/fsuae/services/windowsservice.py \
	od-fs/python/fsuae/shortcutswindow.py \
	od-fs/python/fsuae/svgicon.py \
	od-fs/python/fsuae/uaeconfig.py \
	od-fs/python/fsuae/uaeconfig2.py \
	od-fs/python/fsuae/uaeconfigservice.py \
	od-fs/python/fsuae/uaeconfigwidgets.py \
	od-fs/python/fsuae/var.py \
	od-fs/python/fsuae/vfs/__init__.py \
	od-fs/python/fsuae/vfs/vfsicon.py \
	od-fs/python/fsuae/vfs/vfsservice.py \
	od-fs/python/fsuae/workspace/__init__.py \
	od-fs/python/fsuae/workspace/fsuaedirectorywindow.py \
	od-fs/python/fsuae/workspace/shellwindow.py \
	od-fs/python/fsuae/workspace/systemicon.py \
	od-fs/python/fsuae/workspace/systemwindow.py \
	od-fs/python/sdl3/__doc__.py \
	od-fs/python/sdl3/__init__.py \
	od-fs/python/sdl3/SDL_assert.py \
	od-fs/python/sdl3/SDL_asyncio.py \
	od-fs/python/sdl3/SDL_atomic.py \
	od-fs/python/sdl3/SDL_audio.py \
	od-fs/python/sdl3/SDL_bits.py \
	od-fs/python/sdl3/SDL_blendmode.py \
	od-fs/python/sdl3/SDL_camera.py \
	od-fs/python/sdl3/SDL_clipboard.py \
	od-fs/python/sdl3/SDL_cpuinfo.py \
	od-fs/python/sdl3/SDL_dialog.py \
	od-fs/python/sdl3/SDL_endian.py \
	od-fs/python/sdl3/SDL_error.py \
	od-fs/python/sdl3/SDL_events.py \
	od-fs/python/sdl3/SDL_filesystem.py \
	od-fs/python/sdl3/SDL_gamepad.py \
	od-fs/python/sdl3/SDL_gpu.py \
	od-fs/python/sdl3/SDL_guid.py \
	od-fs/python/sdl3/SDL_haptic.py \
	od-fs/python/sdl3/SDL_hidapi.py \
	od-fs/python/sdl3/SDL_hints.py \
	od-fs/python/sdl3/SDL_image.py \
	od-fs/python/sdl3/SDL_init.py \
	od-fs/python/sdl3/SDL_iostream.py \
	od-fs/python/sdl3/SDL_joystick.py \
	od-fs/python/sdl3/SDL_keyboard.py \
	od-fs/python/sdl3/SDL_keycode.py \
	od-fs/python/sdl3/SDL_loadso.py \
	od-fs/python/sdl3/SDL_locale.py \
	od-fs/python/sdl3/SDL_log.py \
	od-fs/python/sdl3/SDL_main_impl.py \
	od-fs/python/sdl3/SDL_main.py \
	od-fs/python/sdl3/SDL_messagebox.py \
	od-fs/python/sdl3/SDL_metal.py \
	od-fs/python/sdl3/SDL_misc.py \
	od-fs/python/sdl3/SDL_mixer.py \
	od-fs/python/sdl3/SDL_mouse.py \
	od-fs/python/sdl3/SDL_mutex.py \
	od-fs/python/sdl3/SDL_net.py \
	od-fs/python/sdl3/SDL_pen.py \
	od-fs/python/sdl3/SDL_pixels.py \
	od-fs/python/sdl3/SDL_platform.py \
	od-fs/python/sdl3/SDL_power.py \
	od-fs/python/sdl3/SDL_process.py \
	od-fs/python/sdl3/SDL_properties.py \
	od-fs/python/sdl3/SDL_rect.py \
	od-fs/python/sdl3/SDL_render.py \
	od-fs/python/sdl3/SDL_rtf.py \
	od-fs/python/sdl3/SDL_scancode.py \
	od-fs/python/sdl3/SDL_sensor.py \
	od-fs/python/sdl3/SDL_shadercross.py \
	od-fs/python/sdl3/SDL_stdinc.py \
	od-fs/python/sdl3/SDL_storage.py \
	od-fs/python/sdl3/SDL_surface.py \
	od-fs/python/sdl3/SDL_system.py \
	od-fs/python/sdl3/SDL_textengine.py \
	od-fs/python/sdl3/SDL_thread.py \
	od-fs/python/sdl3/SDL_time.py \
	od-fs/python/sdl3/SDL_timer.py \
	od-fs/python/sdl3/SDL_touch.py \
	od-fs/python/sdl3/SDL_tray.py \
	od-fs/python/sdl3/SDL_ttf.py \
	od-fs/python/sdl3/SDL_version.py \
	od-fs/python/sdl3/SDL_video.py \
	od-fs/python/sdl3/SDL_vulkan.py \
	od-fs/python/sdl3/SDL.py \
	od-fs/python/uae/__init__.py \
	od-fs/python/uae/config/__init__.py \
	od-fs/python/uae/configkey.py \
	od-fs/python/uae/configurations/a1000.py \
	od-fs/python/uae/configurations/a1200.py \
	od-fs/python/uae/configurations/a3000.py \
	od-fs/python/uae/configurations/a4000.py \
	od-fs/python/uae/configurations/a4000t.py \
	od-fs/python/uae/configurations/a500.py \
	od-fs/python/uae/configurations/a500p.py \
	od-fs/python/uae/configurations/a600.py \
	od-fs/python/uae/configurations/cd32.py \
	od-fs/python/uae/configurations/cdtv.py \
	od-fs/python/uae/configurations/default.py \
	od-fs/python/uae/inputevent.py \
	od-fs/python/uae/options/__init__.py \
	od-fs/python/uae/options/blacklisted.py \
	od-fs/python/uae/options/known.py \
	od-fs/python/uae/options/ordered.py \
	od-fs/python/uae/xconfig.py

# --------------------------------------------------------------------------------------------------

EXTRA_DIST += \
	od-fs/resources/Fonts/other/AdwaitaSans-Regular.ttf \
	od-fs/resources/Fonts/other/Roboto-Medium.ttf \
	od-fs/resources/Fonts/other/SairaCondensed-Bold.ttf \
	od-fs/resources/Fonts/other/SairaCondensed-Medium.ttf \
	od-fs/resources/Fonts/other/SairaCondensed-Regular.ttf \
	od-fs/resources/Fonts/other/SairaCondensed-SemiBold.ttf \
	od-fs/resources/Fonts/Roboto-Bold.ttf \
	od-fs/resources/Fonts/Roboto-Regular.ttf \
	od-fs/resources/Fonts/Roboto-SemiBold.ttf \
	od-fs/resources/Images/widgets/button_disabled.9p.png \
	od-fs/resources/Images/widgets/button-pressed.9p.png \
	od-fs/resources/Images/widgets/button.9p.png \
	od-fs/resources/Images/widgets/choice-arrow-disabled.png \
	od-fs/resources/Images/widgets/choice-arrow.png \
	od-fs/resources/Images/window/close.png \
	od-fs/resources/Images/window/maximize.png \
	od-fs/resources/Images/window/menu.png \
	od-fs/resources/Images/window/minimize.png

# --------------------------------------------------------------------------------------------------

#EXTRA_DIST += \
#	debian/changelog \
#	debian/compat \
#	debian/control \
#	debian/copyright \
#	debian/rules \
#	debian/source \
#	debian/source/format

# --------------------------------------------------------------------------------------------------

install-data-local:
#	mkdir -p $(DESTDIR)$(datadir)/applications

	mkdir -p $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@

	mkdir -p $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/python
	cp -r $(srcdir)/od-fs/python/* $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/python
#	python3 -m compileall $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/python
	$(PYTHON) -m compileall $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/python
#	FIXME: compileall

	mkdir -p $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/resources

	mkdir -p $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/resources/Fonts
	cp -r $(srcdir)/od-fs/resources/Fonts/* $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/resources/Fonts

	mkdir -p $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/resources/Images
	cp -r $(srcdir)/od-fs/resources/Images/* $(DESTDIR)$(datadir)/@SHARE_DIR_NAME@/resources/Images

# --------------------------------------------------------------------------------------------------

.PHONY: fsemu-data
