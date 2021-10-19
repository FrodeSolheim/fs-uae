. ./PACKAGE.FS
. fsbuild/system.sh

cd fsbuild
rm -Rf _build
mkdir -p _build
cd ..

if [ "`uname`" = "Linux" ]; then
cd dist/linux
BUILD=0 PACKAGE=0 SYSTEM_ARCH="$SYSTEM_ARCH" make
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

PLUGIN_DIR=fsbuild/_build/$PACKAGE_NAME_PRETTY

echo "[plugin]" > $PLUGIN_DIR/Plugin.ini
echo "name = $PACKAGE_NAME_PRETTY" >> $PLUGIN_DIR/Plugin.ini
echo "version = $PACKAGE_VERSION" >> $PLUGIN_DIR/Plugin.ini
unix2dos $PLUGIN_DIR/Plugin.ini
