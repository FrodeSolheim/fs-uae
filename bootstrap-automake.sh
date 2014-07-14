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
echo "Running automake"
python3 Makefile.py Makefile.am
automake --add-missing --copy
echo "Running autoconf"
autoconf
echo "Bootstrap done, you can now run ./configure"
