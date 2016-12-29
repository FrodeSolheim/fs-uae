# Makefile.am for FS-UAE

SUBDIRS =

WARNINGS =

CM_CFLAGS =
CM_CFLAGS += @FREETYPE_CFLAGS@
CM_CFLAGS += @GLEW_CFLAGS@
CM_CFLAGS += @GLIB_CFLAGS@
CM_CFLAGS += @LUA_CFLAGS@
CM_CFLAGS += @MIR_CFLAGS@
CM_CFLAGS += @OPENAL_CFLAGS@
CM_CFLAGS += @OPENGL_CFLAGS@
# CM_CFLAGS += @OS_CFLAGS@
CM_CFLAGS += @PNG_CFLAGS@
CM_CFLAGS += @QT_CFLAGS@
CM_CFLAGS += @SDL2_CFLAGS@
# CM_CFLAGS += @X11_CFLAGS@
CM_CFLAGS += @ZLIB_CFLAGS@
CM_CFLAGS += $(WARNINGS)

if F_NO_STRICT_OVERFLOW
CM_CFLAGS += -fno-strict-overflow
endif
if F_NO_STRICT_ALIASING
CM_CFLAGS += -fno-strict-aliasing
endif
if W_MISSING_DECLARATIONS
CM_CFLAGS += -Wmissing-declarations
endif
if W_ADDRESS
CM_CFLAGS += -Waddress
endif
#if W_MISSING_FORMAT_ATTRIBUTE
#CM_CFLAGS += -Wmissing-format-attribute
#endif

CM_CFLAGS += -fvisibility=hidden

AM_CFLAGS =
AM_CFLAGS += $(CM_CFLAGS) -std=gnu99

AM_CXXFLAGS =
AM_CXXFLAGS += $(CM_CFLAGS)

AM_CPPFLAGS =
AM_CPPFLAGS += @OS_CPPFLAGS@
AM_CPPFLAGS += -DUAE -DFSUAE

LIBS += @CARBON_LIBS@
LIBS += @FREETYPE_LIBS@
LIBS += @GLEW_LIBS@
LIBS += @GLIB_LIBS@
LIBS += @IOKIT_LIBS@
# LIBS += @MIR_LIBS@
LIBS += @OPENAL_LIBS@
LIBS += @OPENGL_LIBS@
# LIBS += @OS_LIBS@
LIBS += @PNG_LIBS@
LIBS += @QT_LIBS@
LIBS += @SDL2_LIBS@
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

bin_PROGRAMS = \
	fs-uae \
	fs-uae-device-helper

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
        src/fs-uae/config.c \
	src/fs-uae/config-accelerator.c \
	src/fs-uae/config-accelerator.h \
	src/fs-uae/config-common.c \
	src/fs-uae/config-common.h \
	src/fs-uae/config-drives.c \
	src/fs-uae/config-drives.h \
	src/fs-uae/config-graphics.c \
	src/fs-uae/config-graphics.h \
	src/fs-uae/config-hardware.c \
	src/fs-uae/config-hardware.h \
	src/fs-uae/config-model.c \
	src/fs-uae/config-model.h \
	src/fs-uae/config-paths.c \
	src/fs-uae/config-sound.c \
	src/fs-uae/config-sound.h \
	src/fs-uae/fs-uae.h \
	src/fs-uae/input.c \
	src/fs-uae/joystick.c \
	src/fs-uae/keyboard.c \
	src/fs-uae/main.c \
	src/fs-uae/menu.c \
	src/fs-uae/mouse.c \
	src/fs-uae/options.h \
	src/fs-uae/paths.c \
	src/fs-uae/paths.h \
	src/fs-uae/plugins.c \
	src/fs-uae/plugins.h \
	src/fs-uae/recording.c \
	src/fs-uae/recording.h \
	src/fs-uae/uae_config.c \
	src/fs-uae/video.c


if WINDOWS
nodist_fs_uae_SOURCES += src/fs-uae/fs-uae.rc
.rc.o:
	windres $< -o $@ -I$(s)
endif

fs_uae_LDADD =

