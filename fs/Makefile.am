# Makefile.am for FS-UAE

SUBDIRS =

WARNINGS =

CM_CFLAGS =

# FIXME: DEBUG BUILDS ONLY
CM_CFLAGS += -DDEBUG=1

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

dist_doc_DATA = README.md COPYING

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

AM_CPPFLAGS += -I$(s)/src/fsapp
AM_CPPFLAGS += -I$(s)/src/fsgui
AM_CPPFLAGS += -I$(s)/src/fslib
AM_CPPFLAGS += -I$(s)/src/fsuae
fs_uae_SOURCES = \
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

noinst_LIBRARIES += .libs/libuae.a
AM_CPPFLAGS += -I$(b)/gen
AM_CPPFLAGS += -I$(s)/include
AM_CPPFLAGS += -I$(s)/od-fs
AM_CPPFLAGS += -I$(s)/od-fs/include
AM_CPPFLAGS += -I$(s)/src/flac/include
AM_CPPFLAGS += -I$(s)/..
AM_CPPFLAGS += -I$(s)/../include
AM_CPPFLAGS += -I$(s)/../jit
AM_CPPFLAGS += -I$(s)/../od-win32
AM_CPPFLAGS += -I$(s)/../od-win32/caps
fs_uae_LDADD += src/libuae.a
# libuae_a_CXXFLAGS = $(CM_CFLAGS) -fpermissive
_libs_libuae_a_SOURCES = \
	../a2065.cpp \
	../a2091.cpp \
	../akiko.cpp \
	../amax.cpp \
	../ar.cpp \
	../arcadia.cpp \
	../archivers/dms/cdata.h \
	../archivers/dms/crc_csum.cpp \
	../archivers/dms/crc_csum.h \
	../archivers/dms/getbits.cpp \
	../archivers/dms/getbits.h \
	../archivers/dms/maketbl.cpp \
	../archivers/dms/maketbl.h \
	../archivers/dms/pfile.cpp \
	../archivers/dms/pfile.h \
	../archivers/dms/tables.cpp \
	../archivers/dms/tables.h \
	../archivers/dms/u_deep.cpp \
	../archivers/dms/u_deep.h \
	../archivers/dms/u_heavy.cpp \
	../archivers/dms/u_heavy.h \
	../archivers/dms/u_init.cpp \
	../archivers/dms/u_init.h \
	../archivers/dms/u_medium.cpp \
	../archivers/dms/u_medium.h \
	../archivers/dms/u_quick.cpp \
	../archivers/dms/u_quick.h \
	../archivers/dms/u_rle.cpp \
	../archivers/dms/u_rle.h \
	../archivers/mp2/kjmp2.cpp \
	../archivers/mp2/kjmp2.h \
	../archivers/wrp/warp.h \
	../archivers/zip/unzip.cpp \
	../archivers/zip/unzip.h \
	../audio.cpp \
	../autoconf.cpp \
	../blitter.cpp \
	../blkdev.cpp \
	../blkdev_cdimage.cpp \
	../bsdsocket.cpp \
	../calc.cpp \
	../cd32_fmv.cpp \
	../cd32_fmv_genlock.cpp \
	../cdrom.cpp \
	../cdtv.cpp \
	../cdtvcr.cpp \
	../cfgfile.cpp \
	../cia.cpp \
	../consolehook.cpp \
	../cpuboard.cpp \
	../cpummu.cpp \
	../cpummu30.cpp \
	../crc32.cpp \
	../custom.cpp \
	../debug.cpp \
	../debuginfo.cpp \
	../debugmem.cpp \
	../def_icons.cpp \
	../devices.cpp \
	../disk.cpp \
	../diskutil.cpp \
	../dlopen.cpp \
	../dongle.cpp \
	../drawing.cpp \
	../driveclick.cpp \
	../enforcer.cpp \
	../ethernet.cpp \
	../events.cpp \
	../expansion.cpp \
	../fake86_cpu.cpp \
	../fdi2raw.cpp \
	../filesys.cpp \
	../flashrom.cpp \
	../fpp.cpp \
	../fpp_native.cpp \
	../fpp_softfloat.cpp \
	../framebufferboards.cpp \
	../fsdb.cpp \
	../fsusage.cpp \
	../gayle.cpp \
	../gfxboard.cpp \
	../gfxutil.cpp \
	../hardfile.cpp \
	../hrtmon.rom.cpp \
	../ide.cpp \
	../idecontrollers.cpp \
	../identify.cpp \
	../include/a2065.h \
	../include/a2091.h \
	../include/akiko.h \
	../include/amax.h \
	../include/ar.h \
	../include/arcadia.h \
	../include/audio.h \
	../include/autoconf.h \
	../include/blitter.h \
	../include/blkdev.h \
	../include/bsdsocket.h \
	../include/calc.h \
	../include/casablanca.h \
	../include/catweasel.h \
	../include/cd32_fmv.h \
	../include/cdtv.h \
	../include/cdtvcr.h \
	../include/cia.h \
	../include/clipboard.h \
	../include/commpipe.h \
	../include/consolehook.h \
	../include/cpu_prefetch.h \
	../include/cpuboard.h \
	../include/cpummu.h \
	../include/cpummu030.h \
	../include/crc32.h \
	../include/custom.h \
	../include/debug.h \
	../include/debugmem.h \
	../include/devices.h \
	../include/disk.h \
	../include/diskutil.h \
	../include/dongle.h \
	../include/drawing.h \
	../include/driveclick.h \
	../include/enforcer.h \
	../include/epsonprinter.h \
	../include/ersatz.h \
	../include/ethernet.h \
	../include/events.h \
	../include/execio.h \
	../include/execlib.h \
	../include/fake86_cpu.h \
	../include/fdi2raw.h \
	../include/filesys.h \
	../include/flashrom.h \
	../include/fpp.h \
	../include/fpp-ieee-be.h \
	../include/fpp-unknown.h \
	../include/fsdb.h \
	../include/fsusage.h \
	../include/framebufferboards.h \
	../include/gayle.h \
	../include/genblitter.h \
	../include/gensound.h \
	../include/gfxboard.h \
	../include/gfxfilter.h \
	../include/gui.h \
	../include/ide.h \
	../include/idecontrollers.h \
	../include/identify.h \
	../include/ini.h \
	../include/inputdevice.h \
	../include/inputrecord.h \
	../include/isofs.h \
	../include/isofs_api.h \
	../include/keyboard.h \
	../include/keybuf.h \
	../include/luascript.h \
	../include/mackbd.h \
	../include/mmu_common.h \
	../include/moduleripper.h \
	../include/native2amiga.h \
	../include/native2amiga_api.h \
	../include/ncr9x_scsi.h \
	../include/ncr_scsi.h \
	../include/newcpu.h \
	../include/options.h \
	../include/osemu.h \
	../include/packed.h \
	../include/parallel.h \
	../include/pci.h \
	../include/pci_hw.h \
	../include/picasso96.h \
	../include/readcpu.h \
	../include/rommgr.h \
	../include/rtc.h \
	../include/rtgmodes.h \
	../include/sampler.h \
	../include/sana2.h \
	../include/savestate.h \
	../include/scp.h \
	../include/scsi.h \
	../include/scsidev.h \
	../include/serial.h \
	../include/sndboard.h \
	../include/specialmonitors.h \
	../include/statusline.h \
	../include/sysdeps.h \
	../include/tabletlibrary.h \
	../include/traps.h \
	../include/uae.h \
	../include/uae/ahi.h \
	../include/uae/api.h \
	../include/uae/asm.h \
	../include/uae/assert.h \
	../include/uae/atomic.h \
	../include/uae/attributes.h \
	../include/uae/byteswap.h \
	../include/uae/caps.h \
	../include/uae/cdrom.h \
	../include/uae/cycleunit.h \
	../include/uae/debuginfo.h \
	../include/uae/dlopen.h \
	../include/uae/endian.h \
	../include/uae/enum.h \
	../include/uae/fs.h \
	../include/uae/funcattr.h \
	../include/uae/glib.h \
	../include/uae/inline.h \
	../include/uae/io.h \
	../include/uae/jitconfig.h \
	../include/uae/likely.h \
	../include/uae/limits.h \
	../include/uae/log.h \
	../include/uae/maccess.h \
	../include/uae/memory.h \
	../include/uae/mman.h \
	../include/uae/parser.h \
	../include/uae/patch.h \
	../include/uae/ppc.h \
	../include/uae/qemu.h \
	../include/uae/segtracker.h \
	../include/uae/seh.h \
	../include/uae/slirp.h \
	../include/uae/socket.h \
	../include/uae/string.h \
	../include/uae/time.h \
	../include/uae/types.h \
	../include/uae/vm.h \
	../include/uaeexe.h \
	../include/uaeipc.h \
	../include/uaenative.h \
	../include/uaeresource.h \
	../include/uaeserial.h \
	../include/uni_common.h \
	../include/unpacked.h \
	../include/videograb.h \
	../include/x86.h \
	../include/xwin.h \
	../include/zarchive.h \
	../include/zfile.h \
	../ini.cpp \
	../inputdevice.cpp \
	../isofs.cpp \
	../jit/codegen_udis86.h \
	../jit/codegen_x86.h \
	../jit/compemu.h \
	../jit/compemu_fpp.cpp \
	../jit/compemu_midfunc_arm.h \
	../jit/compemu_midfunc_x86.h \
	../jit/compemu_support.cpp \
	../jit/flags_x86.h \
	../keybuf.cpp \
	../logging.cpp \
	../main.cpp \
	../mame/a2410.cpp \
	../mame/mameglue.h \
	../mame/tm34010/tms34010.cpp \
	../mame/tm34010/tms34010.h \
	../memory.cpp \
	../native2amiga.cpp \
	../ncr9x_scsi.cpp \
	../ncr_scsi.cpp \
	../newcpu.cpp \
	../newcpu_common.cpp \
	od-fs/ahi_winuae.cpp \
	od-fs/ahidsound.h \
	od-fs/ahidsound_new.cpp \
	od-fs/ahidsound_new.h \
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
	od-fs/include/uae/uae.h \
	od-fs/include/uae/uae_inputevents_def.h \
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
	od-fs/roms.cpp \
	od-fs/sleep.h \
	od-fs/sound_fs.cpp \
	od-fs/spin.cpp \
	od-fs/sounddep/sound.h \
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
	../od-win32/bsdsock.cpp \
	../od-win32/caps/CapsAPI.h \
	../od-win32/caps/Comtype.h \
	../od-win32/caps/caps_win32.cpp \
	../od-win32/caps/caps_win32.h \
	../od-win32/clipboard_win32.cpp \
	../od-win32/clipboard_win32.h \
	../od-win32/debug_win32.h \
	../od-win32/dxwrap.h \
	../od-win32/md-fpp.h \
	../od-win32/midi.h \
	../od-win32/parser.h \
	../od-win32/picasso96_win.cpp \
	../od-win32/picasso96_win.h \
	../od-win32/serial_win32.cpp \
	../od-win32/win32.h \
	../od-win32/win32gfx.h \
	../od-win32/win32_uaenet.cpp \
	../patch.cpp \
	../pcem/codegen.h \
	../pcem/codegen_x86-64.h \
	../pcem/cpu.h \
	../pcem/dma.h \
	../pcem/ibm.h \
	../pcem/keyboard.h \
	../pcem/keyboard_at.h \
	../pcem/mem.h \
	../pcem/model.h \
	../pcem/mouse.h \
	../pcem/mouse_serial.h \
	../pcem/nvr.h \
	../pcem/pcemglue.h \
	../pcem/pic.h \
	../pcem/pit.h \
	../pcem/sound.h \
	../pcem/sound_cms.h \
	../pcem/sound_speaker.h \
	../pcem/timer.h \
	../pcem/x86.h \
	../pcem/x86_ops.h \
	../pci.cpp \
	../ppc/pixman-stubs.cpp \
	../ppc/ppc.cpp \
	../qemu.cpp \
	../qemuvga/cirrus_vga.cpp \
	../qemuvga/cirrus_vga_rop.h \
	../qemuvga/cirrus_vga_rop2.h \
	../qemuvga/cirrus_vga_template.h \
	../qemuvga/es1370.cpp \
	../qemuvga/esp.cpp \
	../qemuvga/lsi53c710.cpp \
	../qemuvga/lsi53c895a.cpp \
	../qemuvga/ne2000.cpp \
	../qemuvga/ne2000.h \
	../qemuvga/pixel_ops.h \
	../qemuvga/qemuaudio.h \
	../qemuvga/qemumemory.h \
	../qemuvga/qemuuaeglue.cpp \
	../qemuvga/qemuuaeglue.h \
	../qemuvga/queue.h \
	../qemuvga/scsi/esp.h \
	../qemuvga/scsi/scsi.h \
	../qemuvga/vga.cpp \
	../qemuvga/vga.h \
	../qemuvga/vga_int.h \
	../qemuvga/vga_template.h \
	../random.cpp \
	../readcpu.cpp \
	../rommgr.cpp \
	../rtc.cpp \
	../sana2.cpp \
	../savestate.cpp \
	../scp.cpp \
	../scsi.cpp \
	../scsiemul.cpp \
	../scsitape.cpp \
	../segtracker.cpp \
	../slirp_uae.cpp \
	../softfloat/softfloat.cpp \
	../softfloat/softfloat_decimal.cpp \
	../softfloat/softfloat_fpsp.cpp \
	../softfloat/softfloat_fpsp_tables.h \
	../softfloat/softfloat.h \
	../softfloat/softfloat-macros.h \
	../softfloat/softfloat-specialize.h \
	../softfloat/softfloat.cpp \
	../softfloat/softfloat_decimal.cpp \
	../softfloat/softfloat_fpsp.cpp \
	../specialmonitors.cpp \
	../statusline.cpp \
	../support/socket.cpp \
	../support/time.cpp \
	../test_card.cpp \
	../traps.cpp \
	../uaeexe.cpp \
	../uaelib.cpp \
	../uaenative.cpp \
	../uaeresource.cpp \
	../uaeserial.cpp \
	../vm.cpp \
	../x86.cpp \
	../zfile.cpp \
	../zfile_archive.cpp
