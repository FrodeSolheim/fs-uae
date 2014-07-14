#!/usr/bin/env python3
import os
import sys

header = """
CC = @CC@
CXX = @CXX@
CFLAGS = @CFLAGS@
CXXFLAGS = @CXXFLAGS@
CPPFLAGS = @CPPFLAGS@
LDFLAGS = @LDFLAGS@
LIBS = @LIBS@
OBJCOPY = @OBJCOPY@
@SET_MAKE@
version = @PACKAGE_VERSION@
series = @PACKAGE_SERIES@

PACKAGE_TARNAME = @PACKAGE_TARNAME@
prefix = @prefix@
exec_prefix = @exec_prefix@
bindir = @bindir@
datarootdir = @datarootdir@
docdir = @docdir@

all: fs-uae-device-helper fs-uae fs-uae.dat mo

fs-uae.dat:
    rm -f fs-uae.dat
    zip -r -Z store fs-uae.dat share

fs-uae-with-dat: fs-uae fs-uae.dat
    cat fs-uae fs-uae.dat > fs-uae-with-dat
    zip -A fs-uae-with-dat
    chmod a+x fs-uae-with-dat
"""

flags_and_libs = """
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
"""

gen_deps = """
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
"""


if "--strict" in sys.argv:
    header += "\nAM_CFLAGS += -Wall -Werror\n"


build68k_sources = [
    "src/build68k.cpp",
    "src/writelog.cpp",
]