fs_uae_device_helper_SOURCES = \
	src/fs-uae/device-helper.c \
	src/fs-uae/paths.c

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
	src/archivers/chd/astring.h \
	src/archivers/chd/bitstream.h \
	src/archivers/chd/chd.h \
	src/archivers/chd/chdcd.h \
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
	src/archivers/chd/sha1.h \
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
	src/def_icons.cpp \
	src/devices.cpp \
	src/disk.cpp \
	src/diskutil.cpp \
	src/dlopen.cpp \
	src/dongle.cpp \
	src/dosbox/callback.h \
	src/dosbox/cmos.cpp \
	src/dosbox/core_full/ea_lookup.h \
	src/dosbox/core_full/load.h \
	src/dosbox/core_full/loadwrite.h \
	src/dosbox/core_full/op.h \
	src/dosbox/core_full/optable.h \
	src/dosbox/core_full/save.h \
	src/dosbox/core_full/string.h \
	src/dosbox/core_full/support.h \
	src/dosbox/core_full.cpp \
	src/dosbox/core_normal/helpers.h \
	src/dosbox/core_normal/prefix_0f.h \
	src/dosbox/core_normal/prefix_66_0f.h \
	src/dosbox/core_normal/prefix_66.h \
	src/dosbox/core_normal/prefix_none.h \
	src/dosbox/core_normal/string.h \
	src/dosbox/core_normal/support.h \
	src/dosbox/core_normal/table_ea.h \
	src/dosbox/core_normal.cpp \
	src/dosbox/core_simple.cpp \
	src/dosbox/cpu.cpp \
	src/dosbox/cpu.h \
	src/dosbox/db_memory.cpp \
	src/dosbox/dosbox.h \
	src/dosbox/flags.cpp \
	src/dosbox/fpu.cpp \
	src/dosbox/fpu.h \
	src/dosbox/fpu_instructions.h \
	src/dosbox/fpu_instructions_x86.h \
	src/dosbox/inout.h \
	src/dosbox/instructions.h \
	src/dosbox/keyboard.cpp \
	src/dosbox/lazyflags.h \
	src/dosbox/logging.h \
	src/dosbox/mem.h \
	src/dosbox/modrm.cpp \
	src/dosbox/modrm.h \
	src/dosbox/paging.cpp \
	src/dosbox/paging.h \
	src/dosbox/pic.cpp \
	src/dosbox/pic.h \
	src/dosbox/regs.h \
	src/dosbox/setup.h \
	src/dosbox/support.h \
	src/dosbox/timer.cpp \
	src/dosbox/timer.h \
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
	src/include/fpp-ieee-be.h \
	src/include/fpp-unknown.h \
	src/include/fsdb.h \
	src/include/fsusage.h \
	src/include/gayle.h \
	src/include/genblitter.h \
	src/include/gensound.h \
	src/include/gfxboard.h \
	src/include/gfxfilter.h \
	src/include/gui.h \
	src/include/ide.h \
	src/include/idecontrollers.h \
	src/include/identify.h \
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
	src/include/uae/attributes.h \
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
	src/include/x86.h \
	src/include/xwin.h \
	src/include/zarchive.h \
	src/include/zfile.h \
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
	src/od-fs/audio.cpp \
	src/od-fs/blkdev-linux.cpp \
	src/od-fs/bsdsocket_host.cpp \
	src/od-fs/callbacks.h \
	src/od-fs/cda_play.cpp \
	src/od-fs/cda_play.h \
	src/od-fs/cdimage_stubs.cpp \
	src/od-fs/charset.cpp \
	src/od-fs/clipboard.cpp \
	src/od-fs/clock.cpp \
	src/od-fs/defines.h \
	src/od-fs/driveclick.cpp \
	src/od-fs/filesys_host.cpp \
	src/od-fs/fsdb_host.cpp \
	src/od-fs/fsdb_host.h \
	src/od-fs/gui.cpp \
	src/od-fs/hardfile_host.cpp \
	src/od-fs/include/direct3d.h \
	src/od-fs/include/registry.h \
	src/od-fs/include/resource.h \
	src/od-fs/include/uae/uae.h \
	src/od-fs/include/uae/uae_inputevents_def.h \
	src/od-fs/include/win32gfx.h \
	src/od-fs/include/win32gui.h \
	src/od-fs/input.cpp \
	src/od-fs/inputrecord.cpp \
	src/od-fs/ioport.cpp \
	src/od-fs/ioport.h \
	src/od-fs/joystick.cpp \
	src/od-fs/keyboard.cpp \
	src/od-fs/libamiga.cpp \
	src/od-fs/logging.cpp \
	src/od-fs/machdep/arm/maccess.h \
	src/od-fs/machdep/generic/maccess.h \
	src/od-fs/machdep/m68k.h \
	src/od-fs/machdep/maccess.h \
	src/od-fs/machdep/machdep.h \
	src/od-fs/machdep/ppc/maccess.h \
	src/od-fs/machdep/rpt.h \
	src/od-fs/machdep/x86/maccess.h \
	src/od-fs/mman.cpp \
	src/od-fs/mouse.cpp \
	src/od-fs/mp3decoder.h \
	src/od-fs/parser.cpp \
	src/od-fs/paths.cpp \
	src/od-fs/roms.cpp \
	src/od-fs/sleep.h \
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
	src/od-fs/video.cpp \
	src/od-fs/vpar.cpp \
	src/od-fs/vpar.h \
	src/od-fs/winuae_compat.h \
	src/od-win32/caps/CapsAPI.h \
	src/od-win32/caps/Comtype.h \
	src/od-win32/caps/caps_win32.cpp \
	src/od-win32/caps/caps_win32.h \
	src/od-win32/debug_win32.h \
	src/od-win32/dxwrap.h \
	src/od-win32/md-fpp.h \
	src/od-win32/parser.h \
	src/od-win32/picasso96_win.cpp \
	src/od-win32/picasso96_win.h \
	src/od-win32/serial_win32.cpp \
	src/od-win32/win32_uaenet.cpp \
	src/patch.cpp \
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
	src/sana2.cpp \
	src/savestate.cpp \
	src/scp.cpp \
	src/scsi.cpp \
	src/scsiemul.cpp \
	src/scsitape.cpp \
	src/segtracker.cpp \
	src/slirp_uae.cpp \
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
        libfsemu/include/fs/emu/cef.h \
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
        libfsemu/src/emu/cef.cpp \
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

