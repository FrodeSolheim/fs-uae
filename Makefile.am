# Makefile.am for FS-UAE

SUBDIRS =

WARNINGS =

CM_CFLAGS =
CM_CFLAGS += @ALSA_CFLAGS@
CM_CFLAGS += @FREETYPE_CFLAGS@
CM_CFLAGS += @GLEW_CFLAGS@
CM_CFLAGS += @GLIB_CFLAGS@
# CM_CFLAGS += @GTK3_CFLAGS@
CM_CFLAGS += @LUA_CFLAGS@
CM_CFLAGS += @MIR_CFLAGS@
CM_CFLAGS += @OPENAL_CFLAGS@
CM_CFLAGS += @OPENGL_CFLAGS@
# CM_CFLAGS += @OS_CFLAGS@
CM_CFLAGS += @PNG_CFLAGS@
CM_CFLAGS += @QT_CFLAGS@
CM_CFLAGS += @SDL2_CFLAGS@
CM_CFLAGS += @SDL2_TTF_CFLAGS@
# CM_CFLAGS += @X11_CFLAGS@
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
CM_CFLAGS += -Werror=missing-declarations
endif
if W_POINTER_ARITH
CM_CFLAGS += -Wpointer-arith
endif


CM_CFLAGS += -fvisibility=hidden

AM_CFLAGS =
AM_CFLAGS += $(CM_CFLAGS) -std=gnu99

AM_CXXFLAGS =
AM_CXXFLAGS += $(CM_CFLAGS)

AM_CPPFLAGS =
AM_CPPFLAGS += @OS_CPPFLAGS@
AM_CPPFLAGS += -DUAE -DFSUAE

LIBS += @ALSA_LIBS@
LIBS += @CARBON_LIBS@
LIBS += @FREETYPE_LIBS@
LIBS += @GLEW_LIBS@
LIBS += @GLIB_LIBS@
# LIBS += @GTK3_LIBS@
LIBS += @IOKIT_LIBS@
# LIBS += @MIR_LIBS@
LIBS += @OPENAL_LIBS@
LIBS += @OPENGL_LIBS@
# LIBS += @OS_LIBS@
LIBS += @PNG_LIBS@
LIBS += @QT_LIBS@
LIBS += @MIDI_LIBS@
LIBS += @SDL2_LIBS@
LIBS += @SDL2_TTF_LIBS@
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
LIBS += ../openal-soft/libcommon.a
# One or more .pc files are missing -liconv
LIBS += -liconv
endif

MSGFMT = @MSGFMT@
ZIP = @ZIP@

s = $(srcdir)
b = $(builddir)

bin_PROGRAMS = fs-uae

libexec_PROGRAMS = fs-uae-device-helper

dist_doc_DATA = README COPYING

noinst_PROGRAMS = \
	gen/build68k \
	gen/genblitter \
	gen/gencomp \
	gen/gencpu \
	gen/genlinetoscr

noinst_LIBRARIES =

BUILT_SOURCES = \
	gen/linetoscr.cpp

nodist_fs_uae_SOURCES =

fs_uae_SOURCES = \
	src/fsuae/fsuae.h \
	src/fsuae/fs-uae.h \
	src/fsuae/fsuae-accelerator.c \
	src/fsuae/fsuae-accelerator.h \
	src/fsuae/fsuae-action.c \
	src/fsuae/fsuae-action.h \
	src/fsuae/fsuae-actions.h \
	src/fsuae/fsuae-config.c \
	src/fsuae/fsuae-config.h \
	src/fsuae/fsuae-graphics.c \
	src/fsuae/fsuae-graphics.h \
	src/fsuae/fsuae-hardware.c \
	src/fsuae/fsuae-hardware.h \
	src/fsuae/fsuae-input.c \
	src/fsuae/fsuae-input.h \
	src/fsuae/fsuae-inputport.c \
	src/fsuae/fsuae-inputport.h \
	src/fsuae/fsuae-joystick.c \
	src/fsuae/fsuae-joystick.h \
	src/fsuae/fsuae-keyboard.c \
	src/fsuae/fsuae-keyboard.h \
	src/fsuae/fsuae-main.c \
	src/fsuae/fsuae-main.h \
	src/fsuae/fsuae-media.c \
	src/fsuae/fsuae-media.h \
	src/fsuae/fsuae-menu.c \
	src/fsuae/fsuae-menu.h \
	src/fsuae/fsuae-model.c \
	src/fsuae/fsuae-model.h \
	src/fsuae/fsuae-mouse.c \
	src/fsuae/fsuae-mouse.h \
	src/fsuae/fsuae-options.h \
	src/fsuae/fsuae-oskeyboard.c \
	src/fsuae/fsuae-oskeyboard.h \
	src/fsuae/fsuae-path.c \
	src/fsuae/fsuae-path.h \
	src/fsuae/fsuae-plugins.c \
	src/fsuae/fsuae-plugins.h \
	src/fsuae/fsuae-recording.c \
	src/fsuae/fsuae-recording.h \
	src/fsuae/fsuae-savestate.c \
	src/fsuae/fsuae-savestate.h \
	src/fsuae/fsuae-sound.c \
	src/fsuae/fsuae-sound.h \
	src/fsuae/fsuae-video.c\
	src/fsuae/fsuae-video.h\
	src/fsuae/uae_config.c


if WINDOWS
nodist_fs_uae_SOURCES += src/fsuae/fs-uae.rc
.rc.o:
	windres $< -o $@ -I$(s)
endif

fs_uae_LDADD =

fs_uae_device_helper_SOURCES = \
	src/fsuae/device-helper.c

fs_uae_device_helper_LDADD =

