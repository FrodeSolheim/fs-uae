include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))

all: fs-uae mo

cppflags = -DFSEMU -DFSUAE -D_FILE_OFFSET_BITS=64

ifeq ($(debug), 1)
cppflags += -DDEBUG
endif

#ifeq ($(wildcard libfsemu), libfsemu)
#libfsemu_dir = libfsemu
#libfsemu/out/libfsemu.a:
#	make -C libfsemu arch=$(arch)
#libfsemu-target: libfsemu/out/libfsemu.a
#else
#libfsemu_dir = ../libfsemu
#libfsemu-target:
#endif

libfsemu_dir = libfsemu
libfsemu-target:
	$(make) -C libfsemu debug=$(debug) devel=$(devel) optimize=$(optimize) \
			android=$(android)

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
		-Isrc/include -Igensrc -Isrc -Isrc/od-win32/caps \
		-I$(libfsemu_dir)/include \
		-Wno-write-strings -fpermissive

cflags = -std=c99 $(cxxflags)
ldflags =
libs = -L$(libfsemu_dir)/out -lfsemu -lpng -lz

else
use_glib := 1
use_freetype := 1
common_flags = -Isrc/od-fs -Isrc/od-fs/include \
		-Isrc/include -Igensrc -Isrc -Isrc/od-win32/caps \
		`pkg-config --cflags glib-2.0 gthread-2.0 libpng` \
		-I$(libfsemu_dir)/include \
		-I$(libfsemu_dir)/src/lua \
		`sdl-config --cflags`
cflags = $(common_flags) -std=c99 $(CFLAGS)
#cxxflags = $(common_flags) -fpermissive $(CXXFLAGS)
cxxflags = $(common_flags) $(CXXFLAGS)
ldflags = $(LDFLAGS)
libs = -L$(libfsemu_dir)/out -lfsemu `sdl-config --libs` \
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
generate = 0

ifeq ($(os), android)
  cppflags += -DANDROID
  cxxflags += 
  libs += -lGLESv1_CM
else ifeq ($(os), windows)
  cppflags += -DWINDOWS
  cxxflags += -U_WIN32 -UWIN32
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
  endif
  cflags += -arch $(arch)
  cxxflags += -arch $(arch)
  ldflags += -arch $(arch) -headerpad_max_install_names
  cppflags += -DMACOSX
  libs += -framework OpenGL -framework Carbon -framework OpenAL
else ifeq ($(os), freebsd)
  cppflags += -DFREEBSD
  libs += -lGL -lGLU -lopenal -lX11 -lcompat
else ifeq ($(os), openbsd)
  cppflags += -DOPENBSD
  libs += -lGL -lGLU -lopenal -lX11 -lcompat
else
  cppflags += -DLINUX
  ldflags += -Wa,--execstack
  libs += -lGL -lGLU -lopenal -ldl -lX11
  generate = 0
endif

ifneq ($(os), android)
	cppflags += -DUSE_SDL -DUSE_GLIB -DWITH_LUA
endif

objects = \
obj/fs-uae-config.o \
obj/fs-uae-input.o \
obj/fs-uae-joystick.o \
obj/fs-uae-keyboard.o \
obj/fs-uae-luascript.o \
obj/fs-uae-main.o \
obj/fs-uae-menu.o \
obj/fs-uae-mouse.o \
obj/fs-uae-paths.o \
obj/fs-uae-uae_config.o \
obj/fs-uae-version.o \
obj/fs-uae-video.o

ifeq ($(os), windows)
objects += obj/fs-uae.res
endif

