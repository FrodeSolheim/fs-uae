PACKAGE=`cat fsbuild/PACKAGE`
# VERSION=`python3 fsbuild/version.py`
VERSION=`cat fsbuild/VERSION`
if [ "`uname`" = "Linux" ]; then
OS=Linux
SYSTEM=Linux
ARCH=x86_64
elif [ "`uname`" = "Darwin" ]; then
SYSTEM=macOS
OS=macOS
ARCH=x86_64
elif [ "`uname -o`" = "Msys" ]; then
SYSTEM=Windows
OS=Windows
ARCH=x86_64
fi