noinst_LIBRARIES += libuae.a
AM_CPPFLAGS += -I$(s)/flac/include
AM_CPPFLAGS += -I$(b)/gen
AM_CPPFLAGS += -I$(s)/src
AM_CPPFLAGS += -I$(s)/src/jit
AM_CPPFLAGS += -I$(s)/src/include
AM_CPPFLAGS += -I$(s)/src/od-fs
AM_CPPFLAGS += -I$(s)/src/od-fs/include
AM_CPPFLAGS += -I$(s)/src/od-win32
AM_CPPFLAGS += -I$(s)/src/od-win32/caps
fs_uae_LDADD += libuae.a
# libuae_a_CXXFLAGS = $(CM_CFLAGS) -fpermissive
libuae_a_SOURCES = \
	src/a2065.cpp \
	src/a2091.cpp \
	src/akiko.cpp \
	src/amax.cpp \
	src/ar.cpp \
	src/arcadia.cpp \
	src/archivers/dms/cdata.h \
	src/archivers/dms/crc_csum.cpp \
	src/archivers/dms/crc_csum.h \
	src/archivers/dms/getbits.cpp \
	src/archivers/dms/getbits.h \
	src/archivers/dms/maketbl.cpp \
	src/archivers/dms/maketbl.h \
	src/archivers/dms/pfile.cpp \
	src/archivers/dms/pfile.h \
	src/archivers/dms/tables.cpp \
	src/archivers/dms/tables.h \
	src/archivers/dms/u_deep.cpp \
	src/archivers/dms/u_deep.h \
	src/archivers/dms/u_heavy.cpp \
	src/archivers/dms/u_heavy.h \
	src/archivers/dms/u_init.cpp \
	src/archivers/dms/u_init.h \
	src/archivers/dms/u_medium.cpp \
	src/archivers/dms/u_medium.h \
	src/archivers/dms/u_quick.cpp \
	src/archivers/dms/u_quick.h \
	src/archivers/dms/u_rle.cpp \
	src/archivers/dms/u_rle.h \
	src/archivers/mp2/kjmp2.cpp \
	src/archivers/mp2/kjmp2.h \
	src/archivers/wrp/warp.h \
	src/archivers/zip/unzip.cpp \
	src/archivers/zip/unzip.h \
	src/audio.cpp \
	src/autoconf.cpp \
	src/blitter.cpp \
	src/blkdev.cpp \
	src/blkdev_cdimage.cpp \
	src/bsdsocket.cpp \
	src/calc.cpp \
	src/casablanca.cpp \
	src/cd32_fmv.cpp \
	src/cd32_fmv_genlock.cpp \
	src/cdrom.cpp \
	src/cdtv.cpp \
	src/cdtvcr.cpp \
	src/cfgfile.cpp \
	src/cia.cpp \
	src/consolehook.cpp \
	src/cpuboard.cpp \
	src/cpummu.cpp \
	src/cpummu30.cpp \
	src/crc32.cpp \
	src/custom.cpp \
	src/debug.cpp \
	src/debuginfo.cpp \
	src/debugmem.cpp \
	src/def_icons.cpp \
	src/devices.cpp \
	src/disk.cpp \
	src/diskutil.cpp \
	src/dlopen.cpp \
	src/dongle.cpp \
	src/drawing.cpp \
	src/driveclick.cpp \
	src/enforcer.cpp \
	src/ethernet.cpp \
	src/events.cpp \
	src/expansion.cpp \
	src/fake86_cpu.cpp \
	src/fdi2raw.cpp \
	src/filesys.cpp \
	src/flashrom.cpp \
	src/fpp.cpp \
	src/fpp_native.cpp \
	src/fpp_softfloat.cpp \
	src/framebufferboards.cpp \
	src/fsdb.cpp \
	src/fsusage.cpp \
	src/gayle.cpp \
	src/gfxboard.cpp \
	src/gfxutil.cpp \
	src/hardfile.cpp \
	src/hrtmon.rom.cpp \
	src/ide.cpp \
	src/idecontrollers.cpp \
	src/identify.cpp \
	src/include/a2065.h \
	src/include/a2091.h \
	src/include/akiko.h \
	src/include/amax.h \
	src/include/ar.h \
	src/include/arcadia.h \
	src/include/audio.h \
	src/include/autoconf.h \
	src/include/blitter.h \
	src/include/blkdev.h \
	src/include/bsdsocket.h \
	src/include/calc.h \
	src/include/casablanca.h \
	src/include/catweasel.h \
	src/include/cd32_fmv.h \
	src/include/cdtv.h \
	src/include/cdtvcr.h \
	src/include/cia.h \
	src/include/clipboard.h \
	src/include/commpipe.h \
	src/include/consolehook.h \
	src/include/cpu_prefetch.h \
	src/include/cpuboard.h \
	src/include/cpummu.h \
	src/include/cpummu030.h \
	src/include/crc32.h \
	src/include/custom.h \
	src/include/debug.h \
	src/include/debugmem.h \
	src/include/devices.h \
	src/include/disk.h \
	src/include/diskutil.h \
	src/include/dongle.h \
	src/include/drawing.h \
	src/include/driveclick.h \
	src/include/enforcer.h \
	src/include/epsonprinter.h \
	src/include/ersatz.h \
	src/include/ethernet.h \
	src/include/events.h \
	src/include/execio.h \
	src/include/execlib.h \
	src/include/fake86_cpu.h \
	src/include/fdi2raw.h \
	src/include/filesys.h \
	src/include/flashrom.h \
	src/include/fpp.h \
	src/include/fpp-ieee-be.h \
	src/include/fpp-unknown.h \
	src/include/fsdb.h \
	src/include/fsusage.h \
	src/include/framebufferboards.h \
	src/include/gayle.h \
	src/include/genblitter.h \
	src/include/gensound.h \
	src/include/gfxboard.h \
	src/include/gfxfilter.h \
	src/include/gui.h \
	src/include/ide.h \
	src/include/idecontrollers.h \
	src/include/identify.h \
	src/include/ini.h \
	src/include/inputdevice.h \
	src/include/inputrecord.h \
	src/include/isofs.h \
	src/include/isofs_api.h \
	src/include/keyboard.h \
	src/include/keybuf.h \
	src/include/luascript.h \
	src/include/mackbd.h \
	src/include/mmu_common.h \
	src/include/moduleripper.h \
	src/include/native2amiga.h \
	src/include/native2amiga_api.h \
	src/include/ncr9x_scsi.h \
	src/include/ncr_scsi.h \
	src/include/newcpu.h \
	src/include/options.h \
	src/include/osemu.h \
	src/include/packed.h \
	src/include/parallel.h \
	src/include/pci.h \
	src/include/pci_hw.h \
	src/include/picasso96.h \
	src/include/readcpu.h \
	src/include/rommgr.h \
	src/include/rtc.h \
	src/include/rtgmodes.h \
	src/include/sampler.h \
	src/include/sana2.h \
	src/include/savestate.h \
	src/include/scp.h \
	src/include/scsi.h \
	src/include/scsidev.h \
	src/include/serial.h \
	src/include/sndboard.h \
	src/include/specialmonitors.h \
	src/include/statusline.h \
	src/include/sysdeps.h \
	src/include/tabletlibrary.h \
	src/include/traps.h \
	src/include/uae.h \
	src/include/uae/ahi.h \
	src/include/uae/api.h \
	src/include/uae/asm.h \
	src/include/uae/assert.h \
	src/include/uae/atomic.h \
	src/include/uae/attributes.h \
	src/include/uae/byteswap.h \
	src/include/uae/caps.h \
	src/include/uae/cdrom.h \
	src/include/uae/cycleunit.h \
	src/include/uae/debuginfo.h \
	src/include/uae/dlopen.h \
	src/include/uae/endian.h \
	src/include/uae/enum.h \
	src/include/uae/fs.h \
	src/include/uae/funcattr.h \
	src/include/uae/glib.h \
	src/include/uae/inline.h \
	src/include/uae/io.h \
	src/include/uae/jitconfig.h \
	src/include/uae/likely.h \
	src/include/uae/limits.h \
	src/include/uae/log.h \
	src/include/uae/maccess.h \
	src/include/uae/memory.h \
	src/include/uae/mman.h \
	src/include/uae/parser.h \
	src/include/uae/patch.h \
	src/include/uae/ppc.h \
	src/include/uae/qemu.h \
	src/include/uae/segtracker.h \
	src/include/uae/seh.h \
	src/include/uae/slirp.h \
	src/include/uae/socket.h \
	src/include/uae/string.h \
	src/include/uae/time.h \
	src/include/uae/types.h \
	src/include/uae/vm.h \
	src/include/uaeexe.h \
	src/include/uaeipc.h \
	src/include/uaenative.h \
	src/include/uaeresource.h \
	src/include/uaeserial.h \
	src/include/uni_common.h \
	src/include/unpacked.h \
	src/include/videograb.h \
	src/include/x86.h \
	src/include/xwin.h \
	src/include/zarchive.h \
	src/include/zfile.h \
	src/ini.cpp \
	src/inputdevice.cpp \
	src/isofs.cpp \
	src/jit/codegen_udis86.h \
	src/jit/codegen_x86.h \
	src/jit/compemu.h \
	src/jit/compemu_fpp.cpp \
	src/jit/compemu_midfunc_arm.h \
	src/jit/compemu_midfunc_x86.h \
	src/jit/compemu_support.cpp \
	src/jit/flags_x86.h \
	src/keybuf.cpp \
	src/logging.cpp \
	src/main.cpp \
	src/mame/a2410.cpp \
	src/mame/mameglue.h \
	src/mame/tm34010/tms34010.cpp \
	src/mame/tm34010/tms34010.h \
	src/memory.cpp \
	src/native2amiga.cpp \
	src/ncr9x_scsi.cpp \
	src/ncr_scsi.cpp \
	src/newcpu.cpp \
	src/newcpu_common.cpp \
	src/od-fs/ahi_winuae.cpp \
	src/od-fs/ahidsound.h \
	src/od-fs/ahidsound_new.cpp \
	src/od-fs/ahidsound_new.h \
	src/od-fs/blkdev-linux.cpp \
	src/od-fs/bsdsocket_posix.cpp \
	src/od-fs/callbacks.h \
	src/od-fs/cda_play.cpp \
	src/od-fs/cda_play.h \
	src/od-fs/cdimage_stubs.cpp \
	src/od-fs/charset.cpp \
	src/od-fs/clock.cpp \
	src/od-fs/defines.h \
	src/od-fs/driveclick.cpp \
	src/od-fs/filesys_host.cpp \
	src/od-fs/fsdb_host.cpp \
	src/od-fs/fsdb_host.h \
	src/od-fs/fsvideo.cpp \
	src/od-fs/gui.cpp \
	src/od-fs/hardfile_host.cpp \
	src/od-fs/include/direct3d.h \
	src/od-fs/include/registry.h \
	src/od-fs/include/resource.h \
	src/od-fs/include/uae/uae.h \
	src/od-fs/include/uae/uae_inputevents_def.h \
	src/od-fs/include/win32gui.h \
	src/od-fs/input.cpp \
	src/od-fs/inputrecord.cpp \
	src/od-fs/ioport.cpp \
	src/od-fs/ioport.h \
	src/od-fs/joystick.cpp \
	src/od-fs/keyboard.cpp \
	src/od-fs/libamiga.cpp \
	src/od-fs/machdep/arm/maccess.h \
	src/od-fs/machdep/generic/maccess.h \
	src/od-fs/machdep/m68k.h \
	src/od-fs/machdep/maccess.h \
	src/od-fs/machdep/machdep.h \
	src/od-fs/machdep/ppc/maccess.h \
	src/od-fs/machdep/rpt.h \
	src/od-fs/machdep/x86/maccess.h \
	src/od-fs/midi.cpp \
	src/od-fs/midi.h \
	src/od-fs/mman.cpp \
	src/od-fs/mouse.cpp \
	src/od-fs/mp3decoder.h \
	src/od-fs/parser.cpp \
	src/od-fs/paths.cpp \
	src/od-fs/roms.cpp \
	src/od-fs/sleep.h \
	src/od-fs/sound_fs.cpp \
	src/od-fs/spin.cpp \
	src/od-fs/sounddep/sound.h \
	src/od-fs/statusline.cpp \
	src/od-fs/stubs.cpp \
	src/od-fs/support.cpp \
	src/od-fs/sysconfig.h \
	src/od-fs/target.h \
	src/od-fs/threaddep/sem.h \
	src/od-fs/threaddep/thread.h \
	src/od-fs/threading.cpp \
	src/od-fs/uae_host.cpp \
	src/od-fs/uaemisc.cpp \
	src/od-fs/videograb_fs.cpp \
	src/od-fs/vpar.cpp \
	src/od-fs/vpar.h \
	src/od-fs/winuae_compat.h \
	src/od-fs/writelog_fs.cpp \
	src/od-win32/bsdsock.cpp \
	src/od-win32/caps/CapsAPI.h \
	src/od-win32/caps/Comtype.h \
	src/od-win32/caps/caps_win32.cpp \
	src/od-win32/caps/caps_win32.h \
	src/od-win32/clipboard_win32.cpp \
	src/od-win32/clipboard_win32.h \
	src/od-win32/debug_win32.h \
	src/od-win32/dxwrap.h \
	src/od-win32/md-fpp.h \
	src/od-win32/midi.h \
	src/od-win32/parser.h \
	src/od-win32/picasso96_win.cpp \
	src/od-win32/picasso96_win.h \
	src/od-win32/serial_win32.cpp \
	src/od-win32/win32.h \
	src/od-win32/win32gfx.h \
	src/od-win32/win32_uaenet.cpp \
	src/patch.cpp \
	src/pcem/codegen.h \
	src/pcem/codegen_x86-64.h \
	src/pcem/cpu.h \
	src/pcem/dma.h \
	src/pcem/ibm.h \
	src/pcem/keyboard.h \
	src/pcem/keyboard_at.h \
	src/pcem/mem.h \
	src/pcem/model.h \
	src/pcem/mouse.h \
	src/pcem/mouse_serial.h \
	src/pcem/nvr.h \
	src/pcem/pcemglue.h \
	src/pcem/pic.h \
	src/pcem/pit.h \
	src/pcem/sound.h \
	src/pcem/sound_cms.h \
	src/pcem/sound_speaker.h \
	src/pcem/timer.h \
	src/pcem/x86.h \
	src/pcem/x86_ops.h \
	src/pci.cpp \
	src/ppc/pixman-stubs.cpp \
	src/ppc/ppc.cpp \
	src/qemu.cpp \
	src/qemuvga/cirrus_vga.cpp \
	src/qemuvga/cirrus_vga_rop.h \
	src/qemuvga/cirrus_vga_rop2.h \
	src/qemuvga/cirrus_vga_template.h \
	src/qemuvga/es1370.cpp \
	src/qemuvga/esp.cpp \
	src/qemuvga/lsi53c710.cpp \
	src/qemuvga/lsi53c895a.cpp \
	src/qemuvga/ne2000.cpp \
	src/qemuvga/ne2000.h \
	src/qemuvga/pixel_ops.h \
	src/qemuvga/qemuaudio.h \
	src/qemuvga/qemumemory.h \
	src/qemuvga/qemuuaeglue.cpp \
	src/qemuvga/qemuuaeglue.h \
	src/qemuvga/queue.h \
	src/qemuvga/scsi/esp.h \
	src/qemuvga/scsi/scsi.h \
	src/qemuvga/vga.cpp \
	src/qemuvga/vga.h \
	src/qemuvga/vga_int.h \
	src/qemuvga/vga_template.h \
	src/random.cpp \
	src/readcpu.cpp \
	src/rommgr.cpp \
	src/rtc.cpp \
	src/sana2.cpp \
	src/savestate.cpp \
	src/scp.cpp \
	src/scsi.cpp \
	src/scsiemul.cpp \
	src/scsitape.cpp \
	src/segtracker.cpp \
	src/slirp_uae.cpp \
	src/softfloat/softfloat.cpp \
	src/softfloat/softfloat_decimal.cpp \
	src/softfloat/softfloat_fpsp.cpp \
	src/softfloat/softfloat_fpsp_tables.h \
	src/softfloat/softfloat.h \
	src/softfloat/softfloat-macros.h \
	src/softfloat/softfloat-specialize.h \
	src/softfloat/softfloat.cpp \
	src/softfloat/softfloat_decimal.cpp \
	src/softfloat/softfloat_fpsp.cpp \
	src/specialmonitors.cpp \
	src/statusline.cpp \
	src/support/socket.cpp \
	src/support/time.cpp \
	src/test_card.cpp \
	src/traps.cpp \
	src/uaeexe.cpp \
	src/uaelib.cpp \
	src/uaenative.cpp \
	src/uaeresource.cpp \
	src/uaeserial.cpp \
	src/vm.cpp \
	src/x86.cpp \
	src/zfile.cpp \
	src/zfile_archive.cpp
