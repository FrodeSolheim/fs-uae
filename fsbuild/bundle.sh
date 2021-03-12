cd fsbuild
rm -Rf _build
mkdir -p _build
cd ..

if [ "`uname`" = "Linux" ]; then
cd dist/linux
BUILD=0 PACKAGE=0 make
mv FS-UAE ../../fsbuild/_build
cd ../..
elif [ "`uname`" = "Darwin" ]; then
cd dist/macos
SIGN=0 NOTARIZE=0 make plugin-no-archive
mv FS-UAE ../../fsbuild/_build
cd ../..
elif [ "`uname -o`" = "Msys" ]; then
cd dist/windows
make progdir
mv FS-UAE ../../fsbuild/_build
cd ../..
fi