fs_uae_sources = [
    "libfsemu/src/base.c",
    "libfsemu/src/config.c",
    "libfsemu/src/data.c",
    "libfsemu/src/emu/actions.c",
    "libfsemu/src/emu/audio_common.c",
    "libfsemu/src/emu/audio_debug.c",
    "libfsemu/src/emu/audio_dummy.c",
    "libfsemu/src/emu/audio_dummy.c",
    "libfsemu/src/emu/audio_openal.c",
    "libfsemu/src/emu/audio_sdl.c",
    "libfsemu/src/emu/dialog.c",
    "libfsemu/src/emu/emu.c",
    "libfsemu/src/emu/emu_lua.c",
    "libfsemu/src/emu/font.c",
    "libfsemu/src/emu/hud.c",
    "libfsemu/src/emu/input.c",
    "libfsemu/src/emu/menu.c",
    "libfsemu/src/emu/netplay.c",
    "libfsemu/src/emu/render.c",
    "libfsemu/src/emu/scanlines.c",
    "libfsemu/src/emu/texture.c",
    "libfsemu/src/emu/theme.c",
    "libfsemu/src/emu/util.c",
    "libfsemu/src/emu/video_buffer.c",
    "libfsemu/src/emu/video.c",
    "libfsemu/src/emu/video_options.c",
    "libfsemu/src/emu/xml_shader.c",
    "libfsemu/src/eventlog.c",
    "libfsemu/src/filesys.c",
    "libfsemu/src/glee.c",
    "libfsemu/src/hashtable.c",
    "libfsemu/src/image.c",
    "libfsemu/src/inifile.c",
    "libfsemu/src/init.c",
    "libfsemu/src/list.c",
    "libfsemu/src/log.c",
    "libfsemu/src/lua/lapi.c",
    "libfsemu/src/lua/lauxlib.c",
    "libfsemu/src/lua/lbaselib.c",
    "libfsemu/src/lua/lbitlib.c",
    "libfsemu/src/lua/lcode.c",
    "libfsemu/src/lua/lcorolib.c",
    "libfsemu/src/lua/lctype.c",
    "libfsemu/src/lua/ldblib.c",
    "libfsemu/src/lua/ldebug.c",
    "libfsemu/src/lua/ldo.c",
    "libfsemu/src/lua/ldump.c",
    "libfsemu/src/lua/lfunc.c",
    "libfsemu/src/lua/lgc.c",
    "libfsemu/src/lua/linit.c",
    "libfsemu/src/lua/liolib.c",
    "libfsemu/src/lua/llex.c",
    "libfsemu/src/lua/lmathlib.c",
    "libfsemu/src/lua/lmem.c",
    "libfsemu/src/lua/loadlib.c",
    "libfsemu/src/lua/lobject.c",
    "libfsemu/src/lua/lopcodes.c",
    "libfsemu/src/lua/loslib.c",
    "libfsemu/src/lua/lparser.c",
    "libfsemu/src/lua/lstate.c",
    "libfsemu/src/lua/lstring.c",
    "libfsemu/src/lua/lstrlib.c",
    "libfsemu/src/lua/ltable.c",
    "libfsemu/src/lua/ltablib.c",
    "libfsemu/src/lua/ltm.c",
    "libfsemu/src/lua/lundump.c",
    "libfsemu/src/lua/lvm.c",
    "libfsemu/src/lua/lzio.c",
    "libfsemu/src/ml/clock.c",
    "libfsemu/src/ml/config.c",
    "libfsemu/src/ml/input.c",
    "libfsemu/src/ml/keyboard.c",
    "libfsemu/src/ml/macosx.c",
    "libfsemu/src/ml/manymouse/linux_evdev.c",
    "libfsemu/src/ml/manymouse/macosx_hidmanager.c",
    "libfsemu/src/ml/manymouse/macosx_hidutilities.c",
    "libfsemu/src/ml/manymouse/manymouse.c",
    "libfsemu/src/ml/manymouse/windows_wminput.c",
    "libfsemu/src/ml/manymouse/x11_xinput2.c",
    "libfsemu/src/ml/ml.c",
    "libfsemu/src/ml/mouse.c",
    "libfsemu/src/ml/opengl.c",
    "libfsemu/src/ml/rawinput.c",
    "libfsemu/src/ml/render.c",
    "libfsemu/src/ml/sdl.c",
    "libfsemu/src/ml/util.c",
    "libfsemu/src/ml/video.c",
    "libfsemu/src/ml/video.c",
    "libfsemu/src/ml/video_mode.c",
    "libfsemu/src/ml/video_sync.c",
    "libfsemu/src/ml/windows.c",
    "libfsemu/src/ml/x11.c",
    "libfsemu/src/opengl.c",
    "libfsemu/src/queue.c",
    "libfsemu/src/random.c",
    "libfsemu/src/ref.c",
    "libfsemu/src/string.c",
    "libfsemu/src/thread.c",
    "libfsemu/src/time.c",
    "libfsemu/src/unicode.c",
    "src/a2091.cpp",
    "src/akiko.cpp",
    "src/amax.cpp",
    "src/arcadia.cpp",
    "src/archivers/dms/crc_csum.cpp",
    "src/archivers/dms/getbits.cpp",
    "src/archivers/dms/maketbl.cpp",
    "src/archivers/dms/pfile.cpp",
    "src/archivers/dms/tables.cpp",
    "src/archivers/dms/u_deep.cpp",
    "src/archivers/dms/u_heavy.cpp",
    "src/archivers/dms/u_init.cpp",
    "src/archivers/dms/u_medium.cpp",
    "src/archivers/dms/u_quick.cpp",
    "src/archivers/dms/u_rle.cpp",
    "src/archivers/mp2/kjmp2.cpp",
    "src/archivers/zip/unzip.cpp",
    "src/ar.cpp",
    "src/aros.rom.cpp",
    "src/audio.cpp",
    "src/autoconf.cpp",
    "src/blitter.cpp",
    "src/blkdev_cdimage.cpp",
    "src/blkdev.cpp",
    "src/bsdsocket.cpp",
    "src/calc.cpp",
    "src/cd32_fmv.cpp",
    "src/cd32_fmv_genlock.cpp",
    "src/cdrom.cpp",
    "src/cdtv.cpp",
    "src/cfgfile.cpp",
    "src/cia.cpp",
    "src/consolehook.cpp",
    "src/cpummu30.cpp",
    "src/cpummu.cpp",
    "src/crc32.cpp",
    "src/custom.cpp",
    "src/debug.cpp",
    "src/disk.cpp",
    "src/diskutil.cpp",
    "src/dongle.cpp",
    "src/drawing.cpp",
    "src/driveclick.cpp",
    "src/enforcer.cpp",
    "src/events.cpp",
    "src/expansion.cpp",
    "src/fdi2raw.cpp",
    "src/filesys.cpp",
    "src/fpp.cpp",
    "src/fsdb.cpp",
    "src/fs-uae/config.c",
    "src/fs-uae/input.c",
    "src/fs-uae/joystick.c",
    "src/fs-uae/keyboard.c",
    "src/fs-uae/luascript.c",
    "src/fs-uae/main.c",
    "src/fs-uae/menu.c",
    "src/fs-uae/mouse.c",
    "src/fs-uae/paths.c",
    "src/fs-uae/plugins.c",
    "src/fs-uae/recording.c",
    "src/fs-uae/uae_config.c",
    "src/fs-uae/video.c",
    "src/fsusage.cpp",
    "src/gayle.cpp",
    "gen/blitfunc.cpp",
    "gen/blittable.cpp",
    "gen/compemu.cpp",
    "gen/compstbl.cpp",
    "gen/cpudefs.cpp",
    "gen/cpuemu_0.cpp",
    "gen/cpuemu_11.cpp",
    "gen/cpuemu_13.cpp",
    "gen/cpuemu_20.cpp",
    "gen/cpuemu_21.cpp",
    "gen/cpuemu_22.cpp",
    "gen/cpuemu_31.cpp",
    "gen/cpuemu_32.cpp",
    "gen/cpuemu_33.cpp",
    "gen/cpustbl.cpp",
    "src/gfxboard.cpp",
    "src/gfxutil.cpp",
    "src/hardfile.cpp",
    "src/hrtmon.rom.cpp",
    "src/identify.cpp",
    "src/inputdevice.cpp",
    "src/isofs.cpp",
    "src/jit/compemu_fpp.cpp",
    "src/jit/compemu_support.cpp",
    "src/keybuf.cpp",
    "src/luascript.cpp",
    "src/main.cpp",
    "src/memory.cpp",
    "src/missing.cpp",
    "src/native2amiga.cpp",
    "src/ncr_scsi.cpp",
    "src/newcpu_common.cpp",
    "src/newcpu.cpp",
    "src/od-fs/ahidsound_new.cpp",
    "src/od-fs/ahi_winuae.cpp",
    "src/od-fs/audio.cpp",
    "src/od-fs/blkdev-linux.cpp",
    "src/od-fs/bsdsocket_host.cpp",
    "src/od-fs/caps.cpp",
    "src/od-fs/cda_play.cpp",
    "src/od-fs/cdimage_stubs.cpp",
    "src/od-fs/charset.cpp",
    "src/od-fs/clipboard.cpp",
    "src/od-fs/clock.cpp",
    "src/od-fs/driveclick.cpp",
    "src/od-fs/filesys_host.cpp",
    "src/od-fs/fsdb_host.cpp",
    "src/od-fs/gui.cpp",
    "src/od-fs/hardfile_host.cpp",
    "src/od-fs/input.cpp",
    "src/od-fs/inputrecord.cpp",
    "src/od-fs/keymap.cpp",
    "src/od-fs/libamiga.cpp",
    "src/od-fs/logging.cpp",
    "src/od-fs/parser.cpp",
    "src/od-fs/paths.cpp",
    "src/od-fs/picasso96.cpp",
    "src/od-fs/random.cpp",
    "src/od-fs/roms.cpp",
    "src/od-fs/serial_host.cpp",
    "src/od-fs/stubs.cpp",
    "src/od-fs/support.cpp",
    "src/od-fs/threading.cpp",
    "src/od-fs/uae_host.cpp",
    "src/od-fs/uaemisc.cpp",
    "src/od-fs/util.cpp",
    "src/od-fs/version.cpp",
    "src/od-fs/video.cpp",
    "src/od-win32/mman.cpp",
    "src/qemuvga/cirrus_vga.cpp",
    "src/qemuvga/lsi53c895a.cpp",
    "src/qemuvga/qemuuaeglue.cpp",
    "src/qemuvga/vga.cpp",
    "src/readcpu.cpp",
    "src/rommgr.cpp",
    "src/savestate.cpp",
    "src/scp.cpp",
    "src/scsi.cpp",
    "src/scsiemul.cpp",
    "src/scsitape.cpp",
    "src/specialmonitors.cpp",
    "src/statusline.cpp",
    "src/traps.cpp",
    "src/uaeexe.cpp",
    "src/uaelib.cpp",
    "src/uaenative.cpp",
    "src/uaeresource.cpp",
    "src/uaeserial.cpp",
    "src/zfile_archive.cpp",
    "src/zfile.cpp",
]