nodist_libuae_a_SOURCES = \
	gen/blit.h \
	gen/blitfunc.cpp \
	gen/blitfunc.h \
	gen/blittable.cpp \
	gen/compemu.cpp \
	gen/compstbl.cpp \
	gen/comptbl.h \
	gen/cpudefs.cpp \
	gen/cpustbl.cpp \
	gen/cputbl.h
BUILT_SOURCES += $(nodist_libuae_a_SOURCES)

noinst_LIBRARIES += libcpuemu.a
fs_uae_LDADD += libcpuemu.a
nodist_libcpuemu_a_SOURCES = \
        gen/cpuemu_0.cpp \
        gen/cpuemu_11.cpp \
        gen/cpuemu_13.cpp \
        gen/cpuemu_20.cpp \
        gen/cpuemu_21.cpp \
        gen/cpuemu_22.cpp \
        gen/cpuemu_23.cpp \
        gen/cpuemu_24.cpp \
        gen/cpuemu_31.cpp \
        gen/cpuemu_32.cpp \
        gen/cpuemu_33.cpp \
        gen/cpuemu_34.cpp \
        gen/cpuemu_35.cpp \
        gen/cpuemu_40.cpp \
        gen/cpuemu_50.cpp
BUILT_SOURCES += $(nodist_libcpuemu_a_SOURCES)

