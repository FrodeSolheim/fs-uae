# Makefile.am for FS-UAE

SUBDIRS =

WARNINGS =

CM_CFLAGS =

# FIXME: DEBUG BUILDS ONLY
# CM_CFLAGS += -DDEBUG

CM_CFLAGS += @ALSA_CFLAGS@
CM_CFLAGS += @FREETYPE_CFLAGS@
CM_CFLAGS += @GLEW_CFLAGS@
CM_CFLAGS += @GLIB_CFLAGS@
# CM_CFLAGS += @GTK3_CFLAGS@
CM_CFLAGS += @MIR_CFLAGS@
# CM_CFLAGS += @OPENAL_CFLAGS@
CM_CFLAGS += @OPENGL_CFLAGS@
# CM_CFLAGS += @OS_CFLAGS@
CM_CFLAGS += @PNG_CFLAGS@
CM_CFLAGS += @PYTHON3_CFLAGS@
CM_CFLAGS += @QT_CFLAGS@
CM_CFLAGS += @SDL3_CFLAGS@
CM_CFLAGS += @SDL3_IMAGE_CFLAGS@
CM_CFLAGS += @SDL3_TTF_CFLAGS@
CM_CFLAGS += @X11_CFLAGS@
CM_CFLAGS += @ZLIB_CFLAGS@
CM_CFLAGS += $(WARNINGS)

if F_NO_STRICT_OVERFLOW
CM_CFLAGS += -fno-strict-overflow
endif

if F_NO_STRICT_ALIASING
CM_CFLAGS += -fno-strict-aliasing
endif

if W_NO_FORMAT_TRUNCATION
if MACOS
# For some reason clang on mac at least passes the configure test for
# -Wno-format-truncation but annoyingly warns about unknown option. So
# don't try to use this on macOS for now.
else
CM_CFLAGS += -Wno-format-truncation
endif
endif

if W_ADDRESS
CM_CFLAGS += -Waddress
endif

#if W_MISSING_FORMAT_ATTRIBUTE
#CM_CFLAGS += -Wmissing-format-attribute
#endif

# if W_MISSING_DECLARATIONS
# CM_CFLAGS += -Wmissing-declarations
# endif

if W_ERROR_MISSING_DECLARATIONS
# FIXME: Re-add for C code!
# CM_CFLAGS += -Werror=missing-declarations
endif

if W_POINTER_ARITH
CM_CFLAGS += -Wpointer-arith
endif

CM_CFLAGS += -fvisibility=hidden

AM_CFLAGS =
AM_CFLAGS += $(CM_CFLAGS) -std=gnu11

AM_CXXFLAGS =
AM_CXXFLAGS += $(CM_CFLAGS)
AM_CXXFLAGS += -Wno-c++11-narrowing

AM_CPPFLAGS =
AM_CPPFLAGS += @OS_CPPFLAGS@
AM_CPPFLAGS += -DUAE -DFSUAE -DFSEMU

LIBS += @ALSA_LIBS@
LIBS += @CARBON_LIBS@
LIBS += @FREETYPE_LIBS@
LIBS += @GLEW_LIBS@
LIBS += @GLIB_LIBS@
# LIBS += @GTK3_LIBS@
LIBS += @IOKIT_LIBS@
# LIBS += @MIR_LIBS@
# LIBS += @OPENAL_LIBS@
LIBS += @OPENGL_LIBS@
# LIBS += @OS_LIBS@
LIBS += @PNG_LIBS@
LIBS += @QT_LIBS@
LIBS += @MIDI_LIBS@
LIBS += @PYTHON3_LIBS@
LIBS += @SDL3_LIBS@
LIBS += @SDL3_IMAGE_LIBS@
LIBS += @SDL3_TTF_LIBS@
LIBS += @X11_LIBS@
LIBS += @ZLIB_LIBS@

AM_LDFLAGS =
# AM_LDFLAGS += -rdynamic
AM_LDFLAGS += @OS_LDFLAGS@

if JIT
if FNO_PIE
AM_CPPFLAGS += -fno-pie
endif
if FNO_PIE_CAP
AM_CPPFLAGS += -fno-PIE
endif
if NO_PIE
AM_LDFLAGS += -no-pie
endif
# if WL_TDATA
# AM_LDFLAGS += -Wl,-Tdata=0x10000000
# endif
endif

if STATIC_BUILD
AM_LDFLAGS += -static
AM_CPPFLAGS += -DGLEW_STATIC
AM_CPPFLAGS += -DAL_LIBTYPE_STATIC
# Hack for OpenAL static missing libcommon.a
# LIBS += ../openal-soft/libcommon.a
# One or more .pc files are missing -liconv
LIBS += -liconv
endif

MSGFMT = @MSGFMT@
ZIP = @ZIP@

s = $(srcdir)
b = $(builddir)

bin_PROGRAMS = fs-uae

libexec_PROGRAMS =

dist_doc_DATA =

#noinst_PROGRAMS = \
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

