#!/bin/sh -e
echo "Boostrapping fs-uae..."
# version number is cached in autom4te.cache
rm -Rf autom4te.cache
echo "Running aclocal"
if [ -e /opt/share/aclocal ]; then
	# extra m4 macros installed on mingw system
	aclocal -I/opt/share/aclocal
else
	aclocal
fi
echo "Running autoheader"
autoheader
# Makefile.am is added just to silence warnings from automake, we only
# want it to install missing files
touch Makefile.am
echo "Running automake --add-missing --copy"
automake --add-missing --copy
rm Makefile.am Makefile.in
echo "Running autoconf"
autoconf
echo "Creating Makefile.in"
python3 Makefile.py
echo "Bootstrap done, you can now run ./configure"