uae_objects = \
obj/gensrc-blitfunc.o \
obj/gensrc-blittable.o \
obj/gensrc-cpudefs.o \
obj/gensrc-cpuemu_0.o \
obj/gensrc-cpuemu_11.o \
obj/gensrc-cpuemu_12.o \
obj/gensrc-cpuemu_20.o \
obj/gensrc-cpuemu_21.o \
obj/gensrc-cpuemu_22.o \
obj/gensrc-cpuemu_31.o \
obj/gensrc-cpustbl.o \
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
obj/crc32.o \
obj/custom.o \
obj/debug.o \
obj/disk.o \
obj/diskutil.o \
obj/dongle.o \
obj/drawing.o \
obj/driveclick.o \
obj/enforcer.o \
obj/ersatz.o \
obj/events.o \
obj/expansion.o \
obj/fdi2raw.o \
obj/filesys.o \
obj/fpp.o \
obj/fsdb.o \
obj/fsusage.o \
obj/gayle.o \
obj/gfxutil.o \
obj/hardfile.o \
obj/hrtmon.rom.o \
obj/identify.o \
obj/inputrecord.o \
obj/isofs.o \
obj/keybuf.o \
obj/luascript.o \
obj/main.o \
obj/memory.o \
obj/missing.o \
obj/native2amiga.o \
obj/newcpu.o \
obj/ncr_scsi.o \
obj/readcpu.o \
obj/rommgr.o \
obj/savestate.o \
obj/scsi.o \
obj/scsiemul.o \
obj/specialmonitors.o \
obj/statusline.o \
obj/traps.o \
obj/uaeexe.o \
obj/uaelib.o \
obj/uaeresource.o \
obj/uaeserial.o \
obj/zfile.o \
obj/zfile_archive.o \
obj/jit-compemu.o \
obj/jit-compemu_fpp.o \
obj/jit-compemu_support.o \
obj/jit-compstbl.o \
obj/zip-archiver-unzip.o \
obj/dms-archiver-crc_csum.o \
obj/dms-archiver-getbits.o \
obj/dms-archiver-maketbl.o \
obj/dms-archiver-pfile.o \
obj/dms-archiver-tables.o \
obj/dms-archiver-u_deep.o \
obj/dms-archiver-u_heavy.o \
obj/dms-archiver-u_init.o \
obj/dms-archiver-u_medium.o \
obj/dms-archiver-u_quick.o \
obj/dms-archiver-u_rle.o \
obj/od-fs-audio.o \
obj/od-fs-bsdsocket_host.o \
obj/od-fs-blkdev-linux.o \
obj/od-fs-caps.o \
obj/od-fs-cda_play.o \
obj/od-fs-charset.o \
obj/od-fs-clock.o \
obj/od-fs-cdimage_stubs.o \
obj/od-fs-clipboard.o \
obj/od-fs-driveclick.o \
obj/od-fs-filesys_host.o \
obj/od-fs-fsdb_host.o \
obj/od-fs-hardfile_host.o \
obj/od-fs-input.o \
obj/od-fs-inputdevice.o \
obj/od-fs-keymap.o \
obj/od-fs-libamiga.o \
obj/od-fs-logging.o \
obj/od-fs-mman.o \
obj/od-fs-parser.o \
obj/od-fs-random.o \
obj/od-fs-roms.o \
obj/od-fs-picasso96.o \
obj/od-fs-serial_host.o \
obj/od-fs-stubs.o \
obj/od-fs-support.o \
obj/od-fs-util.o \
obj/od-fs-threading.o \
obj/od-fs-uae_host.o \
obj/od-fs-uaemisc.o \
obj/od-fs-version.o \
obj/od-fs-video.o

ifeq ($(generate), 1)

gensrc/genblitter: obj/genblitter.o obj/blitops.o obj/writelog.o
	$(cxx) $(cppflags) $(cxxflags) obj/genblitter.o obj/blitops.o \
			obj/writelog.o -o gensrc/genblitter

gensrc/gencpu: gensrc/cpudefs.cpp obj/gencpu.o obj/readcpu.o obj/missing.o \
		gensrc/cpudefs.cpp
	$(cxx) $(cppflags) $(cxxflags) obj/gencpu.o obj/readcpu.o obj/missing.o \
			gensrc/cpudefs.cpp -o gensrc/gencpu

