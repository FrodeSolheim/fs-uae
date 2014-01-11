include ../common.mk
version = $(strip $(shell cat ../VERSION))
emulator_dir = fs-uae_$(version)_windows
launcher_dir = fs-uae-launcher_$(version)_windows
gamecenter_dir = fs-uae-game-center_$(version)_windows
portable_dir = fs-uae-suite_$(version)_windows_portable

#all: bindists setups
#bindists: bindist-emulator bindist-launcher bindist-game-center bindist-portable
#setups: setup-launcher setup-game-center setup-suite

all: progdir zipfile setup

clean:
	rm -Rf fs-uae-*_windows*
	rm -Rf fs-uae_*_windows*

#emulator:
progdir:
	rm -Rf $(emulator_dir)
	mkdir $(emulator_dir)

	make -C ..
	cp /mingw/bin/zlib1.dll $(emulator_dir)
	cp /mingw/bin/libglib-2.0-0.dll $(emulator_dir)
	cp /mingw/bin/libgthread-2.0-0.dll $(emulator_dir)
	cp /mingw/bin/intl.dll $(emulator_dir)
	cp /mingw/bin/libpng*.dll $(emulator_dir)
	cp /mingw/bin/*freetype*.dll $(emulator_dir)
	cp /mingw/bin/libgcc_s_*.dll $(emulator_dir)
	cp /mingw/bin/libstdc++*.dll $(emulator_dir)
	cp /mingw/bin/libintl*.dll $(emulator_dir)
	cp /mingw/bin/libiconv*.dll $(emulator_dir)
	cp /mingw/bin/SDL.dll $(emulator_dir)
	cp /mingw/bin/OpenAL32.dll $(emulator_dir)
	cp -a ../fs-uae.exe $(emulator_dir)/
	cp -a ../fs-uae-device-helper.exe $(emulator_dir)/
	cp -a ../../CAPSImg.dll $(emulator_dir) || cp -a ../CAPSImg.dll $(emulator_dir)
	cp -a ../share $(emulator_dir)/share
	cp -a ../licenses $(emulator_dir)/licenses
	cp -a ../README $(emulator_dir)/README.txt
	cp -a ../COPYING $(emulator_dir)/COPYING.txt
	strip $(emulator_dir)/*.exe
	strip $(emulator_dir)/*.dll
	python /c/signtool.py $(emulator_dir)/*.exe

zipfile:
	cd $(emulator_dir) && zip -9 -r ../../$(emulator_dir).zip *

setup:
	sed s/9.8.7/${version}/g fs-uae.iss > fs-uae.iss.tmp
	iscc fs-uae.iss.tmp
	python /c/signtool.py ../fs-uae_$(version)_windows.exe
	rm fs-uae.iss.tmp

launcher:
	rm -Rf $(launcher_dir)
	#mkdir $(launcher_dir)

	make -C ../launcher -f Makefile.mk
	rm -Rf ../launcher/dist
	cd ../launcher && python setup_py2exe.py py2exe
	#rm -Rf $(launcher_dir)/Launcher
	mv ../launcher/dist $(launcher_dir)

	cp -a ../launcher/share $(launcher_dir)/Share
	mkdir -p $(launcher_dir)/share/fs-uae-launcher/fs_uae_launcher
	cp -pPR ../launcher/fs_uae_launcher/res $(launcher_dir)/share/fs-uae-launcher/fs_uae_launcher/

	mv $(launcher_dir)/fs-uae-launcher.exe $(launcher_dir)/"FS-UAE Launcher.exe"
	python replace_icon.py $(launcher_dir)/"FS-UAE Launcher.exe" ../icon/fs-uae-launcher.ico
	python /c/signtool.py $(launcher_dir)/"FS-UAE Launcher.exe"
	cp -a /mingw/Microsoft.VC90.CRT $(launcher_dir)/
	cp -a $(emulator_dir) $(launcher_dir)/FS-UAE

bindist-launcher: launcher
	cd $(launcher_dir) && zip -9 -r ../$(launcher_dir).zip *

game-center:
	rm -Rf $(gamecenter_dir)
	#mkdir $(gamecenter_dir)

	make -C ../launcher -f Makefile.mk
	rm -Rf ../launcher/dist
	cd ../launcher && python setup_py2exe.py py2exe --game-center
	#rm -Rf $(gamecenter_dir)/"Game Center"
	mv ../launcher/dist $(gamecenter_dir)

	cp -a ../launcher/share $(gamecenter_dir)/Share
	mkdir -p $(gamecenter_dir)/share/fs-uae-game-center/fengestad.input
	cp -pPR ../launcher/fengestad/input/res $(gamecenter_dir)/share/fs-uae-game-center/fengestad.input/
	mkdir -p $(gamecenter_dir)/share/fs-uae-game-center/fs_uae_launcher
	# don't really need all files, just adf save disk and possible some other, these resources
	# should be moved to another package anyway
	cp -pPR ../launcher/fs_uae_launcher/res $(gamecenter_dir)/share/fs-uae-game-center/fs_uae_launcher/
	mkdir -p $(gamecenter_dir)/share/fs-uae-game-center/fengestad.gamecenter
	cp -pPR ../launcher/fengestad/gamecenter/res $(gamecenter_dir)/share/fs-uae-game-center/fengestad.gamecenter

	mv $(gamecenter_dir)/fs-uae-game-center.exe $(gamecenter_dir)/"FS-UAE Game Center.exe"
	python replace_icon.py $(gamecenter_dir)/"FS-UAE Game Center.exe" ../icon/fs-uae-launcher.ico
	python /c/signtool.py $(gamecenter_dir)/"FS-UAE Game Center.exe"
	cp -a /mingw/Microsoft.VC90.CRT $(gamecenter_dir)/
	cp -a $(emulator_dir) $(gamecenter_dir)/FS-UAE

bindist-game-center: game-center
	cd $(gamecenter_dir) && zip -9 -r ../$(gamecenter_dir).zip *

portable:
	rm -Rf $(portable_dir)
	mkdir $(portable_dir)
	touch $(portable_dir)/Portable.ini
	mkdir $(portable_dir)/Kickstarts
	mkdir $(portable_dir)/Configurations
	mkdir $(portable_dir)/Floppies
	mkdir $(portable_dir)/CD-ROMs
	mkdir $(portable_dir)/"Hard Drives"
	mkdir $(portable_dir)/Cache
	mkdir $(portable_dir)/Data
	# mkdir $(portable_dir)/Cache/Temp
	mkdir $(portable_dir)/Downloads
	mkdir $(portable_dir)/Themes
	mkdir $(portable_dir)/"Save States"

	cp launcher-proxy.exe $(portable_dir)/"FS-UAE Launcher.exe"
	python replace_icon.py $(portable_dir)/"FS-UAE Launcher.exe" ../icon/fs-uae-launcher.ico
	python /c/signtool.py $(portable_dir)/"FS-UAE Launcher.exe"

	cp game-center-proxy.exe $(portable_dir)/"FS-UAE Game Center.exe"
	python replace_icon.py $(portable_dir)/"FS-UAE Game Center.exe" ../icon/fs-uae-launcher.ico
	python /c/signtool.py $(portable_dir)/"FS-UAE Game Center.exe"

	# mkdir $(portable_dir)/Programs
	# mkdir $(portable_dir)/Programs/Windows
	mkdir $(portable_dir)/Windows
	# cp -a $(emulator_dir) $(portable_dir)/Programs/Windows/FS-UAE
	cp -a  $(launcher_dir) $(portable_dir)/Windows/"FS-UAE Launcher"
	cp -a  $(gamecenter_dir) $(portable_dir)/Windows/"FS-UAE Game Center"

	# rm -R FS-UAE/Windows/Emulator/launcher

bindist-portable: portable
	cd $(portable_dir) && zip -9 -r ../$(portable_dir).zip *

setup-launcher:
	iscc launcher.iss
	python /c/signtool.py fs-uae-launcher_$(version)_windows.exe

setup-game-center:
	iscc game-center.iss
	python /c/signtool.py fs-uae-game-center_$(version)_windows.exe

setup-suite:
	# iscc suite.iss
	# python /c/signtool.py fs-uae-suite_$(version)_windows.exe
