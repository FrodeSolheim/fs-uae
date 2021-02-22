PACKAGE=`cat fsbuild/PACKAGE`
# VERSION=`python3 fsbuild/version.py`
VERSION=`cat fsbuild/VERSION`
if [ "`uname`" = "Linux" ]; then
OS=Linux
ARCH=x86_64
elif [ "`uname`" = "Darwin" ]; then
OS=macOS
ARCH=x86_64
elif [ "`uname -o`" = "Msys" ]; then
OS=Windows
ARCH=x86_64
fi
