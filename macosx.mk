include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)

bindist: pubfiles
	make distdir
	cd $(dist_dir)/macosx && make
	mkdir -p dist/$(series)/$(version)
	mv $(dist_dir)/macosx/fs-uae-$(version)-macosx.tar.gz dist/$(series)/$(version)/

	cd $(dist_dir)/macosx && make launcher
	cd $(dist_dir)/macosx && make bindist-plus
	mv $(dist_dir)/macosx/fs-uae-plus-$(version)-macosx.tar.gz dist/$(series)/$(version)/

include targets.mk