noinst_LIBRARIES += libfsemu.a
AM_CPPFLAGS += -I$(s)/libfsemu/include
fs_uae_LDADD += libfsemu.a
fs_uae_device_helper_LDADD += libfsemu.a
libfsemu_a_SOURCES = \
	libfsemu/include/fs/base.h \
	libfsemu/include/fs/conf.h \
	libfsemu/include/fs/data.h \
	libfsemu/include/fs/defines.h \
	libfsemu/include/fs/emu.h \
	libfsemu/include/fs/emu/actions.h \
	libfsemu/include/fs/emu/audio.h \
	libfsemu/include/fs/emu/benchmark.h \
	libfsemu/include/fs/emu/buffer.h \
	libfsemu/include/fs/emu/dialog.h \
	libfsemu/include/fs/emu/hacks.h \
	libfsemu/include/fs/emu/log.h \
	libfsemu/include/fs/emu/input.h \
	libfsemu/include/fs/emu/monitor.h \
	libfsemu/include/fs/emu/options.h \
	libfsemu/include/fs/emu/path.h \
	libfsemu/include/fs/emu/render.h \
	libfsemu/include/fs/emu/video.h \
	libfsemu/include/fs/endian.h \
	libfsemu/include/fs/eventlog.h \
	libfsemu/include/fs/filesys.h \
	libfsemu/include/fs/fs.h \
	libfsemu/include/fs/glib.h \
	libfsemu/include/fs/i18n.h \
	libfsemu/include/fs/image.h \
	libfsemu/include/fs/inifile.h \
	libfsemu/include/fs/init.h \
	libfsemu/include/fs/lazyness.h \
	libfsemu/include/fs/log.h \
	libfsemu/include/fs/main.h \
	libfsemu/include/fs/malloc.h \
	libfsemu/include/fs/ml.h \
	libfsemu/include/fs/ml/opengl.h \
	libfsemu/include/fs/ml/options.h \
	libfsemu/include/fs/ref.h \
	libfsemu/include/fs/thread.h \
	libfsemu/include/fs/time.h \
	libfsemu/include/fs/util.h \
	libfsemu/include/fsemu/performance.h \
	libfsemu/src/audio/alsa-audio.c \
	libfsemu/src/audio/audio.c \
	libfsemu/src/audio/dummy-audio.c \
	libfsemu/src/audio/openal-audio.c \
	libfsemu/src/audio/sdl-audio.c \
	libfsemu/src/base.c \
	libfsemu/src/conf.c \
	libfsemu/src/data.c \
	libfsemu/src/emu/actions.c \
	libfsemu/src/emu/audio.h \
	libfsemu/src/emu/dialog.c \
	libfsemu/src/emu/dialog.h \
	libfsemu/src/emu/emu.c \
	libfsemu/src/emu/emu_lua.h \
	libfsemu/src/emu/font.c \
	libfsemu/src/emu/font.h \
	libfsemu/src/emu/hud.c \
	libfsemu/src/emu/hud.h \
	libfsemu/src/emu/input.c \
	libfsemu/src/emu/input.h \
	libfsemu/src/emu/keynames.c \
	libfsemu/src/emu/libfsemu.h \
	libfsemu/src/emu/menu.c \
	libfsemu/src/emu/menu.h \
	libfsemu/src/emu/netplay.c \
	libfsemu/src/emu/netplay.h \
	libfsemu/src/emu/performance.c \
	libfsemu/src/emu/render.c \
	libfsemu/src/emu/render.h \
	libfsemu/src/emu/scanlines.c \
	libfsemu/src/emu/scanlines.h \
	libfsemu/src/emu/texture.c \
	libfsemu/src/emu/texture.h \
	libfsemu/src/emu/theme.c \
	libfsemu/src/emu/theme.h \
	libfsemu/src/emu/util.c \
	libfsemu/src/emu/util.h \
	libfsemu/src/emu/video.c \
	libfsemu/src/emu/video.h \
	libfsemu/src/emu/video_buffer.c \
	libfsemu/src/emu/video_buffer.h \
	libfsemu/src/emu/video_options.c \
	libfsemu/src/emu/xml_shader.c \
	libfsemu/src/emu/xml_shader.h \
	libfsemu/src/eventlog.c \
	libfsemu/src/filesys.c \
	libfsemu/src/image.c \
	libfsemu/src/input/input-actions.c \
	libfsemu/src/input/input.c \
	libfsemu/src/input/legacy-input.c \
	libfsemu/src/input/sdl-input.c \
	libfsemu/src/inifile.c \
	libfsemu/src/init.c \
	libfsemu/src/log.c \
	libfsemu/src/malloc.c \
	libfsemu/src/ml/input.c \
	libfsemu/src/ml/keyboard.c \
	libfsemu/src/ml/macosx.c \
	libfsemu/src/ml/ml.c \
	libfsemu/src/ml/ml_internal.h \
	libfsemu/src/ml/mouse.c \
	libfsemu/src/ml/opengl.c \
	libfsemu/src/ml/rawinput.c \
	libfsemu/src/ml/render.c \
	libfsemu/src/ml/sdl.c \
	libfsemu/src/ml/sdl2_keys.h \
	libfsemu/src/ml/util.c \
	libfsemu/src/ml/video.c \
	libfsemu/src/ml/video.c \
	libfsemu/src/ml/video_mode.c \
	libfsemu/src/ml/video_sync.c \
	libfsemu/src/ml/windows.c \
	libfsemu/src/ml/x11.c \
	libfsemu/src/opengl.c \
	libfsemu/src/path.c \
	libfsemu/src/ref.c \
	libfsemu/src/thread.c \
	libfsemu/src/time.c \
	libfsemu/src/video/dummy-video.c \
	libfsemu/src/video/legacy-video.c \
	libfsemu/src/video/opengl-render.c \
	libfsemu/src/video/render.c \
	libfsemu/src/video/sdl-video.c \
	libfsemu/src/video/sdl-video-common.c \
	libfsemu/src/video/sdl-video-software.c \
	libfsemu/src/video/video.c \
	libfsemu/src/video/video-buffer.c

if FSEMU

noinst_LIBRARIES += libfsemu2.a
 AM_CPPFLAGS += -I$(s)/fsemu/src
