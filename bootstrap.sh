#!/bin/sh
# version number is cached in autom4te.cache
rm -Rf autom4te.cache
if [ -e /opt/share/aclocal ]; then
	# extra m4 macros installed on mingw system
	aclocal -I/opt/share/aclocal
else
	aclocal
fi
autoheader
# Makefile.am is added just to silence warnings from automake, we only
# want it to install missing files
touch Makefile.am
automake --add-missing --copy
rm Makefile.am Makefile.in
autoconf
./Makefile.py