gensrc/genlinetoscr:
	$(cxx) $(cppflags) $(cxxflags) src/genlinetoscr.cpp -o gensrc/genlinetoscr

gensrc/gencomp: src/jit/gencomp.cpp obj/readcpu.o obj/gen-cpudefs.o obj/missing.o obj/writelog.o
	$(cxx) $(cppflags) $(cxxflags) src/jit/gencomp.cpp obj/readcpu.o obj/gen-cpudefs.o obj/missing.o obj/writelog.o -o gensrc/gencomp

gensrc/linetoscr.cpp: gensrc/genlinetoscr
	gensrc/genlinetoscr > gensrc/linetoscr.cpp

gensrc/blit.h: gensrc/genblitter
	gensrc/genblitter i > gensrc/blit.h

gensrc/blitfunc.cpp: gensrc/genblitter gensrc/blitfunc.h
	gensrc/genblitter f > gensrc/blitfunc.cpp

gensrc/blitfunc.h: gensrc/genblitter
	gensrc/genblitter h > gensrc/blitfunc.h

gensrc/blittable.cpp: gensrc/genblitter gensrc/blitfunc.h
	gensrc/genblitter t > gensrc/blittable.cpp

gensrc/build68k:
	$(cxx) $(cppflags) $(cxxflags) src/build68k.cpp src/writelog.cpp -o gensrc/build68k

gensrc/cpudefs.cpp: gensrc/build68k src/table68k
	./gensrc/build68k < src/table68k > gensrc/cpudefs.cpp
	#python util/fix_tchar.py gensrc/cpudefs.cpp

gensrc/cpuemu_0.cpp: gensrc/gencpu
	cd gensrc && ./gencpu --optimized-flags

gensrc/cpustbl.cpp: gensrc/cpuemu_0.cpp

gensrc/cputbl.h: gensrc/cpuemu_0.cpp

gensrc/cpuemu_11.cpp: gensrc/cpuemu_0.cpp
gensrc/cpuemu_12.cpp: gensrc/cpuemu_0.cpp
gensrc/cpuemu_20.cpp: gensrc/cpuemu_0.cpp
gensrc/cpuemu_21.cpp: gensrc/cpuemu_0.cpp
gensrc/cpuemu_31.cpp: gensrc/cpuemu_0.cpp

endif

obj/gensrc-%.o: gensrc/%.cpp
	$(cxx) $(cppflags) $(cxxflags) -c $< -o $@

obj/%.o: src/%.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/zip-archiver-%.o: src/archivers/zip/%.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/dms-archiver-%.o: src/archivers/dms/%.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/jit-%.o: src/jit/%.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -c $< -o $@

obj/jit-compemu_support.o: src/jit/compemu_support.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -fpermissive -c $< -o $@

obj/bsdsocket.o: src/bsdsocket.cpp
	$(cxx) $(cppflags) $(cxxflags) $(uae_warn) -fpermissive -c $< -o $@

obj/od-fs-%.o: src/od-fs/%.cpp
	$(cxx) $(cppflags) $(cxxflags) -c $< -o $@

obj/uae.a: gensrc/blit.h gensrc/linetoscr.cpp $(uae_objects)
ifeq ($(os), macosx)
	rm -f $@
endif
	$(ar) cru $@ $(uae_objects)
ifeq ($(os), macosx)
	ranlib $@
endif

obj/fs-uae.res: src/fs-uae/fs-uae.rc
	windres $< -O coff -o $@

obj/fs-uae-%.o: src/fs-uae/%.c
	$(cc) $(cppflags) $(cflags) -c $< -o $@

#ifeq ($(os), windows)
#out/CAPSImg.dll:
#	cp $(libfs-capsimage_dir)/out/CAPSImg.dll out/
#run_deps: out/CAPSImg.dll
#else
#run_deps:
#endif