nodist__libs_libuae_a_SOURCES = \
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
BUILT_SOURCES += $(nodist__libs_libuae_a_SOURCES)

noinst_LIBRARIES += src/libcpuemu.a
fs_uae_LDADD += src/libcpuemu.a
nodist_src_libcpuemu_a_SOURCES = \
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
BUILT_SOURCES += $(nodist_src_libcpuemu_a_SOURCES)

noinst_LIBRARIES += src/libfsemu/libfsemu.a
AM_CPPFLAGS += -I$(s)/libfsemu/include
fs_uae_LDADD += src/libfsemu/libfsemu.a
src_libfsemu_libfsemu_a_SOURCES = \
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

noinst_LIBRARIES += src/fsemu/libfsemu.a
AM_CPPFLAGS += -I$(s)/fsemu
fs_uae_LDADD += src/fsemu/libfsemu.a
src_fsemu_libfsemu_a_SOURCES = \
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
src_fsemu_libfsemu_a_SOURCES += $(fsemu_data_files)

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
noinst_LIBRARIES += src/glad/libglad.a
AM_CPPFLAGS += -I$(s)/src/glad/include
fs_uae_LDADD += src/glad/libglad.a
src_glad_libglad_a_SOURCES = \
	src/glad/src/glad.c
endif

