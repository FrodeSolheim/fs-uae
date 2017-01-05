all: build-docs

build-docs:
	cd .. && doc/update.py
	python3 build.py

publish: build-docs
	rsync -av --progress --delete dist/ fengestad.no:fs-uae.net/htdocs/devel/docs/