if BUILTIN_CEF
AM_CPPFLAGS += -I$(s)/cef
fs_uae_LDADD += cef/libcef_dll/libcef_dll_wrapper.a -L $(s)/cef/Release -lcef
endif

if BUILTIN_MANYMOUSE
noinst_LIBRARIES += libmanymouse.a
AM_CPPFLAGS += -I$(s)/manymouse
fs_uae_LDADD += libmanymouse.a
fs_uae_device_helper_LDADD += libmanymouse.a
libmanymouse_a_SOURCES = \
	manymouse/linux_evdev.c \
	manymouse/macosx_hidmanager.c \
	manymouse/macosx_hidutilities.c \
	manymouse/manymouse.c \
	manymouse/manymouse.h \
	manymouse/windows_wminput.c \
	manymouse/x11_xinput2.c
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

if WITH_PPC
fs_uae_SOURCES += \
	src/ppc/ppcd.cpp \
	src/ppc/ppcd.h
endif

if WITH_SLIRP
if WITH_BUILTIN_SLIRP
noinst_LIBRARIES += libslirp.a
AM_CPPFLAGS += -I$(s)/src/slirp/include
fs_uae_LDADD += libslirp.a
libslirp_a_SOURCES = \
	src/slirp/bootp.cpp \
	src/slirp/bootp.h \
	src/slirp/cksum.cpp \
	src/slirp/ctl.h \
	src/slirp/debug.h \
	src/slirp/icmp_var.h \
	src/slirp/if.cpp \
	src/slirp/if.h \
	src/slirp/ip.h \
	src/slirp/ip_icmp.cpp \
	src/slirp/ip_icmp.h \
	src/slirp/ip_input.cpp \
	src/slirp/ip_output.cpp \
	src/slirp/libslirp.h \
	src/slirp/main.h \
	src/slirp/mbuf.cpp \
	src/slirp/mbuf.h \
	src/slirp/misc.cpp \
	src/slirp/misc.h \
	src/slirp/sbuf.cpp \
	src/slirp/sbuf.h \
	src/slirp/slirp.cpp \
	src/slirp/slirp.h \
	src/slirp/slirp_config.h \
	src/slirp/slirpdebug.cpp \
	src/slirp/socket.cpp \
	src/slirp/socket.h \
	src/slirp/tcp.h \
	src/slirp/tcp_input.cpp \
	src/slirp/tcp_output.cpp \
	src/slirp/tcp_subr.cpp \
	src/slirp/tcp_timer.cpp \
	src/slirp/tcp_timer.h \
	src/slirp/tcp_var.h \
	src/slirp/tcpip.h \
	src/slirp/tftp.cpp \
	src/slirp/tftp.h \
	src/slirp/udp.cpp \
	src/slirp/udp.h
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
	src/fs-uae/luascript.c \
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