fs_uae_LDADD += libfsemu2.a
fs_uae_device_helper_LDADD += libfsemu2.a
libfsemu2_a_SOURCES = \
	fsemu/gamemode/lib/gamemode_client.h \
	fsemu/manymouse/linux_evdev.c \
	fsemu/manymouse/macosx_hidmanager.c \
	fsemu/manymouse/macosx_hidutilities.c \
	fsemu/manymouse/manymouse.c \
	fsemu/manymouse/manymouse.h \
	fsemu/manymouse/windows_wminput.c \
	fsemu/manymouse/x11_xinput2.c \
	fsemu/src/fsemu.c \
	fsemu/src/fsemu.h \
	fsemu/src/fsemu-all.h \
	fsemu/src/fsemu-button.h \
	fsemu/src/fsemu-action.c \
	fsemu/src/fsemu-action.h \
	fsemu/src/fsemu-actions.h \
	fsemu/src/fsemu-alsaaudio.c \
	fsemu/src/fsemu-alsaaudio.h \
	fsemu/src/fsemu-application.c \
	fsemu/src/fsemu-application.h \
	fsemu/src/fsemu-audiobuffer.c \
	fsemu/src/fsemu-audiobuffer.h \
	fsemu/src/fsemu-audio.c \
	fsemu/src/fsemu-audio.h \
	fsemu/src/fsemu-axis.h \
	fsemu/src/fsemu-background.c \
	fsemu/src/fsemu-background.h \
	fsemu/src/fsemu-color.h \
	fsemu/src/fsemu-common.c \
	fsemu/src/fsemu-common.h \
	fsemu/src/fsemu-compat.h \
	fsemu/src/fsemu-config.c \
	fsemu/src/fsemu-config.h \
	fsemu/src/fsemu-control.c \
	fsemu/src/fsemu-control.h \
	fsemu/src/fsemu-controller.c \
	fsemu/src/fsemu-controller.h \
	fsemu/src/fsemu-data.c \
	fsemu/src/fsemu-data.h \
	fsemu/src/fsemu-emuthread.c \
	fsemu/src/fsemu-emuthread.h \
	fsemu/src/fsemu-events.c \
	fsemu/src/fsemu-events.h \
	fsemu/src/fsemu-fade.c \
	fsemu/src/fsemu-fade.h \
	fsemu/src/fsemu-font.c \
	fsemu/src/fsemu-font.h \
	fsemu/src/fsemu-fontcache.c \
	fsemu/src/fsemu-fontcache.h \
	fsemu/src/fsemu-frame.c \
	fsemu/src/fsemu-frame.h \
	fsemu/src/fsemu-frameinfo.c \
	fsemu/src/fsemu-frameinfo.h \
	fsemu/src/fsemu-gamemode.c \
	fsemu/src/fsemu-gamemode.h \
	fsemu/src/fsemu-glib.h \
	fsemu/src/fsemu-opengl.h \
	fsemu/src/fsemu-oskeyboard.c \
	fsemu/src/fsemu-oskeyboard.h \
	fsemu/src/fsemu-glvideo.c \
	fsemu/src/fsemu-glvideo.h \
	fsemu/src/fsemu-gui.c \
	fsemu/src/fsemu-gui.h \
	fsemu/src/fsemu-helpbar.c \
	fsemu/src/fsemu-helpbar.h \
	fsemu/src/fsemu-helper.c \
	fsemu/src/fsemu-helper.h \
	fsemu/src/fsemu-hud.c \
	fsemu/src/fsemu-hud.h \
	fsemu/src/fsemu-image.c \
	fsemu/src/fsemu-image.h \
	fsemu/src/fsemu-inifile.c \
	fsemu/src/fsemu-inifile.h \
	fsemu/src/fsemu-input.c \
	fsemu/src/fsemu-input.h \
	fsemu/src/fsemu-inputdevice.c \
	fsemu/src/fsemu-inputdevice.h \
	fsemu/src/fsemu-inputmode.c \
	fsemu/src/fsemu-inputmode.h \
	fsemu/src/fsemu-inputport.c \
	fsemu/src/fsemu-inputport.h \
	fsemu/src/fsemu-internal.h \
	fsemu/src/fsemu-key.h \
	fsemu/src/fsemu-keyboard.h \
	fsemu/src/fsemu-layer.h \
	fsemu/src/fsemu-layout.c \
	fsemu/src/fsemu-layout.h \
	fsemu/src/fsemu-led.c \
	fsemu/src/fsemu-led.h \
	fsemu/src/fsemu-leds.c \
	fsemu/src/fsemu-leds.h \
	fsemu/src/fsemu-log.c \
	fsemu/src/fsemu-log.h \
	fsemu/src/fsemu-main.c \
	fsemu/src/fsemu-main.h \
	fsemu/src/fsemu-mainmenu.c \
	fsemu/src/fsemu-mainmenu.h \
	fsemu/src/fsemu-manymouse.c \
	fsemu/src/fsemu-manymouse.h \
	fsemu/src/fsemu-media.c \
	fsemu/src/fsemu-media.h \
	fsemu/src/fsemu-menu.c \
	fsemu/src/fsemu-menu.h \
	fsemu/src/fsemu-module.c \
	fsemu/src/fsemu-module.h \
	fsemu/src/fsemu-monitor.c \
	fsemu/src/fsemu-monitor.h \
	fsemu/src/fsemu-mouse.c \
	fsemu/src/fsemu-mouse.h \
	fsemu/src/fsemu-mutex.c \
	fsemu/src/fsemu-mutex.h \
	fsemu/src/fsemu-opengl.c \
	fsemu/src/fsemu-opengl.h \
	fsemu/src/fsemu-option.c \
	fsemu/src/fsemu-option.h \
	fsemu/src/fsemu-options.h \
	fsemu/src/fsemu-oskeyboard.c \
	fsemu/src/fsemu-oskeyboard.h \
	fsemu/src/fsemu-osmenu.c \
	fsemu/src/fsemu-osmenu.h \
	fsemu/src/fsemu-path.c \
	fsemu/src/fsemu-path.h \
	fsemu/src/fsemu-perfgui.c \
	fsemu/src/fsemu-perfgui.h \
	fsemu/src/fsemu-quit.c \
	fsemu/src/fsemu-quit.h \
	fsemu/src/fsemu-recording.c \
	fsemu/src/fsemu-recording.h \
	fsemu/src/fsemu-refable.c \
	fsemu/src/fsemu-refable.h \
	fsemu/src/fsemu-render.c \
	fsemu/src/fsemu-render.h \
	fsemu/src/fsemu-savestate.c \
	fsemu/src/fsemu-savestate.h \
	fsemu/src/fsemu-screenshot.c \
	fsemu/src/fsemu-screenshot.h \
	fsemu/src/fsemu-sdl.h \
	fsemu/src/fsemu-sdlaudio.c \
	fsemu/src/fsemu-sdlaudio.h \
	fsemu/src/fsemu-sdlgamecontrollerdb.c \
	fsemu/src/fsemu-sdlgamecontrollerdb.h \
	fsemu/src/fsemu-sdlinput.c \
	fsemu/src/fsemu-sdlinput.h \
	fsemu/src/fsemu-sdlvideo.c \
	fsemu/src/fsemu-sdlvideo.h \
	fsemu/src/fsemu-sdlwindow.c \
	fsemu/src/fsemu-sdlwindow.h \
	fsemu/src/fsemu-semaphore.c \
	fsemu/src/fsemu-semaphore.h \
	fsemu/src/fsemu-shader.c \
	fsemu/src/fsemu-shader.h \
	fsemu/src/fsemu-startupinfo.c \
	fsemu/src/fsemu-startupinfo.h \
	fsemu/src/fsemu-stream.c \
	fsemu/src/fsemu-stream.h \
	fsemu/src/fsemu-theme.c \
	fsemu/src/fsemu-theme.h \
	fsemu/src/fsemu-thread.c \
	fsemu/src/fsemu-thread.h \
	fsemu/src/fsemu-time.c \
	fsemu/src/fsemu-time.h \
	fsemu/src/fsemu-titlebar.c \
	fsemu/src/fsemu-titlebar.h \
	fsemu/src/fsemu-types.h \
	fsemu/src/fsemu-util.c \
	fsemu/src/fsemu-util.h \
	fsemu/src/fsemu-video.c \
	fsemu/src/fsemu-video.h \
	fsemu/src/fsemu-videothread.c \
	fsemu/src/fsemu-videothread.h \
	fsemu/src/fsemu-widget.c \
	fsemu/src/fsemu-widget.h \
	fsemu/src/fsemu-window.c \
	fsemu/src/fsemu-window.h \
	fsemu/src/fsemu-xmlshader.c \
	fsemu/src/fsemu-xmlshader.h

fsemu_data_files = \
	fsemu/fonts/saira/SairaCondensed-Bold.ttf \
	fsemu/fonts/saira/SairaCondensed-Medium.ttf \
	fsemu/fonts/saira/SairaCondensed-Regular.ttf \
	fsemu/fonts/saira/SairaCondensed-SemiBold.ttf \
	fsemu/icons/fontawesome/NotificationCursor.png \
	fsemu/icons/fontawesome/NotificationInfo.png \
	fsemu/icons/fontawesome/NotificationKeyboard.png \
	fsemu/icons/fontawesome/NotificationPause.png \
	fsemu/icons/fontawesome/NotificationWarning.png \
	fsemu/icons/fontawesome/NotificationWarp.png \
	fsemu/images/ShortcutKeyAlt.png \
	fsemu/images/TitleBarCloseIcon.png \
	fsemu/images/TitleBarMaximizeIcon.png \
	fsemu/images/TitleBarMenuIcon.png \
	fsemu/images/TitleBarMinimizeIcon.png \
	fsemu/images/TitleBarWithShadow.png

fsuae_data_files = \
	src/fsuae/images/DiskLed_On.png \
	src/fsuae/images/DiskLed.png \
	src/fsuae/images/FloppyLed_On.png \
	src/fsuae/images/FloppyLed.png \
	src/fsuae/images/KLockLed_On.png \
	src/fsuae/images/KLockLed.png \
	src/fsuae/images/MLockLed_On.png \
	src/fsuae/images/MLockLed.png \
	src/fsuae/images/PowerLed_On.png \
	src/fsuae/images/PowerLed.png \
	src/fsuae/images/VSyncLed_Alt.png \
	src/fsuae/images/VSyncLed_On.png \
	src/fsuae/images/VSyncLed.png

# FIXME: EXTRA_DIST ?
libfsemu2_a_SOURCES += $(fsemu_data_files)