AM_CPPFLAGS += -I$(s)/src/fsapp
AM_CPPFLAGS += -I$(s)/src/fsgui
AM_CPPFLAGS += -I$(s)/src/fslib
AM_CPPFLAGS += -I$(s)/src/fsuae

fs_uae_SOURCES = \
	src/fsapp/fsapp-channel.c \
	src/fsapp/fsapp-channel.h \
	src/fsapp/fsapp-config.h \
	src/fsapp/fsapp-events.c \
	src/fsapp/fsapp-events.h \
	src/fsapp/fsapp-main.c \
	src/fsapp/fsgui-config.h \
	src/fsgui/fsgui-font.c \
	src/fsgui/fsgui-font.h \
	src/fsgui/fsgui-image.c \
	src/fsgui/fsgui-image.h \
	src/fsgui/fsgui-surface.c \
	src/fsgui/fsgui-surface.h \
	src/fslib/fslib-config.h \
	src/fslib/fslib-data.c \
	src/fslib/fslib-data.h \
	src/fslib/fslib-os.c \
	src/fslib/fslib-os.h \
	src/fslib/fslib-path.c \
	src/fslib/fslib-path.h \
	src/fslib/fslib-refable.c \
	src/fslib/fslib-refable.h \
	src/fslib/fslib-stream.c \
	src/fslib/fslib-stream.h \
	src/fsuae/fsuae-bridge.c \
	src/fsuae/fsuae-bridge.h \
	src/fsuae/fsuae-config.h \
	src/fsuae/fsuae-constants.h \
	src/fsuae/fsuae-events.c \
	src/fsuae/fsuae-events.h \
	src/fsuae/fsuae-extras.cpp \
	src/fsuae/fsuae-extras.h \
	src/fsuae/fsuae-init.c \
	src/fsuae/fsuae-init.h \
	src/fsuae/fsuae-main.c \
	src/fsuae/fsuae-main.h \
	src/fsuae/fsuae-messages.cpp \
	src/fsuae/fsuae-messages.h \
	src/fsuae/fsuae-path.c \
	src/fsuae/fsuae-path.h \
	src/fsuae/fsuae-plugins.c \
	src/fsuae/fsuae-plugins.h \
	src/libuae/libuae-config.h \
	src/libuae/libuae-main.cpp \
	src/libuae/libuae-main.h \
	src/libuae/libuae-python.cpp \
	src/libuae/libuae-python.h

if WINDOWS
nodist_fs_uae_SOURCES += src/fsuae/fs-uae.rc
.rc.o:
	windres $< -o $@ -I$(s)
else
AM_CPPFLAGS += -I$(s)/od-fs/non-win32
endif

fs_uae_LDADD =

