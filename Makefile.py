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

AM_CFLAGS =
AM_CFLAGS += -I. -Isrc/od-fs -Isrc/od-fs/include
AM_CFLAGS += -Isrc/include -Igen -Isrc/jit -Isrc
AM_CFLAGS += -Ilibfsemu/include -Ilibfsemu/src/lua
AM_CFLAGS += @FREETYPE_CFLAGS@
AM_CFLAGS += @GLIB_CFLAGS@
AM_CFLAGS += @LUA_CFLAGS@
AM_CFLAGS += @MIR_CFLAGS@
AM_CFLAGS += @OPENAL_CFLAGS@
AM_CFLAGS += @OPENGL_CFLAGS@
# AM_CFLAGS += @OS_CFLAGS@
AM_CFLAGS += @PNG_CFLAGS@
AM_CFLAGS += @SDL2_CFLAGS@
# AM_CFLAGS += @X11_CFLAGS@
AM_CFLAGS += @ZLIB_CFLAGS@

AM_CXXFLAGS := $(AM_CFLAGS)
AM_CFLAGS += -std=gnu99

AM_CPPFLAGS = -DHAVE_CONFIG_H @OS_CPPFLAGS@ -DFSUAE -DFSEMU

AM_LIBS =
AM_LIBS += @CARBON_LIBS@
AM_LIBS += @FREETYPE_LIBS@
AM_LIBS += @GLIB_LIBS@
AM_LIBS += @IOKIT_LIBS@
# AM_LIBS += @MIR_LIBS@
AM_LIBS += @OPENAL_LIBS@
AM_LIBS += @OPENGL_LIBS@
# AM_LIBS += @OS_LIBS@
AM_LIBS += @PNG_LIBS@
AM_LIBS += @SDL2_LIBS@
AM_LIBS += @X11_LIBS@
AM_LIBS += @ZLIB_LIBS@

AM_LDFLAGS = @OS_LDFLAGS@

all: fs-uae-device-helper fs-uae

gen/blit.h: gen/genblitter
    gen/genblitter i > gen/blit.h

gen/blitfunc.cpp: gen/genblitter gen/blitfunc.h
    gen/genblitter f > gen/blitfunc.cpp

gen/blitfunc.h: gen/genblitter
    gen/genblitter h > gen/blitfunc.h

gen/blittable.cpp: gen/genblitter gen/blitfunc.h
    gen/genblitter t > gen/blittable.cpp

gen/compemu.cpp: gen/gencomp
    mkdir -p `dirname $@`
    gen/gencomp

gen/compstbl.cpp: gen/compemu.cpp

gen/comptbl.h: gen/compemu.cpp

gen/cpudefs.cpp: gen/build68k src/table68k
    ./gen/build68k < src/table68k > gen/cpudefs.cpp
    # python util/fix_tchar.py gen/cpudefs.cpp

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


build68k_sources = [
    "src/build68k.cpp",
    "src/writelog.cpp",
]


fs_uae_sources = [
    "libfsemu/src/base.c",
    "libfsemu/src/config.c",
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
    "src/inputrecord.cpp",
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
    "src/od-fs/ahi_v2.cpp",
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
    "src/od-fs/keymap.cpp",
    "src/od-fs/libamiga.cpp",
    "src/od-fs/logging.cpp",
    "src/od-fs/mman.cpp",
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


# Using GCC optimization level O0 instead of O2 for cpuemu*, since a bug
# was found caused by the optimizer (where basically (1 ^ 0) & (1 ^ 0) was
# evaluated to 0). This fixes Tower of Babel (IPF).
gen_cpuemu_0_cpp_cflags = "-O0"
gen_cpuemu_11_cpp_cflags = "-O0"
gen_cpuemu_13_cpp_cflags = "-O0"
gen_cpuemu_20_cpp_cflags = "-O0"
gen_cpuemu_21_cpp_cflags = "-O0"
gen_cpuemu_22_cpp_cflags = "-O0"
gen_cpuemu_31_cpp_cflags = "-O0"
gen_cpuemu_32_cpp_cflags = "-O0"
gen_cpuemu_33_cpp_cflags = "-O0"


src_blitter_cpp_deps = ["gen/blit.h"]
src_drawing_cpp_deps = ["gen/linetoscr.cpp"]


def program(name, sources):
    object_list = []
    linker = "$(CC)"

    for source in sources:
        symbol = source.replace("/", "_")
        symbol = symbol.replace(".", "_")
        symbol = symbol.replace("-", "_")
        n, ext = os.path.splitext(source)
        obj_name = "obj/{0}_{1}.o".format(
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
            # "@echo -e \\\\n--- {0} ---".format(obj_name),
            "mkdir -p `dirname $@`",
            "{0} -c $< -o $@".format(compiler),
        ]
        object_list.append(obj_name)
    linker_flags = " $(AM_LDFLAGS) $(LDFLAGS)"
    linker_flags += " $(AM_LIBS) $(LIBS)"

    objects_list_name = name.replace("/", "_").replace("-", "_") + "_objects"
    lists[objects_list_name] = sorted(object_list)
    targets[name] = "$({0})".format(objects_list_name), [
        # "@echo -e \\\\n--- {0} ---".format(name),
        "mkdir -p `dirname $@`",
        "rm -f {0}".format(name),
        "{0} $({1}) {2} -o $@".format(linker, objects_list_name, linker_flags),
    ]


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

    with open("Makefile.in", "w") as f:
        f.write("# Automatically generated by Makefile.py\n")
        f.write(header.replace("    ", "\t"))

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
                f.write("\t{0}\n".format(line))

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


footer = """
version := $(strip $(shell cat VERSION))
series := $(strip $(shell cat SERIES))

ifeq ($(wildcard libfsemu),)
    libfsemu_dir := "../libfsemu"
else
    libfsemu_dir := "libfsemu"
endif

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
    cp -a INSTALL README COPYING NEWS AUTHORS $(dist_dir)
    cp -a VERSION SERIES ChangeLog $(dist_dir)
    # windows.mk macosx.mk debian.mk
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
    # mkdir -p $(dist_dir)/gensrc
    # cp -a gensrc/*.cpp gensrc/*.h $(dist_dir)/gensrc

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
    cd $(dist_dir) && ./bootstrap.sh
    cd $(dist_dir) && rm -Rf autom4te.cache
    cd $(dist_dir) && python util/update-version.py

distcheck: distdir
    cd $(dist_dir) && ./configure
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

pubfiles:
    mkdir -p dist

pubfiles-source:
    mkdir -p dist
    cp -a ChangeLog example.conf dist/
    cp -a fs-uae-$(version)/README dist/
    chmod 0644 dist/README
    chmod 0644 dist/ChangeLog
    chmod 0644 dist/example.conf

clean-dist:
    rm -Rf build dist fs-uae-[0-9]* fs-uae_*

clean:
    rm -Rf gen obj
    rm -f fs-uae fs-uae.exe
    rm -f fs-uae-device-helper fs-uae-device-helper.exe

distclean: clean clean-dist
"""

if __name__ == "__main__":
    main()