if WITH_PEARPC_CPU
noinst_LIBRARIES += src/ppc/libpearpc.a
AM_CPPFLAGS += -I$(s)/src/ppc/pearpc
fs_uae_LDADD += src/ppc/libpearpc.a
src_ppc_libpearpc_a_SOURCES = \
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
noinst_LIBRARIES += src/archivers/chd/libchd.a
fs_uae_LDADD +=  src/archivers/chd/libchd.a
src_archivers_chd_libchd_a_SOURCES = \
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
noinst_LIBRARIES += src/pcem/libpcem.a
fs_uae_LDADD += src/pcem/libpcem.a
src_pcem_libpcem_a_SOURCES = \
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
noinst_LIBRARIES += src/slirp/libslirp.a
fs_uae_LDADD += src/slirp/libslirp.a
LIBS += @SLIRP_LIBS@
src_slirp_libslirp_a_SOURCES = \
	src/slirp/src/arp_table.c \
	src/slirp/src/bootp.c \
	src/slirp/src/bootp.h \
	src/slirp/src/cksum.c \
	src/slirp/src/debug.h \
	src/slirp/src/dhcpv6.c \
	src/slirp/src/dhcpv6.h \
	src/slirp/src/dnssearch.c \
	src/slirp/src/if.c \
	src/slirp/src/if.h \
	src/slirp/src/ip6.h \
	src/slirp/src/ip6_icmp.c \
	src/slirp/src/ip6_icmp.h \
	src/slirp/src/ip6_input.c \
	src/slirp/src/ip6_output.c \
	src/slirp/src/ip.h \
	src/slirp/src/ip_icmp.c \
	src/slirp/src/ip_icmp.h \
	src/slirp/src/ip_input.c \
	src/slirp/src/ip_output.c \
	src/slirp/src/libslirp.h \
	src/slirp/src/libslirp-version.h \
	src/slirp/src/main.h \
	src/slirp/src/mbuf.c \
	src/slirp/src/mbuf.h \
	src/slirp/src/misc.c \
	src/slirp/src/misc.h \
	src/slirp/src/ncsi.c \
	src/slirp/src/ncsi-pkt.h \
	src/slirp/src/ndp_table.c \
	src/slirp/src/sbuf.c \
	src/slirp/src/sbuf.h \
	src/slirp/src/slirp.c \
	src/slirp/src/slirp.h \
	src/slirp/src/socket.c \
	src/slirp/src/socket.h \
	src/slirp/src/state.c \
	src/slirp/src/stream.c \
	src/slirp/src/stream.h \
	src/slirp/src/tcp.h \
	src/slirp/src/tcp_input.c \
	src/slirp/src/tcpip.h \
	src/slirp/src/tcp_output.c \
	src/slirp/src/tcp_subr.c \
	src/slirp/src/tcp_timer.c \
	src/slirp/src/tcp_timer.h \
	src/slirp/src/tcp_var.h \
	src/slirp/src/tftp.c \
	src/slirp/src/tftp.h \
	src/slirp/src/udp6.c \
	src/slirp/src/udp.c \
	src/slirp/src/udp.h \
	src/slirp/src/util.c \
	src/slirp/src/util.h \
	src/slirp/src/version.c \
	src/slirp/src/vmstate.c \
	src/slirp/src/vmstate.h
