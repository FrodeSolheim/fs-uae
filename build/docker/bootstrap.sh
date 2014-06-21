#!/usr/bin/env sh

if [ -e /usr/bin/docker.io ]; then
DOCKER="docker.io"
mkdir -p bin
rm -f bin/docker
ln -s /usr/bin/docker.io bin/docker
CWD=`pwd`
export PATH="$CWD/bin:$PATH"
else
DOCKER="docker"
fi

sh mkimage-debootstrap.sh -a i386 fsuae/ubuntui386 trusty
sudo $DOCKER build -t fsuae/buildi386 .
