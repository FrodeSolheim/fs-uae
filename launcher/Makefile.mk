prefix := /usr

all: mo

share/locale/%/LC_MESSAGES/fs-uae-launcher.mo: po/%.po
	mkdir -p share/locale/$*/LC_MESSAGES
	msgfmt --verbose $< -o $@

catalogs = \
	share/locale/de/LC_MESSAGES/fs-uae-launcher.mo \
	share/locale/fr/LC_MESSAGES/fs-uae-launcher.mo \
	share/locale/it/LC_MESSAGES/fs-uae-launcher.mo \
	share/locale/nb/LC_MESSAGES/fs-uae-launcher.mo \
	share/locale/pl/LC_MESSAGES/fs-uae-launcher.mo \
	share/locale/sr/LC_MESSAGES/fs-uae-launcher.mo \

mo: $(catalogs)

install:
	mkdir -p $(DESTDIR)$(prefix)/share
	cp -a share/* $(DESTDIR)$(prefix)/share

	mkdir -p $(DESTDIR)$(prefix)/share/doc/fs-uae-launcher
	cp -a README COPYING $(DESTDIR)$(prefix)/share/doc/fs-uae-launcher
