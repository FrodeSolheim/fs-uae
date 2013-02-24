include ../common.mk
version = $(strip $(shell cat ../VERSION))

bindist: program
	cp -a ../licenses fs-uae-$(version)-windows/
	cp -a ../README fs-uae-$(version)-windows/README.txt
	cp -a ../COPYING fs-uae-$(version)-windows/COPYING.txt
	cp -a ../example.conf fs-uae-$(version)-windows
	zip -r fs-uae-$(version)-windows.zip fs-uae-$(version)-windows

program:
	make -C ..
	rm -Rf fs-uae-$(version)-windows
	mkdir fs-uae-$(version)-windows
	cp /mingw/bin/zlib1.dll fs-uae-$(version)-windows
	cp /mingw/bin/libglib-2.0-0.dll fs-uae-$(version)-windows
	cp /mingw/bin/libgthread-2.0-0.dll fs-uae-$(version)-windows
	cp /mingw/bin/intl.dll fs-uae-$(version)-windows
	cp /mingw/bin/libpng*.dll fs-uae-$(version)-windows
	cp /mingw/bin/*freetype*.dll fs-uae-$(version)-windows
	cp /mingw/bin/libgcc_s_*.dll fs-uae-$(version)-windows
	cp /mingw/bin/libstdc++*.dll fs-uae-$(version)-windows
	cp /mingw/bin/libintl*.dll fs-uae-$(version)-windows
	cp /mingw/bin/libiconv*.dll fs-uae-$(version)-windows
	cp /mingw/bin/SDL.dll fs-uae-$(version)-windows
	cp /mingw/bin/OpenAL32.dll fs-uae-$(version)-windows
	cp -a ../fs-uae.exe fs-uae-$(version)-windows/fs-uae.exe
	cp -a ../../CAPSImg.dll fs-uae-$(version)-windows || cp -a ../CAPSImg.dll fs-uae-$(version)-windows
	cp -a ../share fs-uae-$(version)-windows
	strip fs-uae-$(version)-windows/*.exe
	strip fs-uae-$(version)-windows/*.dll

launcher:
	make -C ../launcher -f Makefile.mk
	rm -Rf ../launcher/dist
	cd ../launcher && python setup_py2exe.py py2exe
	rm -Rf fs-uae-$(version)-windows/launcher
	mv ../launcher/dist fs-uae-$(version)-windows/launcher
	cp -a ../launcher/share fs-uae-$(version)-windows/launcher/
	mv fs-uae-$(version)-windows/launcher/fs-uae-launcher.exe "fs-uae-$(version)-windows/launcher/FS-UAE Launcher.exe"
	python replace_icon.py "fs-uae-$(version)-windows/launcher/FS-UAE Launcher.exe" ../icon/fs-uae-launcher.ico
	cp -a /mingw/Microsoft.VC90.CRT fs-uae-$(version)-windows/launcher/

bindist-plus:
	zip -r fs-uae-plus-$(version)-windows.zip fs-uae-$(version)-windows

setup:
	iscc fs-uae-setup.iss