noinst_LIBRARIES += libuae.a
AM_CPPFLAGS += -I$(s)/include
AM_CPPFLAGS += -I$(s)/jit
AM_CPPFLAGS += -I$(s)/od-fs
AM_CPPFLAGS += -I$(s)/od-fs/include
AM_CPPFLAGS += -I$(s)/od-fs/flac/include
fs_uae_LDADD += libuae.a
# libuae_a_CXXFLAGS = $(CM_CFLAGS) -fpermissive
libuae_a_SOURCES = \
	a2065.cpp \
	a2091.cpp \
	akiko.cpp \
	amax.cpp \
	ar.cpp \
	arcadia.cpp \
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
	archivers/mp2/kjmp2.cpp \
	archivers/mp2/kjmp2.h \
	archivers/wrp/warp.h \
	archivers/zip/unzip.cpp \
	archivers/zip/unzip.h \
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
	include/casablanca.h \
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
	include/crc32.h \
	include/custom.h \
	include/debug.h \
	include/debugmem.h \
	include/devices.h \
	include/disk.h \
	include/diskutil.h \
	include/dongle.h \
	include/drawing.h \
	include/driveclick.h \
	include/enforcer.h \
	include/epsonprinter.h \
	include/ersatz.h \
	include/ethernet.h \
	include/events.h \
	include/execio.h \
	include/execlib.h \
	include/fake86_cpu.h \
	include/fdi2raw.h \
	include/filesys.h \
	include/flashrom.h \
	include/fpp-ieee-be.h \
	include/fpp-unknown.h \
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
	include/keyboard.h \
	include/keybuf.h \
	include/luascript.h \
	include/mackbd.h \
	include/mmu_common.h \
	include/moduleripper.h \
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
	include/uae/caps.h \
	include/uae/cdrom.h \
	include/uae/cycleunit.h \
	include/uae/debuginfo.h \
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
	inputrecord.cpp \
	isofs.cpp \
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
	od-fs/ahi_winuae.cpp \
	od-fs/ahidsound_new.cpp \
	od-fs/ahidsound_new.h \
	od-fs/ahidsound.h \
	od-fs/blkdev-linux.cpp \
	od-fs/bridge.cpp \
	od-fs/bsdsocket_posix.cpp \
	od-fs/callbacks.h \
	od-fs/cda_play.cpp \
	od-fs/cda_play.h \
	od-fs/cdimage_stubs.cpp \
	od-fs/charset.cpp \
	od-fs/clock.cpp \
	od-fs/config-fs.cpp \
	od-fs/defines.h \
	od-fs/driveclick.cpp \
	od-fs/filesys_host.cpp \
	od-fs/fsdb_host.cpp \
	od-fs/fsdb_host.h \
	od-fs/fsvideo.cpp \
	od-fs/gui.cpp \
	od-fs/hardfile_host.cpp \
	od-fs/include/direct3d.h \
	od-fs/include/registry.h \
	od-fs/include/resource.h \
	od-fs/include/uae/uae_inputevents_def.h \
	od-fs/include/uae/uae.h \
	od-fs/include/win32gui.h \
	od-fs/input.cpp \
	od-fs/inputrecord.cpp \
	od-fs/ioport.cpp \
	od-fs/ioport.h \
	od-fs/joystick.cpp \
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
	od-fs/mman.cpp \
	od-fs/mouse.cpp \
	od-fs/mp3decoder.h \
	od-fs/parser.cpp \
	od-fs/paths.cpp \
	od-fs/random.cpp \
	od-fs/roms.cpp \
	od-fs/sleep.h \
	od-fs/sound_fs.cpp \
	od-fs/sounddep/sound.h \
	od-fs/spin.cpp \
	od-fs/statusline.cpp \
	od-fs/stubs.cpp \
	od-fs/support.cpp \
	od-fs/sysconfig.h \
	od-fs/target.h \
	od-fs/threaddep/sem.h \
	od-fs/threaddep/thread.h \
	od-fs/threading.cpp \
	od-fs/uae_host.cpp \
	od-fs/uaemisc.cpp \
	od-fs/videograb_fs.cpp \
	od-fs/vpar.cpp \
	od-fs/vpar.h \
	od-fs/winuae_compat.h \
	od-fs/writelog_fs.cpp \
	od-win32/bsdsock.cpp \
	od-win32/caps/caps_win32.cpp \
	od-win32/caps/caps_win32.h \
	od-win32/caps/CapsAPI.h \
	od-win32/caps/Comtype.h \
	od-win32/clipboard_win32.cpp \
	od-win32/clipboard_win32.h \
	od-win32/debug_win32.h \
	od-win32/dxwrap.h \
	od-win32/md-fpp.h \
	od-win32/midi.h \
	od-win32/parser.h \
	od-win32/picasso96_win.cpp \
	od-win32/picasso96_win.h \
	od-win32/serial_win32.cpp \
	od-win32/win32_scaler.cpp \
	od-win32/win32_uaenet.cpp \
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

noinst_LIBRARIES += libfsemu0.a
AM_CPPFLAGS += -I$(s)/src/libfsemu/include
fs_uae_LDADD += libfsemu0.a
libfsemu0_a_SOURCES = \
	src/libfsemu/include/fs/base.h \
	src/libfsemu/include/fs/conf.h \
	src/libfsemu/include/fs/data.h \
	src/libfsemu/include/fs/defines.h \
	src/libfsemu/include/fs/emu.h \
	src/libfsemu/include/fs/endian.h \
	src/libfsemu/include/fs/eventlog.h \
	src/libfsemu/include/fs/filesys.h \
	src/libfsemu/include/fs/fs.h \
	src/libfsemu/include/fs/glib.h \
	src/libfsemu/include/fs/i18n.h \
	src/libfsemu/include/fs/image.h \
	src/libfsemu/include/fs/inifile.h \
	src/libfsemu/include/fs/init.h \
	src/libfsemu/include/fs/lazyness.h \
	src/libfsemu/include/fs/log.h \
	src/libfsemu/include/fs/main.h \
	src/libfsemu/include/fs/malloc.h \
	src/libfsemu/include/fs/ref.h \
	src/libfsemu/include/fs/thread.h \
	src/libfsemu/include/fs/time.h \
	src/libfsemu/include/fs/util.h \
	src/libfsemu/src/base.c \
	src/libfsemu/src/conf.c \
	src/libfsemu/src/data.c \
	src/libfsemu/src/eventlog.c \
	src/libfsemu/src/filesys.c \
	src/libfsemu/src/image.c \
	src/libfsemu/src/inifile.c \
	src/libfsemu/src/init.c \
	src/libfsemu/src/log.c \
	src/libfsemu/src/malloc.c \
	src/libfsemu/src/ref.c \
	src/libfsemu/src/thread.c \
	src/libfsemu/src/time.c


if FSEMU