fs_uae_device_helper_sources = [
    "libfsemu/src/base.c",
    "libfsemu/src/data.c",
    "libfsemu/src/filesys.c",
    "libfsemu/src/hashtable.c",
    "libfsemu/src/list.c",
    "libfsemu/src/ml/manymouse/linux_evdev.c",
    "libfsemu/src/ml/manymouse/macosx_hidmanager.c",
    "libfsemu/src/ml/manymouse/macosx_hidutilities.c",
    "libfsemu/src/ml/manymouse/manymouse.c",
    "libfsemu/src/ml/manymouse/windows_wminput.c",
    "libfsemu/src/ml/manymouse/x11_xinput2.c",
    "libfsemu/src/ml/util.c",
    "libfsemu/src/string.c",
    "src/fs-uae/device-helper.c",
]


genblitter_sources = [
    "src/blitops.cpp",
    "src/genblitter.cpp",
    "src/writelog.cpp",
]


gencomp_sources = [
    "gen/cpudefs.cpp",
    "src/jit/gencomp.cpp",
    "src/missing.cpp",
    "src/readcpu.cpp",
]


gencpu_sources = [
    "gen/cpudefs.cpp",
    "src/gencpu.cpp",
    "src/missing.cpp",
    "src/readcpu.cpp",
]