if WITH_SOFTFLOAT
noinst_LIBRARIES += libsoftfloat.a
AM_CPPFLAGS += -I$(s)/softfloat
fs_uae_LDADD += libsoftfloat.a
libsoftfloat_a_SOURCES = \
	softfloat/softfloat-uae.h \
	softfloat/fprem.cc \
	softfloat/fpu_constant.h \
	softfloat/fsincos.cc \
	softfloat/poly.cc \
	softfloat/softfloat16.cc \
	softfloat/softfloat.cc \
	softfloat/softfloat-compare.h \
	softfloat/softfloat.h \
	softfloat/softfloat-macros.h \
	softfloat/softfloat-muladd.cc \
	softfloat/softfloat-round-pack.cc \
	softfloat/softfloat-round-pack.h \
	softfloat/softfloat-specialize.cc \
	softfloat/softfloat-specialize.h \
	softfloat/softfloatx80.cc \
	softfloat/softfloatx80.h
endif

if BUILTIN_LIBMPEG2
SUBDIRS += libmpeg2
AM_CPPFLAGS += -I$(s)/libmpeg2/include
fs_uae_LDADD += \
	libmpeg2/libmpeg2/.libs/libmpeg2.a \
	libmpeg2/libmpeg2/convert/.libs/libmpeg2convert.a
else
CM_CFLAGS += @LIBMPEG2_CFLAGS@
LIBS += @LIBMPEG2_LIBS@
endif

noinst_LIBRARIES += libdummy.a
nodist_libdummy_a_SOURCES = \
	gen/cdummy.c \
	gen/cppdummy.cpp
BUILT_SOURCES += $(nodist_libdummy_a_SOURCES)

gen/cdummy.c:
	echo "int cdummy;" > $(b)/gen/cdummy.c

gen/cppdummy.cpp:
	echo "int cppdummy;" > $(b)/gen/cppdummy.cpp

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
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/rules \
	debian/source \
	debian/source/format \
	dist/linux/build.py \
	dist/arch.py \
	dist/linux/standalone-linux.py \
	dist/macos/fs-uae-config.icns \
	dist/macos/fs-uae.icns \
	dist/macos/PkgInfo \
	dist/macos/sign.py \
	dist/macos/standalone.py \
	dist/windows/clib.py \
	dist/windows/iscc.py \
	dist/windows/sign.py \
	flac/include/FLAC/all.h \
	flac/include/FLAC/assert.h \
	flac/include/FLAC/callback.h \
	flac/include/FLAC/export.h \
	flac/include/FLAC/format.h \
	flac/include/FLAC/metadata.h \
	flac/include/FLAC/ordinals.h \
	flac/include/FLAC/stream_decoder.h \
	flac/include/FLAC/stream_encoder.h \
	fs-uae.spec \
	icon/fs-uae.ico \
	glad/include/glad/glad.h \
	glad/include/KHR/khrplatform.h \
	glad/LICENSE \
	libinih/ini.c \
	libinih/ini.h \
	libinih/LICENSE.txt \
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
	src/filesys_bootrom.cpp \
	src/fs-uae/fs-uae.rc.in \
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
	src/od-fs/bsdsocket_posix.cpp \
	src/od-win32/bsdsock.cpp \
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