share/locale/%/LC_MESSAGES/fs-uae.mo: po/%.po
	mkdir -p share/locale/$*/LC_MESSAGES
	msgfmt --verbose $< -o $@

catalogs = \
	share/locale/cs/LC_MESSAGES/fs-uae.mo \
	share/locale/de/LC_MESSAGES/fs-uae.mo \
	share/locale/es/LC_MESSAGES/fs-uae.mo \
	share/locale/fi/LC_MESSAGES/fs-uae.mo \
	share/locale/fr/LC_MESSAGES/fs-uae.mo \
	share/locale/it/LC_MESSAGES/fs-uae.mo \
	share/locale/nb/LC_MESSAGES/fs-uae.mo \
	share/locale/pl/LC_MESSAGES/fs-uae.mo \
	share/locale/pt/LC_MESSAGES/fs-uae.mo \
	share/locale/sr/LC_MESSAGES/fs-uae.mo

mo: $(catalogs)

fs-uae: libfsemu-target obj/uae.a $(objects)
	rm -f out/fs-uae
	$(cxx) $(ldflags) $(objects) obj/uae.a $(libs) -o fs-uae

dist_dir := fs-uae-$(version)
dist_dir_launcher := fs-uae-launcher-$(version)

distdir-launcher-base:
	rm -Rf $(dist_dir_launcher)/*
	mkdir -p $(dist_dir_launcher)

	cp -a launcher/fs_uae_launcher $(dist_dir_launcher)/
	find $(dist_dir_launcher) -name "*.pyc" -delete
	cp -a launcher/README $(dist_dir_launcher)/
	cp -a launcher/COPYING $(dist_dir_launcher)/
	cp -a launcher/fs-uae-launcher.py $(dist_dir_launcher)/
	cp -a launcher/Makefile.mk $(dist_dir_launcher)/
	cp -a launcher/setup.py $(dist_dir_launcher)/
	cp -a launcher/setup_py2exe.py $(dist_dir_launcher)/
	cp -a launcher/setup_py2app.py $(dist_dir_launcher)/

	mkdir -p $(dist_dir_launcher)/debian
	cp -a launcher/debian/changelog $(dist_dir_launcher)/debian/
	cp -a launcher/debian/compat $(dist_dir_launcher)/debian/
	cp -a launcher/debian/control $(dist_dir_launcher)/debian/
	cp -a launcher/debian/copyright $(dist_dir_launcher)/debian/
	cp -a launcher/debian/rules $(dist_dir_launcher)/debian/
	cp -a launcher/debian/source $(dist_dir_launcher)/debian/

	mkdir -p $(dist_dir_launcher)/scripts
	cp -a launcher/scripts/fs-uae-launcher $(dist_dir_launcher)/scripts/
	cp -a launcher/fs-uae-launcher.spec $(dist_dir_launcher)

	cp -a launcher/share $(dist_dir_launcher)/
	find $(dist_dir_launcher)/share -name *.mo -delete
	mkdir $(dist_dir_launcher)/po/
	cp -a launcher/po/*.po $(dist_dir_launcher)/po/
	cp -a launcher/po/update.py $(dist_dir_launcher)/po/

distdir-base: distdir-launcher-base
	rm -Rf $(dist_dir)/*
	mkdir -p $(dist_dir)
	cp -a $(dist_dir_launcher) $(dist_dir)/launcher

	mkdir -p $(dist_dir)/obj
	touch $(dist_dir)/obj/.dummy
	mkdir -p $(dist_dir)/out
	touch $(dist_dir)/out/.dummy
	cp -a INSTALL README COPYING VERSION SERIES ChangeLog $(dist_dir)
	cp -a common.mk targets.mk $(dist_dir)
	# windows.mk macosx.mk debian.mk
	cp -a Makefile fs-uae.spec example.conf $(dist_dir)
	cp -a src share licenses $(dist_dir)
	find $(dist_dir)/share -name *.mo -delete
	mkdir -p $(dist_dir)/gensrc
	cp -a gensrc/*.cpp gensrc/*.h $(dist_dir)/gensrc

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

	#mkdir -p $(dist_dir)/libfs-capsimage
	#cp -a ../libfs-capsimage/Makefile $(dist_dir)/libfs-capsimage
	#cp -a ../libfs-capsimage/CAPSImage $(dist_dir)/libfs-capsimage
	#find $(dist_dir)/libfs-capsimage -name *.o -delete
	#rm -f $(dist_dir)/libfs-capsimage/CAPSImage/config.h
	#rm -f $(dist_dir)/libfs-capsimage/CAPSImage/config.log
	#rm -f $(dist_dir)/libfs-capsimage/CAPSImage/config.cache
	#rm -f $(dist_dir)/libfs-capsimage/CAPSImage/config.status
	#cp -a ../libfs-capsimage/*.txt $(dist_dir)/libfs-capsimage
	#mkdir -p $(dist_dir)/libfs-capsimage/out

	mkdir -p $(dist_dir)/po
	cp -a po/*.po $(dist_dir)/po/
	cp -a po/update.py $(dist_dir)/po/update.py
	cp -a po/update.sh $(dist_dir)/po/update.sh

	mkdir -p $(dist_dir)/doc
	cp -a doc/Default.fs-uae $(dist_dir)/doc/

	mkdir -p $(dist_dir)/macosx
	cp -a macosx/Makefile $(dist_dir)/macosx/
	cp -a macosx/fix-app.py $(dist_dir)/macosx/
	cp -a macosx/template $(dist_dir)/macosx/

	mkdir -p $(dist_dir)/windows
	cp -a windows/Makefile $(dist_dir)/windows/
	cp -a windows/replace_icon.py $(dist_dir)/windows/
	cp -a windows/fs-uae-setup.iss $(dist_dir)/windows/

	mkdir -p $(dist_dir)/debian
	cp -a debian/changelog $(dist_dir)/debian/
	cp -a debian/compat $(dist_dir)/debian/
	cp -a debian/control $(dist_dir)/debian/
	cp -a debian/copyright $(dist_dir)/debian/
	cp -a debian/rules $(dist_dir)/debian/
	cp -a debian/source $(dist_dir)/debian/

	mkdir -p $(dist_dir)/server
	cp -a server/fs_uae_netplay_server $(dist_dir)/server/
	find $(dist_dir)/server -name "*.pyc" -delete
	cp -a server/COPYING $(dist_dir)/server/
	cp -a server/README $(dist_dir)/server/
	cp -a server/setup.py $(dist_dir)/server/

	mkdir -p $(dist_dir)/server/debian
	cp -a server/debian/changelog $(dist_dir)/server/debian/
	cp -a server/debian/compat $(dist_dir)/server/debian/
	cp -a server/debian/control $(dist_dir)/server/debian/
	cp -a server/debian/copyright $(dist_dir)/server/debian/
	cp -a server/debian/rules $(dist_dir)/server/debian/
	cp -a server/debian/preinst $(dist_dir)/server/debian/
	cp -a server/debian/source $(dist_dir)/server/debian/
	cp -a server/debian/*.init $(dist_dir)/server/debian/
	cp -a server/debian/*.default $(dist_dir)/server/debian/

	mkdir -p $(dist_dir)/server/scripts
	cp -a server/scripts/fs-uae-netplay-server $(dist_dir)/server/scripts/
	cp -a server/scripts/fs-uae-game-server $(dist_dir)/server/scripts/

	#mkdir -p $(dist_dir)/launcher
	#cp -a launcher/fs_uae_launcher $(dist_dir)/launcher/
	#find $(dist_dir)/launcher -name "*.pyc" -delete
	#cp -a launcher/README $(dist_dir)/launcher/
	#cp -a launcher/COPYING $(dist_dir)/launcher/
	#cp -a launcher/fs-uae-launcher.py $(dist_dir)/launcher/
	#cp -a launcher/Makefile.mk $(dist_dir)/launcher/
	#cp -a launcher/setup.py $(dist_dir)/launcher/
	#cp -a launcher/setup_py2exe.py $(dist_dir)/launcher/
	#cp -a launcher/setup_py2app.py $(dist_dir)/launcher/

	#mkdir -p $(dist_dir)/launcher/debian
	#cp -a launcher/debian/changelog $(dist_dir)/launcher/debian/
	#cp -a launcher/debian/compat $(dist_dir)/launcher/debian/
	#cp -a launcher/debian/control $(dist_dir)/launcher/debian/
	#cp -a launcher/debian/copyright $(dist_dir)/launcher/debian/
	#cp -a launcher/debian/rules $(dist_dir)/launcher/debian/
	#cp -a launcher/debian/source $(dist_dir)/launcher/debian/

	#mkdir -p $(dist_dir)/launcher/scripts
	#cp -a launcher/scripts/fs-uae-launcher $(dist_dir)/launcher/scripts/
	#cp -a launcher/fs-uae-launcher.spec $(dist_dir)/launcher/
	#cp -a launcher/share $(dist_dir)/launcher/

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
	
	find $(dist_dir) -name *~ -delete

distdir: distdir-base
	cd $(dist_dir) && python util/update-version.py
	python util/update-version.py $(dist_dir_launcher)/fs-uae-launcher.spec
	python util/update-version.py $(dist_dir_launcher)/setup.py
	python util/update-version.py \
		$(dist_dir_launcher)/fs_uae_launcher/Version.py \
		--update-series
	python util/update-version.py $(dist_dir_launcher)/debian/changelog

distcheck: distdir
	cd $(dist_dir) && $(make)

po-dist:
	mkdir -p dist/$(series)/po/fs-uae
	cp po/*.po dist/$(series)/po/fs-uae/
	mkdir -p dist/$(series)/po/fs-uae-launcher
	cp launcher/po/*.po dist/$(series)/po/fs-uae-launcher/

dist: distdir pubfiles-source po-dist
	find $(dist_dir_launcher) -exec touch \{\} \;
	find $(dist_dir) -exec touch \{\} \;

	tar zcfv fs-uae-launcher-$(version).tar.gz $(dist_dir_launcher)
	tar zcfv fs-uae-$(version).tar.gz $(dist_dir)
	mkdir -p dist/$(series)/$(version)
	mv fs-uae-$(version).tar.gz dist/$(series)/$(version)/
	mv fs-uae-launcher-$(version).tar.gz dist/$(series)/$(version)/
	mkdir -p dist/files/
	cp doc/Default.fs-uae dist/files/
	cp server/fs_uae_netplay_server/game.py \
		dist/files/fs-uae-netplay-server.py
	cp server/fs_uae_netplay_server/game.py \
		dist/$(series)/$(version)/fs-uae-game-server-$(version).py

install:
	install -d $(DESTDIR)$(prefix)/bin
	install fs-uae $(DESTDIR)$(prefix)/bin/fs-uae
	install -d $(DESTDIR)$(prefix)/share
	cp -R share/* $(DESTDIR)$(prefix)/share

	install -d $(DESTDIR)$(docdir)
	cp README COPYING example.conf $(DESTDIR)$(docdir)

clean:
	rm -f gensrc/build68k gensrc/genblitter gensrc/gencpu gensrc/genlinetoscr
	rm -f obj/*.o obj/*.a
	rm -f out/fs-uae*
	rm -f fs-uae
	rm -f fs-uae.exe
	$(make) -C libfsemu clean

distclean: clean

include targets.mk
