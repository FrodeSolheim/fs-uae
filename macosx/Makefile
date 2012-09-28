include ../common.mk
version = $(strip $(shell cat ../VERSION))

bindist: bundle
	python fix-app.py
	cp -pPR ../licenses fs-uae-$(version)-macosx/
	cp ../COPYING fs-uae-$(version)-macosx/
	cp ../README fs-uae-$(version)-macosx/
	cp ../example.conf fs-uae-$(version)-macosx/
	tar zcfv fs-uae-$(version)-macosx.tar.gz fs-uae-$(version)-macosx

bundle:
	make -C ..
	rm -Rf fs-uae-$(version)-macosx
	mkdir -p fs-uae-$(version)-macosx
	mkdir -p fs-uae-$(version)-macosx/FS-UAE.app/Contents/Frameworks
	mkdir -p fs-uae-$(version)-macosx/FS-UAE.app/Contents/MacOS
	mkdir -p fs-uae-$(version)-macosx/FS-UAE.app/Contents/Resources
	cp -pPR template/* fs-uae-$(version)-macosx/FS-UAE.app/
	cp ../out/fs-uae fs-uae-$(version)-macosx/FS-UAE.app/Contents/MacOS
	cp ../../out/libfs-capsimage.dylib fs-uae-$(version)-macosx/FS-UAE.app/Contents/Frameworks
	cp -pPR ../share/* fs-uae-$(version)-macosx/FS-UAE.app/Contents/Resources
	strip fs-uae-$(version)-macosx/FS-UAE.app/Contents/MacOS/*
	#strip fs-uae-$(version)-macosx/FS-UAE.app/Contents/Frameworks/*
	cp ../icon/fs-uae.icns fs-uae-$(version)-macosx/FS-UAE.app/Contents/Resources/
	cp ../icon/fs-uae-config.icns fs-uae-$(version)-macosx/FS-UAE.app/Contents/Resources/

launcher:
	make -C ../launcher -f Makefile.mk
	mkdir -p fs-uae-$(version)-macosx
	cd ../launcher && python2.7 setup_py2app.py py2app --iconfile ../icon/fs-uae.icns
	mv ../launcher/dist/"FS-UAE Launcher.app" fs-uae-$(version)-macosx/
	cp -pPR ../launcher/share/* fs-uae-$(version)-macosx/"FS-UAE Launcher.app"/Contents/Resources

bindist-plus:
	tar zcfv fs-uae-plus-$(version)-macosx.tar.gz fs-uae-$(version)-macosx
