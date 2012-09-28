include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))
dist_dir = fs-uae-$(version)

bindist: pubfiles
	make distdir
	cd $(dist_dir) && fakeroot debian/rules binary
	mkdir -p dist/$(series)/$(version)
	mv fs-uae_$(version)-*deb dist/$(series)/$(version)/
	cd $(dist_dir)/server && fakeroot debian/rules binary-indep
	mv $(dist_dir)/fs-uae-netplay-server_$(version)-*deb dist/$(series)/$(version)/
	cd $(dist_dir)/launcher && fakeroot debian/rules binary-indep
	mv $(dist_dir)/fs-uae-launcher_$(version)-*deb dist/$(series)/$(version)/

include targets.mk