all-local: mo fs-uae.dat

install-data-hook: mo fs-uae.dat
	chmod -R u+w $(s)/share/
	mkdir -p $(DESTDIR)$(datadir)/applications
	cp -R $(s)/share/applications/* $(DESTDIR)$(datadir)/applications
	mkdir -p $(DESTDIR)$(datadir)/fs-uae
	touch $(DESTDIR)$(datadir)/fs-uae/share-dir
	cp fs-uae.dat $(DESTDIR)$(datadir)/fs-uae
	mkdir -p $(DESTDIR)$(datadir)/icons
	cp -R $(s)/share/icons/* $(DESTDIR)$(datadir)/icons
	mkdir -p $(DESTDIR)$(datadir)/locale
	cp -R $(b)/share/locale/* $(DESTDIR)$(datadir)/locale
	mkdir -p $(DESTDIR)$(datadir)/mime
	cp -R $(s)/share/mime/* $(DESTDIR)$(datadir)/mime

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/applications/fs-uae.desktop
	rm -Rf $(DESTDIR)$(datadir)/fs-uae
	find $(DESTDIR)$(datadir)/icons/hicolor -name fs-uae.png -delete
	find $(DESTDIR)$(datadir)/locale -name fs-uae.mo -delete
	rm -f $(DESTDIR)$(datadir)/mime/packages/fs-uae.xml

clean-local:
	rm -f fs-uae.dat
	find share -name fs-uae.mo -delete

fs-uae-dist:
	$(MAKE) -C dist/@OS_NAME@

standalone: fs-uae$(EXEEXT) fs-uae.dat
	@echo "Stripping fs-uae$(EXEEXT)"
	strip fs-uae$(EXEEXT)
	@echo "Appending fs-uae.dat"
	cat fs-uae.dat >> fs-uae$(EXEEXT)
	@echo "Correcting zip offsets"
	zip -A fs-uae$(EXEEXT)

bindist:
	make distdir
if LINUX
	cd fs-uae-@PACKAGE_VERSION@ && fs-sdk-linux-x86-64 ./configure
	fs-sdk-linux-x86-64 make -C fs-uae-@PACKAGE_VERSION@ -j4
	fs-sdk-linux-x86-64 make -C fs-uae-@PACKAGE_VERSION@/dist/linux
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_linux_x86-64.tar.xz .
endif
if MACOS
	cd fs-uae-@PACKAGE_VERSION@ && ./configure
	make -C fs-uae-@PACKAGE_VERSION@ -j4
	make -C fs-uae-@PACKAGE_VERSION@/dist/macos
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_macos_x86-64.tar.xz .
endif
if WINDOWS
	cd fs-uae-@PACKAGE_VERSION@ && ./configure
	make -C fs-uae-@PACKAGE_VERSION@ -j2
	make -C fs-uae-@PACKAGE_VERSION@/dist/windows tar.xz
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_windows_x86-64.tar.xz .
endif
	rm -Rf fs-uae-@PACKAGE_VERSION@

bindist-x86:
	make distdir
if LINUX
	cd fs-uae-@PACKAGE_VERSION@ && fs-sdk-linux-x86 ./configure
	fs-sdk-linux-x86 make -C fs-uae-@PACKAGE_VERSION@ -j4
	fs-sdk-linux-x86 make -C fs-uae-@PACKAGE_VERSION@/dist/linux
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_linux_x86.tar.xz .
endif
if MACOS
	cd fs-uae-@PACKAGE_VERSION@ && ./configure
	make -C fs-uae-@PACKAGE_VERSION@ -j4
	make -C fs-uae-@PACKAGE_VERSION@/dist/macos
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_macos_x86.tar.xz .
endif
if WINDOWS
	cd fs-uae-@PACKAGE_VERSION@ && ./configure
	make -C fs-uae-@PACKAGE_VERSION@ -j2
	make -C fs-uae-@PACKAGE_VERSION@/dist/windows tar.xz
	mv fs-uae-@PACKAGE_VERSION@/fs-uae_@PACKAGE_VERSION@_windows_x86.tar.xz .
endif
	rm -Rf fs-uae-@PACKAGE_VERSION@