noinst_LIBRARIES += libfsemu.a
AM_CPPFLAGS += -I$(s)/src/fsemu
fs_uae_LDADD += libfsemu.a
libfsemu_a_SOURCES = \
	src/fsemu/fsemu-action.c \
	src/fsemu/fsemu-action.h \
	src/fsemu/fsemu-actions.h \
	src/fsemu/fsemu-all.h \
	src/fsemu/fsemu-alsaaudio.c \
	src/fsemu/fsemu-alsaaudio.h \
	src/fsemu/fsemu-application.c \
	src/fsemu/fsemu-application.h \
	src/fsemu/fsemu-audio.c \
	src/fsemu/fsemu-audio.h \
	src/fsemu/fsemu-audiobuffer.c \
	src/fsemu/fsemu-audiobuffer.h \
	src/fsemu/fsemu-axis.h \
	src/fsemu/fsemu-background.c \
	src/fsemu/fsemu-background.h \
	src/fsemu/fsemu-button.h \
	src/fsemu/fsemu-color.h \
	src/fsemu/fsemu-common.c \
	src/fsemu/fsemu-common.h \
	src/fsemu/fsemu-compat.h \
	src/fsemu/fsemu-config.c \
	src/fsemu/fsemu-config.h \
	src/fsemu/fsemu-control.c \
	src/fsemu/fsemu-control.h \
	src/fsemu/fsemu-controller.c \
	src/fsemu/fsemu-controller.h \
	src/fsemu/fsemu-data.c \
	src/fsemu/fsemu-data.h \
	src/fsemu/fsemu-emuthread.c \
	src/fsemu/fsemu-emuthread.h \
	src/fsemu/fsemu-events.c \
	src/fsemu/fsemu-events.h \
	src/fsemu/fsemu-fade.c \
	src/fsemu/fsemu-fade.h \
	src/fsemu/fsemu-font.c \
	src/fsemu/fsemu-font.h \
	src/fsemu/fsemu-fontcache.c \
	src/fsemu/fsemu-fontcache.h \
	src/fsemu/fsemu-frame.c \
	src/fsemu/fsemu-frame.h \
	src/fsemu/fsemu-frameinfo.c \
	src/fsemu/fsemu-frameinfo.h \
	src/fsemu/fsemu-gamemode.c \
	src/fsemu/fsemu-gamemode.h \
	src/fsemu/fsemu-glib.h \
	src/fsemu/fsemu-glvideo.c \
	src/fsemu/fsemu-glvideo.h \
	src/fsemu/fsemu-gui.c \
	src/fsemu/fsemu-gui.h \
	src/fsemu/fsemu-helpbar.c \
	src/fsemu/fsemu-helpbar.h \
	src/fsemu/fsemu-helper.c \
	src/fsemu/fsemu-helper.h \
	src/fsemu/fsemu-hud.c \
	src/fsemu/fsemu-hud.h \
	src/fsemu/fsemu-image.c \
	src/fsemu/fsemu-image.h \
	src/fsemu/fsemu-inifile.c \
	src/fsemu/fsemu-inifile.h \
	src/fsemu/fsemu-input.c \
	src/fsemu/fsemu-input.h \
	src/fsemu/fsemu-inputdevice.c \
	src/fsemu/fsemu-inputdevice.h \
	src/fsemu/fsemu-inputmode.c \
	src/fsemu/fsemu-inputmode.h \
	src/fsemu/fsemu-inputport.c \
	src/fsemu/fsemu-inputport.h \
	src/fsemu/fsemu-internal.h \
	src/fsemu/fsemu-key.h \
	src/fsemu/fsemu-keyboard.c \
	src/fsemu/fsemu-keyboard.h \
	src/fsemu/fsemu-layer.h \
	src/fsemu/fsemu-layout.c \
	src/fsemu/fsemu-layout.h \
	src/fsemu/fsemu-led.c \
	src/fsemu/fsemu-led.h \
	src/fsemu/fsemu-leds.c \
	src/fsemu/fsemu-leds.h \
	src/fsemu/fsemu-log.c \
	src/fsemu/fsemu-log.h \
	src/fsemu/fsemu-main.c \
	src/fsemu/fsemu-main.h \
	src/fsemu/fsemu-mainmenu.c \
	src/fsemu/fsemu-mainmenu.h \
	src/fsemu/fsemu-manymouse.c \
	src/fsemu/fsemu-manymouse.h \
	src/fsemu/fsemu-media.c \
	src/fsemu/fsemu-media.h \
	src/fsemu/fsemu-menu.c \
	src/fsemu/fsemu-menu.h \
	src/fsemu/fsemu-module.c \
	src/fsemu/fsemu-module.h \
	src/fsemu/fsemu-monitor.c \
	src/fsemu/fsemu-monitor.h \
	src/fsemu/fsemu-mouse.c \
	src/fsemu/fsemu-mouse.h \
	src/fsemu/fsemu-mutex.c \
	src/fsemu/fsemu-mutex.h \
	src/fsemu/fsemu-opengl.c \
	src/fsemu/fsemu-opengl.h \
	src/fsemu/fsemu-opengl.h \
	src/fsemu/fsemu-option.c \
	src/fsemu/fsemu-option.h \
	src/fsemu/fsemu-options.h \
	src/fsemu/fsemu-oskeyboard.c \
	src/fsemu/fsemu-oskeyboard.c \
	src/fsemu/fsemu-oskeyboard.h \
	src/fsemu/fsemu-oskeyboard.h \
	src/fsemu/fsemu-osmenu.c \
	src/fsemu/fsemu-osmenu.h \
	src/fsemu/fsemu-path.c \
	src/fsemu/fsemu-path.h \
	src/fsemu/fsemu-perfgui.c \
	src/fsemu/fsemu-perfgui.h \
	src/fsemu/fsemu-python.c \
	src/fsemu/fsemu-python.h \
	src/fsemu/fsemu-quit.c \
	src/fsemu/fsemu-quit.h \
	src/fsemu/fsemu-recording.c \
	src/fsemu/fsemu-recording.h \
	src/fsemu/fsemu-render.c \
	src/fsemu/fsemu-render.h \
	src/fsemu/fsemu-savestate.c \
	src/fsemu/fsemu-savestate.h \
	src/fsemu/fsemu-screenshot.c \
	src/fsemu/fsemu-screenshot.h \
	src/fsemu/fsemu-sdl.h \
	src/fsemu/fsemu-sdlaudio.c \
	src/fsemu/fsemu-sdlaudio.h \
	src/fsemu/fsemu-sdlgamecontrollerdb.c \
	src/fsemu/fsemu-sdlgamecontrollerdb.h \
	src/fsemu/fsemu-sdlinput.c \
	src/fsemu/fsemu-sdlinput.h \
	src/fsemu/fsemu-sdlvideo.c \
	src/fsemu/fsemu-sdlvideo.h \
	src/fsemu/fsemu-sdlwindow.c \
	src/fsemu/fsemu-sdlwindow.h \
	src/fsemu/fsemu-semaphore.c \
	src/fsemu/fsemu-semaphore.h \
	src/fsemu/fsemu-shader.c \
	src/fsemu/fsemu-shader.h \
	src/fsemu/fsemu-startupinfo.c \
	src/fsemu/fsemu-startupinfo.h \
	src/fsemu/fsemu-stream.c \
	src/fsemu/fsemu-stream.h \
	src/fsemu/fsemu-theme.c \
	src/fsemu/fsemu-theme.h \
	src/fsemu/fsemu-thread.c \
	src/fsemu/fsemu-thread.h \
	src/fsemu/fsemu-time.c \
	src/fsemu/fsemu-time.h \
	src/fsemu/fsemu-titlebar.c \
	src/fsemu/fsemu-titlebar.h \
	src/fsemu/fsemu-types.h \
	src/fsemu/fsemu-util.c \
	src/fsemu/fsemu-util.h \
	src/fsemu/fsemu-video.c \
	src/fsemu/fsemu-video.h \
	src/fsemu/fsemu-videothread.c \
	src/fsemu/fsemu-videothread.h \
	src/fsemu/fsemu-widget.c \
	src/fsemu/fsemu-widget.h \
	src/fsemu/fsemu-window.c \
	src/fsemu/fsemu-window.h \
	src/fsemu/fsemu-xmlshader.c \
	src/fsemu/fsemu-xmlshader.h \
	src/fsemu/fsemu.c \
	src/fsemu/fsemu.h \
	src/gamemode/lib/gamemode_client.h \
	src/manymouse/linux_evdev.c \
	src/manymouse/macosx_hidmanager.c \
	src/manymouse/macosx_hidutilities.c \
	src/manymouse/manymouse.c \
	src/manymouse/manymouse.h \
	src/manymouse/windows_wminput.c \
	src/manymouse/x11_xinput2.c

