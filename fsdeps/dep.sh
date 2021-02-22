. fsdeps/env.sh

export PREFIX="$FSDEPS_PREFIX"
export REINSTALL="$FSDEPS_REINSTALL"
export INSTALLED="$PREFIX/installed/$PACKAGE-$REVISION"

mkdir -p $PREFIX/installed

if test -e $INSTALLED ; then
if [ "$REINSTALL" != "1" ]; then
echo "$INSTALLED"
exit 0
fi
fi

echo
echo $PACKAGE
