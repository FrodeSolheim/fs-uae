#!/usr/bin/env sh

if [ -e /usr/bin/docker.io ]; then
DOCKER="docker.io"
else
DOCKER="docker"
fi
CWD=`pwd`
UID=`id -u $USER`

if [ "$1" = "" ]; then
echo "Helper script to build Linux 32-bit binaries via Docker"
echo ""
echo "Syntax:"
echo "./i386-build.sh [setup|<command>]"
echo ""
echo "Examples:"
echo "./i386-build.sh setup"
echo "./i386-build.sh ./configure"
echo "./i386-build.sh make clean all"
echo ""
echo "You must have docker[.io] installed and run ./i386-build.sh setup"
echo "at least once. The build is done in the 'native' directory, so remember"
echo "to run make clean to clear out the 64-bit objects first (and also"
echo "when you go back to building 'natively' again)."
echo ""
exit
fi

if [ "$1" = "setup" ]; then
cd docker && ./bootstrap.sh
exit
fi

$DOCKER run --rm -t -i -u $UID -w /build -v "$CWD":/build \
fsuae/buildi386 "$@"