genlinetoscr_sources = [
    "src/genlinetoscr.cpp",
]


# Some code (e.g. op_4480_13 handling on NEG.L) relies on signed
# integers overflowing (undefined behavior). Without fno-strict-overflow,
# Tower of Babel (IPF) will for example not load.

cpuemu_flags = "-fno-strict-overflow"
if "--strict" in sys.argv:
    cpuemu_flags += " -Wno-unused-variable" \
                    " -Wno-error" \
                    " -Wno-sign-compare"


gen_cpuemu_0_cpp_cflags = cpuemu_flags
gen_cpuemu_11_cpp_cflags = cpuemu_flags
gen_cpuemu_13_cpp_cflags = cpuemu_flags
gen_cpuemu_20_cpp_cflags = cpuemu_flags
gen_cpuemu_21_cpp_cflags = cpuemu_flags
gen_cpuemu_22_cpp_cflags = cpuemu_flags
gen_cpuemu_31_cpp_cflags = cpuemu_flags
gen_cpuemu_32_cpp_cflags = cpuemu_flags
gen_cpuemu_33_cpp_cflags = cpuemu_flags


if "--strict" in sys.argv:
    # exceptions to allow other modules to be compiled with -Werror
    gen_blitfunc_cpp_cflags = "-Wno-unused-variable"
    src_bsdsocket_cpp_cflags = "-Wno-unused-variable"
    src_build68k_cpp_cflags = "-Wno-error"
    src_jit_gencomp_cpp_cflags = "-Wno-error"
    src_jit_gencomp_cpp_cflags += " -Wno-unused-function"
    src_gencpu_cpp_cflags = "-Wno-error"
    src_gencpu_cpp_cflags += " -Wno-unused-function"
    src_a2091_cpp_cflags = "-Wno-sign-compare"
    src_akiko_cpp_cflags = "-Wno-sign-compare -Wno-unused-function"
    src_ar_cpp_cflags = "-Wno-sign-compare"
    src_audio_cpp_cflags = "-Wno-sign-compare"
    src_blitter_cpp_cflags = "-Wno-sign-compare"
    src_blkdev_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_blkdev_cdimage_cpp_cflags = "-Wno-sign-compare -Wno-error=unused-but-set-variable"
    src_calc_cpp_cflags = "-Wno-sign-compare"
    src_cd32_fmv_cpp_cflags = "-Wno-sign-compare"
    src_cdtv_cpp_cflags = "-Wno-error"
    src_cfgfile_cpp_cflags = "-Wno-sign-compare -Wno-error=parentheses -Wno-unused-variable -Wno-error=unused-but-set-variable"
    src_cia_cpp_cflags = "-Wno-sign-compare"
    src_consolehook_cpp_cflags = "-Wno-sign-compare"
    src_cpummu_cpp_cflags = "-Wno-error"
    src_custom_cpp_cflags = "-Wno-sign-compare -Wno-unused-variable -Wno-error -Wno-unused-function"
    src_debug_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_disk_cpp_cflags = "-Wno-error -Wno-unused-function -Wno-sign-compare -Wno-unused-variable"
    src_drawing_cpp_cflags = "-Wno-unused-function -Wno-sign-compare -Wno-error"
    src_fdi2raw_cpp_cflags = "-Wno-unused-function -Wno-sign-compare -Wno-error"
    src_filesys_cpp_cflags = "-Wno-unused-variable -Wno-sign-compare -Wno-error"
    src_fpp_cpp_cflags = "-Wno-error -Wno-unused-function"
    src_gayle_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_gfxboard_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_hardfile_cpp_cflags = "-Wno-sign-compare"
    src_inputdevice_cpp_cflags = "-Wno-sign-compare -Wno-error -Wno-unused-variable"
    src_isofs_cpp_cflags = "-Wno-sign-compare -Wno-unused-function -Wno-error"
    src_main_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_memory_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_ncr_scsi_cpp_cflags = "-Wno-unused-variable -Wno-unused-function"
    src_newcpu_cpp_cflags = "-Wno-sign-compare -Wno-unused-variable -Wno-error"
    src_od_fs_ahi_winuae_cpp_cflags = "-Wno-sign-compare -Wno-unused-variable -Wno-error -Wno-unused-function"
    src_od_fs_hardfile_host_cpp_cflags = "-Wno-error"
    src_od_fs_mman_cpp_cflags = "-Wno-unused-variable -Wno-error"
    src_od_fs_picasso96_cpp_cflags = "-Wno-error=overflow -Wno-error -Wno-sign-compare -Wno-unused-variable -Wno-error=unused-but-set-variable -Wno-unused-function"
    src_od_fs_video_cpp_cflags = "-Wno-unused-variable -Wno-error -Wno-unused-function"
    src_qemuvga_cirrus_vga_cpp_cflags = "-Wno-sign-compare -Wno-unused-function -Wno-error"
    src_qemuvga_lsi53c895a_cpp_cflags = "-Wno-sign-compare"
    src_qemuvga_vga_cpp_cflags = "-Wno-sign-compare -Wno-unused-function -Wno-error"
    src_rommgr_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_savestate_cpp_cflags = "-Wno-sign-compare -Wno-error"
    src_scsiemul_cpp_cflags = "-Wno-sign-compare -Wno-unused-variable"
    src_specialmonitors_cpp_cflags = "-Wno-error"
    src_statusline_cpp_cflags = "-Wno-error"
    src_traps_cpp_cflags = "-Wno-sign-compare"
    src_uaelib_cpp_cflags = "-Wno-sign-compare"
    src_uaeserial_cpp_cflags = "-Wno-sign-compare"
    src_zfile_cpp_cflags = "-Wno-sign-compare -Wno-unused-function -Wno-error"
    src_zfile_archive_cpp_cflags = "-Wno-sign-compare -Wno-error"


