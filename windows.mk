include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)

bindist: pubfiles
	make distdir
	cd $(dist_dir)/windows && make
	mkdir -p dist/$(series)/$(version)
	mv $(dist_dir)/windows/fs-uae-$(version)-windows.zip dist/$(series)/$(version)/

	cd $(dist_dir)/windows && make launcher
	cd $(dist_dir)/windows && make bindist-plus
	mv $(dist_dir)/windows/fs-uae-plus-$(version)-windows.zip dist/$(series)/$(version)/

	cd $(dist_dir)/windows && make setup
	mv $(dist_dir)/windows/fs-uae-$(version)-setup.exe dist/$(series)/$(version)/

include targets.mk
