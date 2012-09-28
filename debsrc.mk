include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)
dist_dir_launcher = fs-uae-launcher-$(version)

debseries := unstable
debversion := 0

bindist: pubfiles
	make dist
	#rm -f fs-uae_$(version).orig.tar.gz
	#ln -s dist/$(series)/$(version)/fs-uae-$(version).tar.gz fs-uae_$(version).orig.tar.gz
	test -f fs-uae_$(version).orig.tar.gz || cp dist/$(series)/$(version)/fs-uae-$(version).tar.gz fs-uae_$(version).orig.tar.gz

	sed -i "s/-0[)] unstable;/-$(debversion)) $(debseries);/g" $(dist_dir)/debian/changelog
	cd $(dist_dir) && dpkg-buildpackage -S -us -uc

	#rm -f fs-uae-launcher_$(version).orig.tar.gz
	test -f fs-uae-launcher_$(version).orig.tar.gz || cp dist/$(series)/$(version)/fs-uae-launcher-$(version).tar.gz fs-uae-launcher_$(version).orig.tar.gz

	sed -i "s/-0[)] unstable;/-$(debversion)) $(debseries);/g" $(dist_dir_launcher)/debian/changelog
	cd $(dist_dir_launcher) && dpkg-buildpackage -S -us -uc

include targets.mk