src_blitter_cpp_deps = ["gen/blit.h"]
src_drawing_cpp_deps = ["gen/linetoscr.cpp"]
src_jit_compemu_support_cpp_deps = ["gen/comptbl.h"]


def program(name, sources):
    object_list = []
    linker = "$(CC)"

    for source in sources:
        symbol = source.replace("/", "_")
        symbol = symbol.replace(".", "_")
        symbol = symbol.replace("-", "_")
        n, ext = os.path.splitext(source)
        obj_name = ".obj/{0}_{1}.o".format(
            name.replace("/", "_").replace("-", "_"), n.replace("/", "_"))
        if ext in [".cpp", ".cc"]:
            compiler = "$(CXX)"
            compiler += " $(AM_CXXFLAGS) $(CXXFLAGS)"
            linker = "$(CXX)"
        else:
            compiler = "$(CC)"
            compiler += " $(AM_CFLAGS) $(CFLAGS)"
        compiler += " $(AM_CPPFLAGS) $(CPPFLAGS)"
        deps = [source]
        extra_cflags = globals().get(symbol + "_cflags", "")
        if extra_cflags:
            compiler += " " + extra_cflags
        deps.extend(globals().get(symbol + "_deps", []))
        targets[obj_name] = " ".join(deps), [
            "\t@mkdir -p {0}".format(os.path.dirname(obj_name)),
            "\t{0} -c {1} -o {2}".format(compiler, source, obj_name),
        ]
        object_list.append(obj_name)
    linker_flags = " $(AM_LDFLAGS) $(LDFLAGS)"
    linker_flags += " $(LIBS)"

    objects_list_name = name.replace("/", "_").replace("-", "_") + "_objects"
    lists[objects_list_name] = sorted(object_list)
    targets[name] = "$({0})".format(objects_list_name), [
        "\tmkdir -p {0}".format(os.path.dirname(name) or "."),
        "\t@rm -f {0}".format(name),
        "\t{0} $({1}) {2} -o {3}".format(
            linker, objects_list_name, linker_flags, name),
        "ifneq (\"$(OBJCOPY)\",\"\")",
        "\tobjcopy {0} {0}.dbg".format(name),
        "\tobjcopy --add-gnu-debuglink={0}.dbg {0}".format(name),
        "\tchmod a-x {0}.dbg".format(name),
        "\tstrip -S {0}".format(name),
        "endif",
    ]


