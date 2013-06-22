include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)

bindist: pubfiles
	make distdir
	cd $(dist_dir)/macosx && make
	mkdir -p dist/$(series)/$(version)
	mv $(dist_dir)/macosx/fs-uae-emulator_$(version)_macosx.tar.gz dist/$(series)/$(version)/
	# mv $(dist_dir)/macosx/fs-uae-launcher_$(version)_macosx.tar.gz dist/$(series)/$(version)/
	# mv $(dist_dir)/macosx/fs-uae-game-center_$(version)_macosx.tar.gz dist/$(series)/$(version)/
	mv $(dist_dir)/macosx/fs-uae-suite_$(version)_macosx_bundles.tar.gz dist/$(series)/$(version)/
	mv $(dist_dir)/macosx/fs-uae-suite_$(version)_macosx_portable.tar.gz dist/$(series)/$(version)/

include targets.mk