data/.fsemu: $(fsemu_data_files) ${fsuae_data_files}
	mkdir -p $(b)/data/
	cp $(s)/fsemu/fonts/saira/* $(b)/data/
	cp $(s)/fsemu/icons/fontawesome/* $(b)/data/
	cp $(s)/fsemu/images/* $(b)/data/
	cp $(s)/src/fsuae/images/* $(b)/data/
	touch data/.fsemu

fsemu-data: data/.fsemu

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
# fs_uae_device_helper_LDADD += libmanymouse.a
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
AM_CPPFLAGS += -I$(s)/glad/include
fs_uae_LDADD += libglad.a
fs_uae_device_helper_LDADD += libglad.a
libglad_a_SOURCES = \
	glad/src/glad.c
endif

if WITH_PEARPC_CPU
noinst_LIBRARIES += libpearpc.a
AM_CPPFLAGS += -I$(s)/src/ppc/pearpc
fs_uae_LDADD += libpearpc.a
libpearpc_a_SOURCES = \
	src/ppc/pearpc/cpu/common.h \
	src/ppc/pearpc/cpu/cpu.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_alu.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_alu.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_cpu.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_cpu.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_dec.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_dec.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_exc.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_exc.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_fpu.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_fpu.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_mmu.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_mmu.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_opc.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_opc.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_tools.h \
	src/ppc/pearpc/cpu/cpu_generic/ppc_vec.cpp \
	src/ppc/pearpc/cpu/cpu_generic/ppc_vec.h \
	src/ppc/pearpc/cpu/debug.h \
	src/ppc/pearpc/cpu/mem.h \
	src/ppc/pearpc/info.h \
	src/ppc/pearpc/io/io.h \
	src/ppc/pearpc/pearpc_config.h \
	src/ppc/pearpc/system/arch/generic/sysendian.h \
	src/ppc/pearpc/system/arch/generic/sysfeatures.h \
	src/ppc/pearpc/system/arch/sysendian.h \
	src/ppc/pearpc/system/arch/sysfeatures.h \
	src/ppc/pearpc/system/arch/x86/sysendian.h \
	src/ppc/pearpc/system/arch/x86/sysfeatures.h \
	src/ppc/pearpc/system/arch/x86_64/sysendian.h \
	src/ppc/pearpc/system/arch/x86_64/sysfeatures.h \
	src/ppc/pearpc/system/systhread.h \
	src/ppc/pearpc/system/types.h \
	src/ppc/pearpc/tools/debug.h \
	src/ppc/pearpc/tools/endianess.h \
	src/ppc/pearpc/tools/snprintf.h \
	src/ppc/pearpc/tracers.h \
	src/ppc/pearpc/uaeglue.cpp
endif

if WITH_CHD
noinst_LIBRARIES += libchd.a
# AM_CPPFLAGS += -I$(s)/src/ppc/pearpc
fs_uae_LDADD += libchd.a
libchd_a_SOURCES = \
	src/archivers/chd/astring.h \
	src/archivers/chd/bitstream.h \
	src/archivers/chd/chd.h \
	src/archivers/chd/chdcd.cpp \
	src/archivers/chd/chdcd.h \
	src/archivers/chd/chdcdrom.cpp \
	src/archivers/chd/chdcdrom.h \
	src/archivers/chd/chdcodec.h \
	src/archivers/chd/chdtypes.h \
	src/archivers/chd/corefile.h \
	src/archivers/chd/coretmpl.h \
	src/archivers/chd/flac.h \
	src/archivers/chd/hashing.h \
	src/archivers/chd/huffman.h \
	src/archivers/chd/md5.h \
	src/archivers/chd/osdcore.h \
	src/archivers/chd/sha1.h
endif

if WITH_X86
noinst_LIBRARIES += libpcem.a
# AM_CPPFLAGS += -I$(s)/src/ppc/pearpc
fs_uae_LDADD += libpcem.a
libpcem_a_SOURCES = \
src/pcem/386_common.h \
src/pcem/386.cpp \
src/pcem/386_dynarec.cpp \
src/pcem/386.h \
src/pcem/386_ops.h \
src/pcem/808x.cpp \
src/pcem/codegen.h \
src/pcem/codegen_x86-64.h \
src/pcem/codegen_x86.h \
src/pcem/config.h \
src/pcem/cpu.cpp \
src/pcem/cpu.h \
src/pcem/device.h \
src/pcem/dma.cpp \
src/pcem/dma.h \
src/pcem/dosbox/dbopl.cpp \
src/pcem/dosbox/dbopl.h \
src/pcem/dosbox/nukedopl.cpp \
src/pcem/dosbox/nukedopl.h \
src/pcem/fdc.h \
src/pcem/filters.h \
src/pcem/ibm.h \
src/pcem/io.h \
src/pcem/keyboard_at.cpp \
src/pcem/keyboard_at.h \
src/pcem/keyboard.cpp \
src/pcem/keyboard.h \
src/pcem/mca.h \
src/pcem/mem.cpp \
src/pcem/mem.h \
src/pcem/model.h \
src/pcem/mouse.h \
src/pcem/mouse_ps2.cpp \
src/pcem/mouse_ps2.h \
src/pcem/mouse_serial.cpp \
src/pcem/mouse_serial.h \
src/pcem/nmi.h \
src/pcem/nvr.h \
src/pcem/nvr_tc8521.h \
src/pcem/paths.h \
src/pcem/pcemglue.cpp \
src/pcem/pcemglue.h \
src/pcem/pcemnvr.cpp \
src/pcem/pcemrtc.cpp \
src/pcem/pci.h \
src/pcem/pic.cpp \
src/pcem/pic.h \
src/pcem/pit.cpp \
src/pcem/pit.h \
src/pcem/plat-keyboard.h \
src/pcem/plat-midi.h \
src/pcem/plat-mouse.h \
src/pcem/ps2_mca.h \
src/pcem/rom.h \
src/pcem/rtc.h \
src/pcem/serial.cpp \
src/pcem/serial.h \
src/pcem/sound_cms.cpp \
src/pcem/sound_cms.h \
src/pcem/sound_dbopl.cpp \
src/pcem/sound_dbopl.h \
src/pcem/sound_emu8k.h \
src/pcem/sound.h \
src/pcem/sound_mpu401_uart.cpp \
src/pcem/sound_mpu401_uart.h \
src/pcem/sound_opl.cpp \
src/pcem/sound_opl.h \
src/pcem/sound_sb.cpp \
src/pcem/sound_sb_dsp.cpp \
src/pcem/sound_sb_dsp.h \
src/pcem/sound_sb.h \
src/pcem/sound_speaker.cpp \
src/pcem/sound_speaker.h \
src/pcem/t1000.h \
src/pcem/t3100e.h \
src/pcem/timer.cpp \
src/pcem/timer.h \
src/pcem/video.h \
src/pcem/x86_flags.h \
src/pcem/x86.h \
src/pcem/x86_ops_arith.h \
src/pcem/x86_ops_atomic.h \
src/pcem/x86_ops_bcd.h \
src/pcem/x86_ops_bit.h \
src/pcem/x86_ops_bitscan.h \
src/pcem/x86_ops_call.h \
src/pcem/x86_ops_flag.h \
src/pcem/x86_ops_fpu.h \
src/pcem/x86_ops.h \
src/pcem/x86_ops_inc_dec.h \
src/pcem/x86_ops_int.h \
src/pcem/x86_ops_io.h \
src/pcem/x86_ops_jump.h \
src/pcem/x86_ops_misc.h \
src/pcem/x86_ops_mmx_arith.h \
src/pcem/x86_ops_mmx_cmp.h \
src/pcem/x86_ops_mmx.h \
src/pcem/x86_ops_mmx_logic.h \
src/pcem/x86_ops_mmx_mov.h \
src/pcem/x86_ops_mmx_pack.h \
src/pcem/x86_ops_mmx_shift.h \
src/pcem/x86_ops_mov_ctrl.h \
src/pcem/x86_ops_mov.h \
src/pcem/x86_ops_mov_seg.h \
src/pcem/x86_ops_movx.h \
src/pcem/x86_ops_msr.h \
src/pcem/x86_ops_mul.h \
src/pcem/x86_ops_pmode.h \
src/pcem/x86_ops_prefix.h \
src/pcem/x86_ops_rep.h \
src/pcem/x86_ops_ret.h \
src/pcem/x86_ops_set.h \
src/pcem/x86_ops_shift.h \
src/pcem/x86_ops_stack.h \
src/pcem/x86_ops_string.h \
src/pcem/x86_ops_xchg.h \
src/pcem/x86seg.cpp \
src/pcem/x87.cpp \
src/pcem/x87.h \
src/pcem/x87_ops_arith.h \
src/pcem/x87_ops.h \
src/pcem/x87_ops_loadstore.h \
src/pcem/x87_ops_misc.h \
src/pcem/xi8088.h
endif

if WITH_PPC
fs_uae_SOURCES += \
	src/ppc/ppcd.cpp \
	src/ppc/ppcd.h
endif

if WITH_SLIRP
if WITH_BUILTIN_SLIRP
noinst_LIBRARIES += libslirp.a
# AM_CPPFLAGS += -I$(s)/src/slirp/include
fs_uae_LDADD += libslirp.a
# libslirp_a_SOURCES = \
# 	src/slirp/bootp.cpp \
# 	src/slirp/bootp.h \
# 	src/slirp/cksum.cpp \
# 	src/slirp/ctl.h \
# 	src/slirp/debug.h \
# 	src/slirp/icmp_var.h \
# 	src/slirp/if.cpp \
# 	src/slirp/if.h \
# 	src/slirp/ip.h \
# 	src/slirp/ip_icmp.cpp \
# 	src/slirp/ip_icmp.h \
# 	src/slirp/ip_input.cpp \
# 	src/slirp/ip_output.cpp \
# 	src/slirp/libslirp.h \
# 	src/slirp/main.h \
# 	src/slirp/mbuf.cpp \
# 	src/slirp/mbuf.h \
# 	src/slirp/misc.cpp \
# 	src/slirp/misc.h \
# 	src/slirp/sbuf.cpp \
# 	src/slirp/sbuf.h \
# 	src/slirp/slirp.cpp \
# 	src/slirp/slirp.h \
# 	src/slirp/slirp_config.h \
# 	src/slirp/slirpdebug.cpp \
# 	src/slirp/socket.cpp \
# 	src/slirp/socket.h \
# 	src/slirp/tcp.h \
# 	src/slirp/tcp_input.cpp \
# 	src/slirp/tcp_output.cpp \
# 	src/slirp/tcp_subr.cpp \
# 	src/slirp/tcp_timer.cpp \
# 	src/slirp/tcp_timer.h \
# 	src/slirp/tcp_var.h \
# 	src/slirp/tcpip.h \
# 	src/slirp/tftp.cpp \
# 	src/slirp/tftp.h \
# 	src/slirp/udp.cpp \
# 	src/slirp/udp.h

libslirp_a_SOURCES = \
	slirp/src/arp_table.c \
	slirp/src/bootp.c \
	slirp/src/bootp.h \
	slirp/src/cksum.c \
	slirp/src/debug.h \
	slirp/src/dhcpv6.c \
	slirp/src/dhcpv6.h \
	slirp/src/dnssearch.c \
	slirp/src/if.c \
	slirp/src/if.h \
	slirp/src/ip6.h \
	slirp/src/ip6_icmp.c \
	slirp/src/ip6_icmp.h \
	slirp/src/ip6_input.c \
	slirp/src/ip6_output.c \
	slirp/src/ip.h \
	slirp/src/ip_icmp.c \
	slirp/src/ip_icmp.h \
	slirp/src/ip_input.c \
	slirp/src/ip_output.c \
	slirp/src/libslirp.h \
	slirp/src/libslirp-version.h \
	slirp/src/main.h \
	slirp/src/mbuf.c \
	slirp/src/mbuf.h \
	slirp/src/misc.c \
	slirp/src/misc.h \
	slirp/src/ncsi.c \
	slirp/src/ncsi-pkt.h \
	slirp/src/ndp_table.c \
	slirp/src/sbuf.c \
	slirp/src/sbuf.h \
	slirp/src/slirp.c \
	slirp/src/slirp.h \
	slirp/src/socket.c \
	slirp/src/socket.h \
	slirp/src/state.c \
	slirp/src/stream.c \
	slirp/src/stream.h \
	slirp/src/tcp.h \
	slirp/src/tcp_input.c \
	slirp/src/tcpip.h \
	slirp/src/tcp_output.c \
	slirp/src/tcp_subr.c \
	slirp/src/tcp_timer.c \
	slirp/src/tcp_timer.h \
	slirp/src/tcp_var.h \
	slirp/src/tftp.c \
	slirp/src/tftp.h \
	slirp/src/udp6.c \
	slirp/src/udp.c \
	slirp/src/udp.h \
	slirp/src/util.c \
	slirp/src/util.h \
	slirp/src/version.c \
	slirp/src/vmstate.c \
	slirp/src/vmstate.h

endif
endif

if WITH_TOCCATA
fs_uae_SOURCES += \
	src/sndboard.cpp
endif

if WITH_UDIS86
noinst_LIBRARIES += libudis86.a
AM_CPPFLAGS += -I$(s)/libudis86/include
fs_uae_LDADD += libudis86.a
libudis86_a_SOURCES = \
	libudis86/include/udis86.h \
	libudis86/src/decode.c \
	libudis86/src/decode.h \
	libudis86/src/extern.h \
	libudis86/src/itab.c \
	libudis86/src/itab.h \
	libudis86/src/syn-att.c \
	libudis86/src/syn-intel.c \
	libudis86/src/syn.c \
	libudis86/src/syn.h \
	libudis86/src/types.h \
	libudis86/src/udint.h \
	libudis86/src/udis86.c
endif

if WITH_LUA
noinst_LIBRARIES += liblua.a
AM_CPPFLAGS += -I$(s)/lua/src
fs_uae_LDADD += liblua.a
liblua_a_SOURCES = \
	lua/src/lapi.c \
	lua/src/lapi.h \
	lua/src/lauxlib.c \
	lua/src/lauxlib.h \
	lua/src/lbaselib.c \
	lua/src/lbitlib.c \
	lua/src/lcode.c \
	lua/src/lcode.h \
	lua/src/lcorolib.c \
	lua/src/lctype.c \
	lua/src/lctype.h \
	lua/src/ldblib.c \
	lua/src/ldebug.c \
	lua/src/ldebug.h \
	lua/src/ldo.c \
	lua/src/ldo.h \
	lua/src/ldump.c \
	lua/src/lfunc.c \
	lua/src/lfunc.h \
	lua/src/lgc.c \
	lua/src/lgc.h \
	lua/src/linit.c \
	lua/src/liolib.c \
	lua/src/llex.c \
	lua/src/llex.h \
	lua/src/llimits.h \
	lua/src/lmathlib.c \
	lua/src/lmem.c \
	lua/src/lmem.h \
	lua/src/loadlib.c \
	lua/src/lobject.c \
	lua/src/lobject.h \
	lua/src/lopcodes.c \
	lua/src/lopcodes.h \
	lua/src/loslib.c \
	lua/src/lparser.c \
	lua/src/lparser.h \
	lua/src/lstate.c \
	lua/src/lstate.h \
	lua/src/lstring.c \
	lua/src/lstring.h \
	lua/src/lstrlib.c \
	lua/src/ltable.c \
	lua/src/ltable.h \
	lua/src/ltablib.c \
	lua/src/ltm.c \
	lua/src/ltm.h \
	lua/src/lua.h \
	lua/src/luaconf.h \
	lua/src/lualib.h \
	lua/src/lundump.c \
	lua/src/lundump.h \
	lua/src/lvm.c \
	lua/src/lvm.h \
	lua/src/lzio.c \
	lua/src/lzio.h
fs_uae_SOURCES += \
	src/fsuae/fsuae-luascript.c \
	src/luascript.cpp
libfsemu_a_SOURCES += \
	libfsemu/src/emu/emu_lua.c
endif

if PROWIZARD
noinst_LIBRARIES += libprowizard.a
AM_CPPFLAGS += -I$(s)/prowizard/include
fs_uae_LDADD += libprowizard.a
fs_uae_SOURCES += src/moduleripper.cpp
libprowizard_a_SOURCES = \
	prowizard/include/extern.h \
	prowizard/include/globals.h \
	prowizard/include/ptktable.h \
	prowizard/include/tuning.h \
	prowizard/include/vars.h \
	prowizard/misc/misc.c \
	prowizard/misc/testbag.c \
	prowizard/prowiz.c \
	prowizard/r/AC1D_packer.c \
	prowizard/r/AMOS-MusicBank.c \
	prowizard/r/BP-Soundmonitor.c \
	prowizard/r/BSI-FutureComposer.c \
	prowizard/r/ChipTracker.c \
	prowizard/r/DigiBooster17.c \
	prowizard/r/DigitalIllusion.c \
	prowizard/r/EurekaPacker.c \
	prowizard/r/FastTracker2.c \
	prowizard/r/FC-M-Packer.c \
	prowizard/r/FuchsTracker.c \
	prowizard/r/FutureComposer13.c \
	prowizard/r/FutureComposer14.c \
	prowizard/r/FuzzacPacker.c \
	prowizard/r/GMC.c \
	prowizard/r/GnuPlayer.c \
	prowizard/r/GPMO.c \
	prowizard/r/HeatseekerMC10.c \
	prowizard/r/HornetPacker.c \
	prowizard/r/JamCracker.c \
	prowizard/r/KefrensSoundMachine.c \
	prowizard/r/MED-Octamed.c \
	prowizard/r/MOD-compatible.c \
	prowizard/r/ModuleProtector.c \
	prowizard/r/Mugician.c \
	prowizard/r/Newtron.c \
	prowizard/r/NewtronOld.c \
	prowizard/r/NoiseFromHeaven.c \
	prowizard/r/NoisePacker1.c \
	prowizard/r/NoisePacker2.c \
	prowizard/r/NoiseRunner.c \
	prowizard/r/NovoTrade.c \
	prowizard/r/NP3.c \
	prowizard/r/PhaPacker.c \
	prowizard/r/PolkaPacker.c \
	prowizard/r/PowerMusic.c \
	prowizard/r/Promizer01.c \
	prowizard/r/Promizer10c.c \
	prowizard/r/Promizer18a.c \
	prowizard/r/Promizer20.c \
	prowizard/r/Promizer40.c \
	prowizard/r/ProPacker10.c \
	prowizard/r/ProPacker21.c \
	prowizard/r/ProPacker30.c \
	prowizard/r/Prorunner10.c \
	prowizard/r/Prorunner20.c \
	prowizard/r/QuadraComposer.c \
	prowizard/r/SGT-Packer.c \
	prowizard/r/skizzo.c \
	prowizard/r/SkytPacker.c \
	prowizard/r/SoundFX.c \
	prowizard/r/SoundTracker26.c \
	prowizard/r/SoundTracker.c \
	prowizard/r/StartrekkerPack.c \
	prowizard/r/StimPacker.c \
	prowizard/r/StoneArtsPlayer.c \
	prowizard/r/TheDarkDemon.c \
	prowizard/r/ThePlayer22a.c \
	prowizard/r/ThePlayer30a.c \
	prowizard/r/ThePlayer40.c \
	prowizard/r/ThePlayer41a.c \
	prowizard/r/ThePlayer50a.c \
	prowizard/r/ThePlayer60a.c \
	prowizard/r/ThePlayer61a.c \
	prowizard/r/TitanicsPlayer.c \
	prowizard/r/TMK.c \
	prowizard/r/TrackerPacker1.c \
	prowizard/r/TrackerPacker2.c \
	prowizard/r/TrackerPacker3.c \
	prowizard/r/UnicTracker2.c \
	prowizard/r/UnicTracker.c \
	prowizard/r/Viruz2.c \
	prowizard/r/WantonPacker.c \
	prowizard/r/XannPlayer.c \
	prowizard/r/ZenPacker.c
endif

#if WITH_SOFTFLOAT
#noinst_LIBRARIES += libsoftfloat.a
#AM_CPPFLAGS += -I$(s)/softfloat
#fs_uae_LDADD += libsoftfloat.a
#libsoftfloat_a_SOURCES = \
#	softfloat/softfloat-uae.h \
#	softfloat/fprem.cc \
#	softfloat/fpu_constant.h \
#	softfloat/fsincos.cc \
#	softfloat/poly.cc \
#	softfloat/softfloat16.cc \
#	softfloat/softfloat.cc \
#	softfloat/softfloat-compare.h \
#	softfloat/softfloat.h \
#	softfloat/softfloat-macros.h \
#	softfloat/softfloat-muladd.cc \
#	softfloat/softfloat-round-pack.cc \
#	softfloat/softfloat-round-pack.h \
#	softfloat/softfloat-specialize.cc \
#	softfloat/softfloat-specialize.h \
#	softfloat/softfloatx80.cc \
#	softfloat/softfloatx80.h
#endif

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

gen_build68k_SOURCES = \
	src/build68k.cpp \
	src/writelog.cpp

gen_genblitter_SOURCES = \
	src/blitops.cpp \
	src/genblitter.cpp \
	src/writelog.cpp

gen_gencomp_SOURCES = \
	src/jit/gencomp.cpp \
	src/missing.cpp \
	src/readcpu.cpp

nodist_gen_gencomp_SOURCES = \
	gen/cpudefs.cpp

gen_gencpu_SOURCES = \
	src/gencpu.cpp \
	src/missing.cpp \
	src/readcpu.cpp

nodist_gen_gencpu_SOURCES = \
	gen/cpudefs.cpp

gen_genlinetoscr_SOURCES = \
	src/genlinetoscr.cpp

TESTS = \
	tests/dummy-test

EXTRA_TESTS = \
	tests/cppcheck-fs-uae \
	tests/cppcheck-libfsemu \
	tests/cppcheck-libmpeg2 \
	tests/cppcheck-lua \
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
	flac/include/FLAC/all.h \
	flac/include/FLAC/assert.h \
	flac/include/FLAC/callback.h \
	flac/include/FLAC/export.h \
	flac/include/FLAC/format.h \
	flac/include/FLAC/metadata.h \
	flac/include/FLAC/ordinals.h \
	flac/include/FLAC/stream_decoder.h \
	flac/include/FLAC/stream_encoder.h \
	fsemu/gamemode/LICENSE.txt \
	fsemu/gamemode/README.md \
	fsemu/libinih/ini.c \
	fsemu/libinih/ini.h \
	fsemu/libinih/LICENSE.txt \
	fs-uae.spec \
	icon/fs-uae.ico \
	glad/include/glad/glad.h \
	glad/include/KHR/khrplatform.h \
	glad/LICENSE \
	libudis86/README \
	licenses/fs-uae.txt \
	licenses/glib.txt \
	licenses/ipflib.txt \
	licenses/libgcc.txt \
	licenses/libintl.txt \
	licenses/libpng.txt \
	licenses/libstdc++.txt \
	licenses/lua.txt \
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

gen/blit.h: gen/genblitter$(EXEEXT)
	$(b)/gen/genblitter$(EXEEXT) i > $(b)/gen/blit.h

gen/blitfunc.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
	$(b)/gen/genblitter$(EXEEXT) f > $(b)/gen/blitfunc.cpp

gen/blitfunc.h: gen/genblitter$(EXEEXT)
	$(b)/gen/genblitter$(EXEEXT) h > $(b)/gen/blitfunc.h

gen/blittable.cpp: gen/genblitter$(EXEEXT) gen/blitfunc.h
	$(b)/gen/genblitter$(EXEEXT) t > $(b)/gen/blittable.cpp

gen/compemu.cpp: gen/gencomp$(EXEEXT)
	$(b)/gen/gencomp$(EXEEXT)

gen/compstbl.cpp: gen/compemu.cpp

gen/comptbl.h: gen/compemu.cpp

gen/cpudefs.cpp: gen/build68k$(EXEEXT) src/table68k
	$(b)/gen/build68k$(EXEEXT) < $(s)/src/table68k > $(b)/gen/cpudefs.cpp

gen/cpuemu_0.cpp: gen/gencpu$(EXEEXT)
	cd $(b)/gen && ./gencpu$(EXEEXT) --optimized-flags

gen/cpuemu_11.cpp: gen/cpuemu_0.cpp

gen/cpuemu_13.cpp: gen/cpuemu_0.cpp

gen/cpuemu_20.cpp: gen/cpuemu_0.cpp

gen/cpuemu_21.cpp: gen/cpuemu_0.cpp

gen/cpuemu_22.cpp: gen/cpuemu_0.cpp

gen/cpuemu_23.cpp: gen/cpuemu_0.cpp

gen/cpuemu_24.cpp: gen/cpuemu_0.cpp

gen/cpuemu_31.cpp: gen/cpuemu_0.cpp

gen/cpuemu_32.cpp: gen/cpuemu_0.cpp

gen/cpuemu_33.cpp: gen/cpuemu_0.cpp

gen/cpuemu_34.cpp: gen/cpuemu_0.cpp

gen/cpuemu_35.cpp: gen/cpuemu_0.cpp

gen/cpuemu_40.cpp: gen/cpuemu_0.cpp

gen/cpuemu_50.cpp: gen/cpuemu_0.cpp

gen/cpustbl.cpp: gen/cpuemu_0.cpp

gen/cputbl.h: gen/cpuemu_0.cpp

gen/linetoscr.cpp: gen/genlinetoscr$(EXEEXT)
	$(b)/gen/genlinetoscr$(EXEEXT) > $(b)/gen/linetoscr.cpp

endif

catalogs = \
	share/locale/cs/LC_MESSAGES/fs-uae.mo \
	share/locale/da/LC_MESSAGES/fs-uae.mo \
	share/locale/de/LC_MESSAGES/fs-uae.mo \
	share/locale/eo/LC_MESSAGES/fs-uae.mo \
	share/locale/es/LC_MESSAGES/fs-uae.mo \
	share/locale/fi/LC_MESSAGES/fs-uae.mo \
	share/locale/fr/LC_MESSAGES/fs-uae.mo \
	share/locale/hu/LC_MESSAGES/fs-uae.mo \
	share/locale/it/LC_MESSAGES/fs-uae.mo \
	share/locale/nb/LC_MESSAGES/fs-uae.mo \
	share/locale/nl/LC_MESSAGES/fs-uae.mo \
	share/locale/pl/LC_MESSAGES/fs-uae.mo \
	share/locale/pt/LC_MESSAGES/fs-uae.mo \
	share/locale/pt_BR/LC_MESSAGES/fs-uae.mo \
	share/locale/sr/LC_MESSAGES/fs-uae.mo \
	share/locale/sv/LC_MESSAGES/fs-uae.mo \
	share/locale/tr/LC_MESSAGES/fs-uae.mo

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
	rm -f fs-uae.dat
	for d in $(s) $(b) ; do \
	  ( cd $$d && $(ZIP) -X $(abs_builddir)/fs-uae.dat $$(find share | LC_ALL=C sort ) ) ; \
	done

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
	find share -name fs-uae.mo -delete

.PHONY: fsemu-data