def po_files():
    object_list = []
    for name in os.listdir("po"):
        if not name.endswith(".po"):
            continue
        source = "po/{0}".format(name)
        obj_name = "share/locale/{0}/LC_MESSAGES/fs-uae.mo".format(name[:-3])
        deps = [source]
        targets[obj_name] = " ".join(deps), [
            # "\t@echo -e \\\\n--- {0} ---".format(obj_name),
            "\tmkdir -p share/locale/{0}/LC_MESSAGES".format(name[:-3]),
            "\tmsgfmt --verbose {0} -o {1}".format(source, obj_name),
        ]
        object_list.append(obj_name)
    objects_list_name = "mo_files"
    lists[objects_list_name] = sorted(object_list)
    targets["mo"] = "\t$({0})".format(objects_list_name), []


def main():
    global targets, lists
    targets = {}
    lists = {}

    program("fs-uae", fs_uae_sources)
    program("fs-uae-device-helper", fs_uae_device_helper_sources)
    program("gen/build68k", build68k_sources)
    program("gen/genblitter", genblitter_sources)
    program("gen/gencomp", gencomp_sources)
    program("gen/gencpu", gencpu_sources)
    program("gen/genlinetoscr", genlinetoscr_sources)
    po_files()
    if "Makefile.am" in sys.argv:
        with open("Makefile.am", "w") as f:
            makefile_am(f)
    else:
        makefile_in()


def makefile_am(f):
    f.write("""
bin_PROGRAMS = \\
    fs-uae \\
    fs-uae-device-helper

noinst_PROGRAMS = \\
    gen/build68k gen/genblitter \\
    gen/gencomp \\
    gen/gencpu \\
    gen/genlinetoscr
""".replace("    ", "\t"))

    f.write("""
BUILT_SOURCES = \\
    gen/blit.h \\
    gen/blitfunc.cpp \\
    gen/blitfunc.h \\
    gen/blittable.cpp \\
    gen/compemu.cpp \\
    gen/compstbl.cpp \\
    gen/comptbl.h \\
    gen/cpudefs.cpp \\
    gen/cpuemu_0.cpp \\
    gen/cpuemu_11.cpp \\
    gen/cpuemu_13.cpp \\
    gen/cpuemu_20.cpp \\
    gen/cpuemu_21.cpp \\
    gen/cpuemu_22.cpp \\
    gen/cpuemu_31.cpp \\
    gen/cpuemu_32.cpp \\
    gen/cpuemu_33.cpp \\
    gen/cpustbl.cpp \\
    gen/cputbl.h \\
    gen/linetoscr.cpp
""".replace("    ", "\t"))
    f.write(gen_deps.replace("    ", "\t"))
    f.write(flags_and_libs.replace("    ", "\t"))

    f.write("\nfs_uae_SOURCES = ")
    for source in fs_uae_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\nfs_uae_device_helper_SOURCES = ")
    for source in fs_uae_device_helper_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\ngen_build68k_SOURCES = ")
    for source in build68k_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\ngen_genblitter_SOURCES = ")
    for source in genblitter_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\ngen_gencomp_SOURCES = ")
    for source in gencomp_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\ngen_gencpu_SOURCES = ")
    for source in gencpu_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")

    f.write("\ngen_genlinetoscr_SOURCES = ")
    for source in genlinetoscr_sources:
        f.write("\\\n\t{0}".format(source))
    f.write("\n")