fsemu_data_files =

fsuae_data_files =

# FIXME: EXTRA_DIST ?
libfsemu_a_SOURCES += $(fsemu_data_files)

#data/.fsemu: $(fsemu_data_files) ${fsuae_data_files}
#	mkdir -p $(b)/data/
#	cp $(s)/fsemu/fonts/saira/* $(b)/data/
#	cp $(s)/fsemu/icons/fontawesome/* $(b)/data/
#	cp $(s)/fsemu/images/* $(b)/data/
#	cp $(s)/src/fsuae/images/* $(b)/data/
#	touch data/.fsemu

fsemu-data: 
# data/.fsemu

else

fsemu-data:

endif

if BUILTIN_CEF
AM_CPPFLAGS += -I$(s)/cef
fs_uae_LDADD += cef/libcef_dll/libcef_dll_wrapper.a -L $(s)/cef/Release -lcef
endif

if BUILTIN_MANYMOUSE
# noinst_LIBRARIES += libmanymouse.a
# AM_CPPFLAGS += -I$(s)/manymouse
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

if WITH_GLAD
noinst_LIBRARIES += libglad.a
AM_CPPFLAGS += -I$(s)/od-fs/glad/include
fs_uae_LDADD += libglad.a
libglad_a_SOURCES = \
	od-fs/glad/src/glad.c
endif

