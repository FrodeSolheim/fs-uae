version = $(strip $(shell . ../../PACKAGE.FS; echo $$PACKAGE_VERSION))

os = macos
arch = $(strip $(shell python3 ../arch.py))

os_arch = $(os)_$(arch)
ver_os_arch = $(version)_$(os_arch)
dname = fs-uae_$(ver_os_arch)

all: bindist

clean:
	rm -Rf ../macos/fs-uae* FS-UAE

bindist: bundle
	tar Jcfv ../../$(dname).tar.xz FS-UAE.app

plugin_dir = FS-UAE
os_arch_dir = ${plugin_dir}/macOS/${arch}
data_dir = ${plugin_dir}/Data
licenses_dir = ${plugin_dir}/Licenses

plugin: plugin-no-archive
	tar Jcfv ../../FS-UAE_${version}_macOS_$(arch).tar.xz FS-UAE

plugin-no-archive: bundle
	rm -Rf FS-UAE
	mkdir -p FS-UAE/macOS/$(arch)

	rm -Rf FS-UAE
	mkdir -p ${plugin_dir}
	echo ${version} > ${plugin_dir}/Version.txt
	cp ../../README ${plugin_dir}/ReadMe.txt

	mkdir -p ${os_arch_dir}
	echo ${version} > ${os_arch_dir}/Version.txt
	mv FS-UAE.app FS-UAE/macOS/$(arch)/

	# mkdir -p ${licenses_dir}

	# FIXME: Move locale and fs-uae.dat out of .app and into Data
	# mkdir -p ${data_dir}
	# cp -a fsemu/data/* ${data_dir}/

bundle:
	make -C ../..
	rm -Rf FS-UAE.app
	mkdir -p FS-UAE.app/Contents/Frameworks
	mkdir -p FS-UAE.app/Contents/MacOS
	mkdir -p FS-UAE.app/Contents/Resources
	mkdir -p FS-UAE.app/Contents/
	cp Info.plist FS-UAE.app/Contents/
	cp ./PkgInfo FS-UAE.app/Contents/
	cp ../../fs-uae FS-UAE.app/Contents/MacOS/
	cp ../../fs-uae-device-helper FS-UAE.app/Contents/MacOS/

	cp ../../fs-uae.dat FS-UAE.app/Contents/Resources/
	mkdir -p FS-UAE.app/Contents/Resources/fs-uae
	touch FS-UAE.app/Contents/Resources/fs-uae/share-dir
	cp -pPR ../../share/locale FS-UAE.app/Contents/Resources/Locale

	cp -pPR ../../data/* FS-UAE.app/Contents/Resources

	strip FS-UAE.app/Contents/MacOS/*
	cp ./fs-uae.icns FS-UAE.app/Contents/Resources/
	cp ./fs-uae-config.icns FS-UAE.app/Contents/Resources/
	# cp -pPR ./../../licenses FS-UAE.app/Contents/Resources/
	# cp ./../../COPYING FS-UAE.app/Contents/Resources/
	# cp ./../../README FS-UAE.app/Contents/Resources/
	./standalone.py FS-UAE.app
	./sign.py FS-UAE.app
	./notarize.py FS-UAE.app no.fengestad.fs-uae