def makefile_in():
    with open("Makefile.in", "w") as f:
        f.write("# Makefile.  Generated from Makefile.in by configure.\n")
        f.write("# Makefile.in.  Generated by Makefile.py.\n")
        f.write(header.replace("    ", "\t"))
        f.write(gen_deps.replace("    ", "\t"))
        f.write(flags_and_libs.replace("    ", "\t"))

        for list in sorted(lists):
            members = lists[list]
            f.write("\n{0} = \\\n".format(list))
            for i, member in enumerate(members):
                extra = "" if i == len(members) - 1 else " \\"
                f.write("{0}{1}\n".format(member, extra))

        for target in sorted(targets):
            dependencies, lines = targets[target]
            f.write("\n{0}: {1}\n".format(target, dependencies))
            for line in lines:
                f.write("{0}\n".format(line))

        f.write("\n")
        f.write(footer.replace("    ", "\t"))

    if "--verify-dependencies" in sys.argv:
        # This test makes sure that each object target can be built isolated,
        # i.e. that dependencies are correctly configured.
        for target in sorted(targets):
            if not target.endswith(".o"):
                continue
            print("\nverifying target", target)
            print("")
            assert os.system("make clean") == 0
            assert os.system("make {0}".format(target)) == 0

    if "--verify-config-usage" in sys.argv:
        # This test makes sure config.h is included in "all" source files,
        # to avoid bugs where a macro is not correctly defined.
        os.system("make clean")
        os.system("rm -f config.h")
        for target in sorted(targets):
            if not target.endswith(".o"):
                continue
            for skip in ["_lua_", "_manymouse_", "_dms_", "_qemuvga_",
                         "_aros.rom.o", "_hrtmon.rom.o", "_blitops.o",
                         "_genlinetoscr.o", "_readcpu.o", "_missing.o",
                         "_gencpu.o", "_cpudefs.o", "_gencomp.o",
                         "_writelog.o"]:
                if skip in target:
                    break
            else:
                print("\nverifying target", target)
                print("")
                # compilation should fail since config.h is not present
                assert os.system("make {0}".format(target)) != 0
        print("\nALL OK\n")