if WITH_PEARPC_CPU
noinst_LIBRARIES += libpearpc.a
AM_CPPFLAGS += -I$(s)/src/ppc/pearpc
fs_uae_LDADD += libpearpc.a
libpearpc_a_SOURCES = \
	ppc/pearpc/cpu/common.h \
	ppc/pearpc/cpu/cpu_generic/ppc_alu.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_alu.h \
	ppc/pearpc/cpu/cpu_generic/ppc_cpu.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_cpu.h \
	ppc/pearpc/cpu/cpu_generic/ppc_dec.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_dec.h \
	ppc/pearpc/cpu/cpu_generic/ppc_exc.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_exc.h \
	ppc/pearpc/cpu/cpu_generic/ppc_fpu.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_fpu.h \
	ppc/pearpc/cpu/cpu_generic/ppc_mmu.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_mmu.h \
	ppc/pearpc/cpu/cpu_generic/ppc_opc.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_opc.h \
	ppc/pearpc/cpu/cpu_generic/ppc_tools.h \
	ppc/pearpc/cpu/cpu_generic/ppc_vec.cpp \
	ppc/pearpc/cpu/cpu_generic/ppc_vec.h \
	ppc/pearpc/cpu/cpu.h \
	ppc/pearpc/cpu/debug.h \
	ppc/pearpc/cpu/mem.h \
	ppc/pearpc/info.h \
	ppc/pearpc/io/io.h \
	ppc/pearpc/pearpc_config.h \
	ppc/pearpc/system/arch/generic/sysendian.h \
	ppc/pearpc/system/arch/generic/sysfeatures.h \
	ppc/pearpc/system/arch/sysendian.h \
	ppc/pearpc/system/arch/sysfeatures.h \
	ppc/pearpc/system/arch/x86_64/sysendian.h \
	ppc/pearpc/system/arch/x86_64/sysfeatures.h \
	ppc/pearpc/system/arch/x86/sysendian.h \
	ppc/pearpc/system/arch/x86/sysfeatures.h \
	ppc/pearpc/system/systhread.h \
	ppc/pearpc/system/types.h \
	ppc/pearpc/tools/debug.h \
	ppc/pearpc/tools/endianess.h \
	ppc/pearpc/tools/snprintf.h \
	ppc/pearpc/tracers.h \
	ppc/pearpc/uaeglue.cpp
endif

if WITH_CHD
noinst_LIBRARIES += libchd.a
fs_uae_LDADD +=  libchd.a
libchd_a_SOURCES = \
	archivers/chd/astring.h \
	archivers/chd/bitstream.h \
	archivers/chd/chd.h \
	archivers/chd/chdcd.cpp \
	archivers/chd/chdcd.h \
	archivers/chd/chdcdrom.cpp \
	archivers/chd/chdcdrom.h \
	archivers/chd/chdcodec.h \
	archivers/chd/chdtypes.h \
	archivers/chd/corefile.h \
	archivers/chd/coretmpl.h \
	archivers/chd/flac.h \
	archivers/chd/hashing.h \
	archivers/chd/huffman.h \
	archivers/chd/md5.h \
	archivers/chd/osdcore.h \
	archivers/chd/sha1.h
endif

if WITH_X86
noinst_LIBRARIES += libpcem.a
fs_uae_LDADD += libpcem.a
libpcem_a_SOURCES = \
	pcem/386_common.cpp \
	pcem/386_common.h \
	pcem/386_dynarec.cpp \
	pcem/386_ops.h \
	pcem/386.cpp \
	pcem/386.h \
	pcem/808x.cpp \
	pcem/codegen_x86-64.h \
	pcem/codegen_x86.h \
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
	pcem/fdc.h \
	pcem/filters.h \
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
	pcem/timer.cpp \
	pcem/timer.h \
	pcem/vid_cl5429.cpp \
	pcem/vid_cl5429.h \
	pcem/vid_s3.cpp \
	pcem/vid_s3.h \
	pcem/vid_sdac_ramdac.cpp \
	pcem/vid_sdac_ramdac.h \
	pcem/vid_svga_render.cpp \
	pcem/vid_svga_render.h \
	pcem/vid_svga.cpp \
	pcem/vid_svga.h \
	pcem/vid_s3_virge.cpp \
	pcem/vid_s3_virge.h \
	pcem/vid_permedia2.cpp \
	pcem/vid_permedia2.h \
	pcem/video.h \
	pcem/x86_flags.h \
	pcem/x86_ops_arith.h \
	pcem/x86_ops_atomic.h \
	pcem/x86_ops_bcd.h \
	pcem/x86_ops_bit.h \
	pcem/x86_ops_bitscan.h \
	pcem/x86_ops_call.h \
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

if WITH_PPC
fs_uae_SOURCES += \
	ppc/ppcd.cpp \
	ppc/ppcd.h
endif

if WITH_SLIRP
if WITH_BUILTIN_SLIRP
noinst_LIBRARIES += libslirp.a
fs_uae_LDADD += libslirp.a
LIBS += @SLIRP_LIBS@
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

