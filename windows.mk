include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)

bindist: pubfiles
	make distdir
	cd $(dist_dir)/windows && make
	mkdir -p dist/$(series)/$(version)
	mv $(dist_dir)/windows/fs-uae-emulator_$(version)_windows.zip dist/$(series)/$(version)/
	# mv $(dist_dir)/windows/fs-uae-launcher_$(version)_windows.zip dist/$(series)/$(version)/
	# mv $(dist_dir)/windows/fs-uae-game-center_$(version)_windows.zip dist/$(series)/$(version)/
	mv $(dist_dir)/windows/fs-uae-suite_$(version)_windows_portable.zip dist/$(series)/$(version)/
	mv $(dist_dir)/windows/fs-uae-suite_$(version)_windows_setup.exe dist/$(series)/$(version)/

include targets.mk