endif
endif

if WITH_TOCCATA
fs_uae_SOURCES += \
	src/sndboard.cpp
endif

if WITH_UDIS86
noinst_LIBRARIES += src/libudis86/libudis86.a
AM_CPPFLAGS += -I$(s)/src/libudis86/include
fs_uae_LDADD += src/libudis86/libudis86.a
src_libudis86_libudis86_a_SOURCES = \
	src/libudis86/include/udis86.h \
	src/libudis86/src/decode.c \
	src/libudis86/src/decode.h \
	src/libudis86/src/extern.h \
	src/libudis86/src/itab.c \
	src/libudis86/src/itab.h \
	src/libudis86/src/syn-att.c \
	src/libudis86/src/syn-intel.c \
	src/libudis86/src/syn.c \
	src/libudis86/src/syn.h \
	src/libudis86/src/types.h \
	src/libudis86/src/udint.h \
	src/libudis86/src/udis86.c
endif

if PROWIZARD
noinst_LIBRARIES += src/prowizard/libprowizard.a
AM_CPPFLAGS += -I$(s)/src/prowizard/include
fs_uae_LDADD += src/prowizard/libprowizard.a
fs_uae_SOURCES += src/moduleripper.cpp
src_prowizard_libprowizard_a_SOURCES = \
	src/prowizard/include/extern.h \
	src/prowizard/include/globals.h \
	src/prowizard/include/ptktable.h \
	src/prowizard/include/tuning.h \
	src/prowizard/include/vars.h \
	src/prowizard/misc/misc.c \
	src/prowizard/misc/testbag.c \
	src/prowizard/prowiz.c \
	src/prowizard/r/AC1D_packer.c \
	src/prowizard/r/AMOS-MusicBank.c \
	src/prowizard/r/BP-Soundmonitor.c \
	src/prowizard/r/BSI-FutureComposer.c \
	src/prowizard/r/ChipTracker.c \
	src/prowizard/r/DigiBooster17.c \
	src/prowizard/r/DigitalIllusion.c \
	src/prowizard/r/EurekaPacker.c \
	src/prowizard/r/FastTracker2.c \
	src/prowizard/r/FC-M-Packer.c \
	src/prowizard/r/FuchsTracker.c \
	src/prowizard/r/FutureComposer13.c \
	src/prowizard/r/FutureComposer14.c \
	src/prowizard/r/FuzzacPacker.c \
	src/prowizard/r/GMC.c \
	src/prowizard/r/GnuPlayer.c \
	src/prowizard/r/GPMO.c \
	src/prowizard/r/HeatseekerMC10.c \
	src/prowizard/r/HornetPacker.c \
	src/prowizard/r/JamCracker.c \
	src/prowizard/r/KefrensSoundMachine.c \
	src/prowizard/r/MED-Octamed.c \
	src/prowizard/r/MOD-compatible.c \
	src/prowizard/r/ModuleProtector.c \
	src/prowizard/r/Mugician.c \
	src/prowizard/r/Newtron.c \
	src/prowizard/r/NewtronOld.c \
	src/prowizard/r/NoiseFromHeaven.c \
	src/prowizard/r/NoisePacker1.c \
	src/prowizard/r/NoisePacker2.c \
	src/prowizard/r/NoiseRunner.c \
	src/prowizard/r/NovoTrade.c \
	src/prowizard/r/NP3.c \
	src/prowizard/r/PhaPacker.c \
	src/prowizard/r/PolkaPacker.c \
	src/prowizard/r/PowerMusic.c \
	src/prowizard/r/Promizer01.c \
	src/prowizard/r/Promizer10c.c \
	src/prowizard/r/Promizer18a.c \
	src/prowizard/r/Promizer20.c \
	src/prowizard/r/Promizer40.c \
	src/prowizard/r/ProPacker10.c \
	src/prowizard/r/ProPacker21.c \
	src/prowizard/r/ProPacker30.c \
	src/prowizard/r/Prorunner10.c \
	src/prowizard/r/Prorunner20.c \
	src/prowizard/r/QuadraComposer.c \
	src/prowizard/r/SGT-Packer.c \
	src/prowizard/r/skizzo.c \
	src/prowizard/r/SkytPacker.c \
	src/prowizard/r/SoundFX.c \
	src/prowizard/r/SoundTracker26.c \
	src/prowizard/r/SoundTracker.c \
	src/prowizard/r/StartrekkerPack.c \
	src/prowizard/r/StimPacker.c \
	src/prowizard/r/StoneArtsPlayer.c \
	src/prowizard/r/TheDarkDemon.c \
	src/prowizard/r/ThePlayer22a.c \
	src/prowizard/r/ThePlayer30a.c \
	src/prowizard/r/ThePlayer40.c \
	src/prowizard/r/ThePlayer41a.c \
	src/prowizard/r/ThePlayer50a.c \
	src/prowizard/r/ThePlayer60a.c \
	src/prowizard/r/ThePlayer61a.c \
	src/prowizard/r/TitanicsPlayer.c \
	src/prowizard/r/TMK.c \
	src/prowizard/r/TrackerPacker1.c \
	src/prowizard/r/TrackerPacker2.c \
	src/prowizard/r/TrackerPacker3.c \
	src/prowizard/r/UnicTracker2.c \
	src/prowizard/r/UnicTracker.c \
	src/prowizard/r/Viruz2.c \
	src/prowizard/r/WantonPacker.c \
	src/prowizard/r/XannPlayer.c \
	src/prowizard/r/ZenPacker.c
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
