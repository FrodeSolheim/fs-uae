pubfiles:
	mkdir -p dist

pubfiles-source:
	mkdir -p dist
	cp -a Changelog example.conf dist/
	cp -a fs-uae-$(version)/README dist/
	chmod 0644 dist/README
	chmod 0644 dist/Changelog
	chmod 0644 dist/example.conf

