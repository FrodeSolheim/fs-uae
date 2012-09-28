include common.mk
version = $(strip $(shell cat VERSION))
series = $(strip $(shell cat SERIES))

project_dir = home:FrodeSolheim:fs-uae-$(series)

all: package add commit

package:
	make dist
	make -f debsrc.mk

	rm -f ~/osc/$(project_dir)/fs-uae/*
	mkdir -p ~/osc/$(project_dir)/fs-uae
	cp dist/$(series)/$(version)/fs-uae-$(version).tar.gz ~/osc/$(project_dir)/fs-uae/
	cp fs-uae-$(version)/fs-uae.spec ~/osc/$(project_dir)/fs-uae/
	cp fs-uae_$(version).orig.tar.gz ~/osc/$(project_dir)/fs-uae/
	cp fs-uae_$(version)-0.debian.tar.gz ~/osc/$(project_dir)/fs-uae/
	cp fs-uae_$(version)-0.dsc ~/osc/$(project_dir)/fs-uae/

	rm -f ~/osc/$(project_dir)/fs-uae-launcher/*
	mkdir -p ~/osc/$(project_dir)/fs-uae-launcher
	cp dist/$(series)/$(version)/fs-uae-launcher-$(version).tar.gz ~/osc/$(project_dir)/fs-uae-launcher/
	cp fs-uae-launcher-$(version)/fs-uae-launcher.spec ~/osc/$(project_dir)/fs-uae-launcher/
	cp fs-uae-launcher_$(version).orig.tar.gz ~/osc/$(project_dir)/fs-uae-launcher/
	cp fs-uae-launcher_$(version)-0.debian.tar.gz ~/osc/$(project_dir)/fs-uae-launcher/
	cp fs-uae-launcher_$(version)-0.dsc ~/osc/$(project_dir)/fs-uae-launcher/

add:
	cd ~/osc/$(project_dir)/fs-uae && osc addremove .
	cd ~/osc/$(project_dir)/fs-uae-launcher && osc addremove .

commit:
	cd ~/osc/$(project_dir)/fs-uae && osc commit -m $(version)
	cd ~/osc/$(project_dir)/fs-uae-launcher && osc commit -m $(version)

include targets.mk
