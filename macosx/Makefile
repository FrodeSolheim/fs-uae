include ../common.mk
version = $(strip $(shell cat ../VERSION))
game_center_app = fs-uae-game-center_$(version)_macosx/FS-UAE Game Center.app
game_center_resources = $(game_center_app)/Contents/Resources

# all: bindist bindist-launcher bindist-game-center bindist-portable bindist-bundles

all: bindist

clean:
	rm -Rf ../macosx/fs-uae*

bindist: bundle
	cd fs-uae_$(version)_macosx && GZIP=-9 tar zcfv ../../fs-uae_$(version)_macosx.tar.gz *

bundle:
	make -C ..
	rm -Rf fs-uae_$(version)_macosx
	mkdir -p fs-uae_$(version)_macosx
	mkdir -p fs-uae_$(version)_macosx/FS-UAE.app/Contents/Frameworks
	mkdir -p fs-uae_$(version)_macosx/FS-UAE.app/Contents/MacOS
	mkdir -p fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources
	cp -pPR template/* fs-uae_$(version)_macosx/FS-UAE.app/
	cp ../fs-uae fs-uae_$(version)_macosx/FS-UAE.app/Contents/MacOS/
	cp ../fs-uae-device-helper fs-uae_$(version)_macosx/FS-UAE.app/Contents/MacOS/
	cp ../../libfs-capsimage.dylib fs-uae_$(version)_macosx/FS-UAE.app/Contents/Frameworks || cp ../libfs-capsimage.dylib fs-uae_$(version)_macosx/FS-UAE.app/Contents/Frameworks
	cp -pPR ../share/* fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources
	strip fs-uae_$(version)_macosx/FS-UAE.app/Contents/MacOS/*
	#strip fs-uae_$(version)_macosx/FS-UAE.app/Contents/Frameworks/*
	cp ../icon/fs-uae.icns fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../icon/fs-uae-config.icns fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp -pPR ../licenses fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../COPYING fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cp ../README fs-uae_$(version)_macosx/FS-UAE.app/Contents/Resources/
	cd fs-uae_$(version)_macosx && python ../fs-make-standalone-app.py FS-UAE.app

launcher:
	make -C ../launcher -f Makefile.mk
	rm -Rf fs-uae-launcher_$(version)_macosx	
	mkdir -p fs-uae-launcher_$(version)_macosx
	cd ../launcher && python2.7 setup_py2app.py py2app --iconfile ../icon/fs-uae-launcher.icns
	mv ../launcher/dist/"FS-UAE Launcher.app" fs-uae-launcher_$(version)_macosx/
	cp -pPR ../launcher/share fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app"/Contents/Resources/

	cp -pPR fs-uae_$(version)_macosx/FS-UAE.app fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app"/Contents/Resources/

bindist-launcher: launcher
	cd fs-uae-launcher_$(version)_macosx && GZIP=-9 tar zcfv ../fs-uae-launcher_$(version)_macosx.tar.gz *

game-center:
	make -C ../launcher -f Makefile.mk
	rm -Rf fs-uae-game-center_$(version)_macosx	
	mkdir -p fs-uae-game-center_$(version)_macosx
	cd ../launcher && python2.7 setup_py2app.py --game-center py2app --iconfile ../icon/fs-uae-launcher.icns
	mv ../launcher/dist/"FS-UAE Game Center.app" "$(game_center_app)"

	cp -pPR ../launcher/share "$(game_center_resources)"/
	mkdir -p "$(game_center_resources)"/share/fs-uae-game-center/fengestad.input
	cp -pPR ../launcher/fengestad/input/res "$(game_center_resources)"/share/fs-uae-game-center/fengestad.input
	mkdir -p "$(game_center_resources)"/share/fs-uae-game-center/fengestad.gamecenter
	cp -pPR ../launcher/fengestad/gamecenter/res "$(game_center_resources)"/share/fs-uae-game-center/fengestad.gamecenter

	cp -pPR fs-uae_$(version)_macosx/FS-UAE.app "$(game_center_resources)"/

bindist-game-center: game-center
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
	cp -pPR "$(game_center_app)" fs-uae-suite_$(version)_macosx_portable/
	#mkdir fs-uae-suite_$(version)_macosx_portable/Programs
	#mkdir fs-uae-suite_$(version)_macosx_portable/Programs/"Mac OS X"
	#cp -pPR fs-uae_$(version)_macosx/FS-UAE.app fs-uae-suite_$(version)_macosx_portable/Programs/"Mac OS X"

bindist-portable: portable
	cd fs-uae-suite_$(version)_macosx_portable && GZIP=-9 tar zcfv ../fs-uae-suite_$(version)_macosx_portable.tar.gz *

bundles:
	rm -Rf fs-uae-suite_$(version)_macosx
	mkdir fs-uae-suite_$(version)_macosx
	cp -pPR fs-uae-launcher_$(version)_macosx/"FS-UAE Launcher.app" fs-uae-suite_$(version)_macosx/
	cp -pPR "$(game_center_app)" fs-uae-suite_$(version)_macosx/
	#cp -pPR fs-uae_$(version)_macosx/"FS-UAE.app" fs-uae_$(version)_macosx/"FS-UAE Launcher.app"/Contents/Resources/

bindist-bundles: bundles
	cd fs-uae-suite_$(version)_macosx && GZIP=-9 tar zcfv ../fs-uae-suite_$(version)_macosx.tar.gz *