if WITH_TOCCATA
fs_uae_SOURCES += \
	sndboard.cpp
endif

if WITH_UDIS86
noinst_LIBRARIES += libudis86.a
AM_CPPFLAGS += -I$(s)/od-fs/libudis86/include
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

if PROWIZARD
noinst_LIBRARIES += libprowizard.a
AM_CPPFLAGS += -I$(s)/prowizard/include
fs_uae_LDADD += libprowizard.a
fs_uae_SOURCES += moduleripper.cpp
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

if WITH_LIBMPEG2
CM_CFLAGS += @LIBMPEG2_CFLAGS@
LIBS += @LIBMPEG2_LIBS@
endif

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
#	echo "int cdummy;" > $(b)/gen/cdummy.c
#
#gen/cppdummy.cpp:
#	echo "int cppdummy;" > $(b)/gen/cppdummy.cpp

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

TESTS = \
	tests/dummy-test

EXTRA_TESTS = \
	tests/cppcheck-fs-uae \
	tests/cppcheck-libfsemu \
	tests/cppcheck-libmpeg2 \
	tests/cppcheck-manymouse \
	tests/cppcheck-od-fs \
	tests/cppcheck-pearpc \
	tests/cppcheck-ppc \
	tests/cppcheck-slirp \
	tests/cppcheck-uae

EXTRA_DIST = $(TESTS) $(EXTRA_TESTS) \
	$(fsuae_data_files) \
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/rules \
	debian/source \
	debian/source/format \
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
	docs/getting-started.md \
	docs/g-sync.md \
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
	src/flac/include/FLAC/all.h \
	src/flac/include/FLAC/assert.h \
	src/flac/include/FLAC/callback.h \
	src/flac/include/FLAC/export.h \
	src/flac/include/FLAC/format.h \
	src/flac/include/FLAC/metadata.h \
	src/flac/include/FLAC/ordinals.h \
	src/flac/include/FLAC/stream_decoder.h \
	src/flac/include/FLAC/stream_encoder.h \
	fsemu/gamemode/LICENSE.txt \
	fsemu/gamemode/README.md \
	fsemu/libinih/ini.c \
	fsemu/libinih/ini.h \
	fsemu/libinih/LICENSE.txt \
	fs-uae.spec \
	icon/fs-uae.ico \
	src/glad/include/glad/glad.h \
	src/glad/include/KHR/khrplatform.h \
	src/glad/LICENSE \
	src/libudis86/README \
	licenses/fs-uae.txt \
	licenses/glib.txt \
	licenses/ipflib.txt \
	licenses/libgcc.txt \
	licenses/libintl.txt \
	licenses/libpng.txt \
	licenses/libstdc++.txt \
	licenses/zlib.txt \
	po \
	slirp/CHANGELOG.md \
	slirp/COPYRIGHT \
	slirp/README.md \
	src/aks.def \
	src/filesys_bootrom.cpp \
	src/fsuae/fs-uae.rc.in \
	src/inputevents.def \
	src/jit/codegen_x86.cpp \
	src/jit/compemu_midfunc_x86.cpp \
	src/jit/compemu_prefs.cpp \
	src/jit/exception_handler.cpp \
	src/mame/tm34010/34010fld.c \
	src/mame/tm34010/34010tbl.c \
	src/mame/tm34010/34010ops.c \
	src/mame/tm34010/34010ops.h \
	src/mame/tm34010/34010gfx.c \
	src/od-win32/mman.cpp \
	src/p96_blit.cpp \
	src/sinctable.cpp \
	src/table68k \
	share/applications \
	share/fs-uae \
	share/icons \
	share/mime

CLEANFILES = $(BUILT_SOURCES)

if CODEGEN

#gen/blit.h: gen/genblitter$(EXEEXT)
#	$(b)/gen/genblitter$(EXEEXT) i > $(b)/gen/blit.h

#gen/blitfunc.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
#	$(b)/gen/genblitter$(EXEEXT) f > $(b)/gen/blitfunc.cpp

#gen/blitfunc.h: gen/genblitter$(EXEEXT)
#	$(b)/gen/genblitter$(EXEEXT) h > $(b)/gen/blitfunc.h

#gen/blittable.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
#	$(b)/gen/genblitter$(EXEEXT) t > $(b)/gen/blittable.cpp

#gen/compemu.cpp: gen/gencomp$(EXEEXT)
#	$(b)/gen/gencomp$(EXEEXT)

#gen/compstbl.cpp: gen/compemu.cpp

#gen/comptbl.h: gen/compemu.cpp

#gen/cpudefs.cpp: gen/build68k$(EXEEXT) src/table68k
#	$(b)/gen/build68k$(EXEEXT) < $(s)/src/table68k > $(b)/gen/cpudefs.cpp

#gen/cpuemu_0.cpp: gen/gencpu$(EXEEXT)
#	cd $(b)/gen && ./gencpu$(EXEEXT) --optimized-flags

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
#	$(b)/gen/genlinetoscr$(EXEEXT) > $(b)/gen/linetoscr.cpp

