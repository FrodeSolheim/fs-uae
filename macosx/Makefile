include ../common.mk
version = $(strip $(shell cat ../VERSION))

all: bindist bindist-launcher bindist-gamecenter bindist-portable bindist-bundles

clean:
	rm -Rf ../macosx/fs-uae*

bindist: bundle
	cd fs-uae-emulator_$(version)_macosx && GZIP=-9 tar zcfv ../fs-uae-emulator_$(version)_macosx.tar.gz *

bundle:
	make -C ..
	rm -Rf fs-uae-emulator_$(version)_macosx
	mkdir -p fs-uae-emulator_$(version)_macosx
	mkdir -p fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Frameworks
	mkdir -p fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/MacOS
	mkdir -p fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources
	cp -pPR template/* fs-uae-emulator_$(version)_macosx/FS-UAE.app/
	cp ../fs-uae fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/MacOS
	cp ../../libfs-capsimage.dylib fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Frameworks || cp ../libfs-capsimage.dylib fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Frameworks
	cp -pPR ../share/* fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources
	strip fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/MacOS/*
	#strip fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Frameworks/*
	cp ../icon/fs-uae.icns fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../icon/fs-uae-config.icns fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp -pPR ../licenses fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../COPYING fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../README fs-uae-emulator_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cd fs-uae-emulator_$(version)_macosx && python2.7 ../fix-app.py

launcher:
	make -C ../launcher -f Makefile.mk
	rm -Rf fs-uae-launcher_$(version)_macosx	
	mkdir -p fs-uae-launcher_$(version)_macosx
	cd ../launcher && python2.7 setup_py2app.py py2app --iconfile ../icon/fs-uae-launcher.icns
	mv ../launcher/dist/"FS-UAE Launcher.app" fs-uae-launcher_$(version)_macosx/
	cp -pPR ../launcher/share fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app"/Contents/Resources/

	cp -pPR fs-uae-emulator_$(version)_macosx/FS-UAE.app fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app"/Contents/Resources/

bindist-launcher: launcher
	cd fs-uae-launcher_$(version)_macosx && GZIP=-9 tar zcfv ../fs-uae-launcher_$(version)_macosx.tar.gz *

gamecenter:
	make -C ../launcher -f Makefile.mk
	rm -Rf fs-uae-game-center_$(version)_macosx	
	mkdir -p fs-uae-game-center_$(version)_macosx
	cd ../launcher && python2.7 setup_py2app.py --game-center py2app --iconfile ../icon/fs-uae-launcher.icns
	mv ../launcher/dist/"FS-UAE Game Center.app" fs-uae-game-center_$(version)_macosx/

	cp -pPR ../launcher/share fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/
	mkdir -p fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/share/fs-uae-game-center/fengestad.input
	cp -pPR ../launcher/fengestad/input/res fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/share/fs-uae-game-center/fengestad.input
	mkdir -p fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/share/fs-uae-game-center/fengestad.gamecenter
	cp -pPR ../launcher/fengestad/gamecenter/res fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/share/fs-uae-game-center/fengestad.gamecenter

	cp -pPR fs-uae-emulator_$(version)_macosx/FS-UAE.app fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app"/Contents/Resources/

bindist-gamecenter: gamecenter
	cd fs-uae-game-center_$(version)_macosx && GZIP=-9 tar zcfv ../fs-uae-game-center_$(version)_macosx.tar.gz *

portable:
	rm -Rf fs-uae-suite_$(version)_macosx_portable
	mkdir fs-uae-suite_$(version)_macosx_portable
	touch fs-uae-suite_$(version)_macosx_portable/Portable.ini
	mkdir fs-uae-suite_$(version)_macosx_portable/Kickstarts
	mkdir fs-uae-suite_$(version)_macosx_portable/Configurations
	mkdir fs-uae-suite_$(version)_macosx_portable/Floppies
	mkdir fs-uae-suite_$(version)_macosx_portable/CD-ROMs
	mkdir fs-uae-suite_$(version)_macosx_portable/"Hard Drives"
	mkdir fs-uae-suite_$(version)_macosx_portable/Cache
	mkdir fs-uae-suite_$(version)_macosx_portable/Data
	#mkdir fs-uae-suite_$(version)_macosx_portable/Cache/Temp
	mkdir fs-uae-suite_$(version)_macosx_portable/Downloads
	mkdir fs-uae-suite_$(version)_macosx_portable/Themes
	mkdir fs-uae-suite_$(version)_macosx_portable/"Save States"
	cp -pPR fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app" fs-uae-suite_$(version)_macosx_portable/
	cp -pPR fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app" fs-uae-suite_$(version)_macosx_portable/
	#mkdir fs-uae-suite_$(version)_macosx_portable/Programs
	#mkdir fs-uae-suite_$(version)_macosx_portable/Programs/"Mac OS X"
	#cp -pPR fs-uae-emulator_$(version)_macosx/FS-UAE.app fs-uae-suite_$(version)_macosx_portable/Programs/"Mac OS X"

bindist-portable: portable
	cd fs-uae-suite_$(version)_macosx_portable && GZIP=-9 tar zcfv ../fs-uae-suite_$(version)_macosx_portable.tar.gz *

bundles:
	rm -Rf fs-uae-suite_$(version)_macosx_bundles
	mkdir fs-uae-suite_$(version)_macosx_bundles
	cp -pPR fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app" fs-uae-suite_$(version)_macosx_bundles/
	cp -pPR fs-uae-game-center_$(version)_macosx/"FS-UAE Game Center.app" fs-uae-suite_$(version)_macosx_bundles/
	#cp -pPR fs-uae-emulator_$(version)_macosx/"FS-UAE.app" fs-uae-emulator_$(version)_macosx_bundles/"FS-UAE Launcher.app"/Contents/Resources/

bindist-bundles: bundles
	cd fs-uae-suite_$(version)_macosx_bundles && GZIP=-9 tar zcfv ../fs-uae-suite_$(version)_macosx_bundles.tar.gz *