footer = """\
dist_name = fs-uae-$(version)
dist_dir := $(dist_name)
libfsemu_dir := libfsemu

distdir-base:
    rm -Rf $(dist_dir)
    mkdir -p $(dist_dir)

    cp -a INSTALL README COPYING NEWS AUTHORS $(dist_dir)
    cp -a ChangeLog $(dist_dir)
    cp -a fs-uae.spec $(dist_dir)
    cp -a example.conf $(dist_dir)
    cp -a configure.ac $(dist_dir)
    cp -a bootstrap.sh $(dist_dir)
    cp -a Makefile.py $(dist_dir)
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

    mkdir -p $(dist_dir)/libfsemu
    cp -a $(libfsemu_dir)/COPYING $(dist_dir)/libfsemu
    cp -a $(libfsemu_dir)/README $(dist_dir)/libfsemu
    cp -a $(libfsemu_dir)/include $(dist_dir)/libfsemu
    cp -a $(libfsemu_dir)/src $(dist_dir)/libfsemu

    mkdir -p $(dist_dir)/po
    cp -a po/*.po $(dist_dir)/po/
    cp -a po/update.py $(dist_dir)/po/update.py
    cp -a po/update.sh $(dist_dir)/po/update.sh

    mkdir -p $(dist_dir)/doc
    cp -a doc/Default.fs-uae $(dist_dir)/doc/

    mkdir -p $(dist_dir)/build/linux-dist
    cp -p build/linux-dist/build.py $(dist_dir)/build/linux-dist/
    cp -p build/linux-dist/Makefile.in $(dist_dir)/build/linux-dist/
    cp -p build/linux-dist/standalone.py $(dist_dir)/build/linux-dist/

    mkdir -p $(dist_dir)/build/steamos-dist
    cp -p build/steamos-dist/Makefile.in $(dist_dir)/build/steamos-dist/
    cp -p build/steamos-dist/README $(dist_dir)/build/steamos-dist/

    mkdir -p $(dist_dir)/macosx
    cp -p macosx/standalone.py $(dist_dir)/macosx/
    cp -p macosx/Info.plist.in $(dist_dir)/macosx/
    cp -p macosx/Makefile.in $(dist_dir)/macosx/
    cp -p macosx/PkgInfo $(dist_dir)/macosx/

    mkdir -p $(dist_dir)/windows
    cp -p windows/Makefile.in $(dist_dir)/windows/
    cp -p windows/replace_icon.py $(dist_dir)/windows/
    cp -p windows/fs-uae.iss.in $(dist_dir)/windows/

    mkdir -p $(dist_dir)/debian
    cp -a debian/changelog $(dist_dir)/debian/
    cp -a debian/compat $(dist_dir)/debian/
    cp -a debian/control $(dist_dir)/debian/
    cp -a debian/copyright $(dist_dir)/debian/
    cp -a debian/rules $(dist_dir)/debian/
    cp -a debian/source $(dist_dir)/debian/

    mkdir -p $(dist_dir)/icon
    cp icon/fs-uae.ico $(dist_dir)/icon/
    cp icon/fs-uae.icns $(dist_dir)/icon/
    cp icon/fs-uae-launcher.ico $(dist_dir)/icon/
    cp icon/fs-uae-launcher.icns $(dist_dir)/icon/
    cp icon/fs-uae-config.icns $(dist_dir)/icon/

    find $(dist_dir) -name "*~" -delete

distdir: distdir-base
    cd $(dist_dir) && ./bootstrap.sh
    cd $(dist_dir) && rm -Rf autom4te.cache

distcheck: distdir
    cd $(dist_dir) && ./configure
    cd $(dist_dir) && $(MAKE) all distclean

dist: distdir
    find $(dist_dir) -exec touch \{\} \;
    tar zcfv $(dist_name).tar.gz $(dist_name)

install:
    install -d $(DESTDIR)$(bindir)
    install fs-uae $(DESTDIR)$(bindir)/fs-uae
    install fs-uae-device-helper $(DESTDIR)$(bindir)/fs-uae-device-helper
    install -d $(DESTDIR)$(datarootdir)
    cp -R share/* $(DESTDIR)$(datarootdir)
    cp fs-uae.dat $(DESTDIR)$(datarootdir)/fs-uae/fs-uae.dat
    install -d $(DESTDIR)$(docdir)
    cp README COPYING example.conf $(DESTDIR)$(docdir)

clean:
    rm -Rf gen .obj
    rm -f fs-uae
    rm -f fs-uae.dbg
    rm -f fs-uae.exe
    rm -f fs-uae-device-helper
    rm -f fs-uae-device-helper.dbg
    rm -f fs-uae-device-helper.exe

distclean: clean
    rm -f config.h
    rm -f config.log
    rm -f config.status
    rm -Rf dist
    rm -Rf fs-uae[-_][0-9]*
    rm -Rf fs-uae-dbg[-_][0-9]*
    rm -f macosx/Info.plist
    rm -f macosx/Makefile
    rm -f Makefile
    rm -Rf share/locale/
    rm -f stamp-h1
    rm -f windows/fs-uae.iss
    rm -f windows/Makefile
"""

if __name__ == "__main__":
    main()