endif

catalogs =

share/locale/cs/LC_MESSAGES/fs-uae.mo: po/cs.po
	mkdir -p share/locale/cs/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/cs.po -o $@

share/locale/da/LC_MESSAGES/fs-uae.mo: po/da.po
	mkdir -p share/locale/da/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/da.po -o $@

share/locale/de/LC_MESSAGES/fs-uae.mo: po/de.po
	mkdir -p share/locale/de/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/de.po -o $@

share/locale/eo/LC_MESSAGES/fs-uae.mo: po/eo.po
	mkdir -p share/locale/eo/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/eo.po -o $@

share/locale/es/LC_MESSAGES/fs-uae.mo: po/es.po
	mkdir -p share/locale/es/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/es.po -o $@

share/locale/fi/LC_MESSAGES/fs-uae.mo: po/fi.po
	mkdir -p share/locale/fi/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/fi.po -o $@

share/locale/fr/LC_MESSAGES/fs-uae.mo: po/fr.po
	mkdir -p share/locale/fr/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/fr.po -o $@

share/locale/hu/LC_MESSAGES/fs-uae.mo: po/hu.po
	mkdir -p share/locale/hu/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/hu.po -o $@

share/locale/it/LC_MESSAGES/fs-uae.mo: po/it.po
	mkdir -p share/locale/it/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/it.po -o $@

share/locale/nb/LC_MESSAGES/fs-uae.mo: po/nb.po
	mkdir -p share/locale/nb/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/nb.po -o $@

share/locale/nl/LC_MESSAGES/fs-uae.mo: po/nl.po
	mkdir -p share/locale/nl/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/nl.po -o $@

share/locale/pl/LC_MESSAGES/fs-uae.mo: po/pl.po
	mkdir -p share/locale/pl/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/pl.po -o $@

share/locale/pt/LC_MESSAGES/fs-uae.mo: po/pt.po
	mkdir -p share/locale/pt/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/pt.po -o $@

share/locale/pt_BR/LC_MESSAGES/fs-uae.mo: po/pt_BR.po
	mkdir -p share/locale/pt_BR/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/pt_BR.po -o $@

share/locale/sr/LC_MESSAGES/fs-uae.mo: po/sr.po
	mkdir -p share/locale/sr/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/sr.po -o $@

share/locale/sv/LC_MESSAGES/fs-uae.mo: po/sv.po
	mkdir -p share/locale/sv/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/sv.po -o $@

share/locale/tr/LC_MESSAGES/fs-uae.mo: po/tr.po
	mkdir -p share/locale/tr/LC_MESSAGES
	$(MSGFMT) --verbose $(s)/po/tr.po -o $@

mo: $(catalogs)

fs-uae.dat: $(catalogs)
#	rm -f fs-uae.dat
#	for d in $(s) $(b) ; do \
#	  ( cd $$d && $(ZIP) -X $(abs_builddir)/fs-uae.dat $$(find share | LC_ALL=C sort ) ) ; \
#	done

all-local: mo fs-uae.dat fsemu-data

install-data-hook: mo fs-uae.dat
	chmod -R u+w $(s)/share  	# FIXME: Why?

	mkdir -p $(DESTDIR)$(datadir)/applications
	cp -R $(s)/share/applications/* $(DESTDIR)$(datadir)/applications/
	mkdir -p $(DESTDIR)$(datadir)/fs-uae
	touch $(DESTDIR)$(datadir)/fs-uae/share-dir
	cp fs-uae.dat $(DESTDIR)$(datadir)/fs-uae/
	cp -R data/* $(DESTDIR)$(datadir)/fs-uae/
	mkdir -p $(DESTDIR)$(datadir)/icons
	cp -R $(s)/share/icons/* $(DESTDIR)$(datadir)/icons/
	mkdir -p $(DESTDIR)$(datadir)/locale
	cp -R $(b)/share/locale/* $(DESTDIR)$(datadir)/locale/
	mkdir -p $(DESTDIR)$(datadir)/mime
	cp -R $(s)/share/mime/* $(DESTDIR)$(datadir)/mime/

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/applications/net.fs_uae.FS-UAE.desktop
	rm -Rf $(DESTDIR)$(datadir)/fs-uae
	find $(DESTDIR)$(datadir)/icons/hicolor -name net.fs_uae.FS-UAE.png -delete
	find $(DESTDIR)$(datadir)/locale -name fs-uae.mo -delete
	rm -f $(DESTDIR)$(datadir)/mime/packages/net.fs_uae.FS-UAE.xml

clean-local:
	rm -f fs-uae.dat
	rm -f data/*.ttf
	rm -f data/*.png
	rm -f data/.fsemu
#	find share -name fs-uae.mo -delete

format:
	find src/fsapp -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find src/fsemu -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find src/fsgui -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find src/fslib -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find src/fsuae -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i

	ruff format

.PHONY: fsemu-data
