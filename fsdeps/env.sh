# export FSDEPS_NAME=project-name
# export FSDEPS_PREFIX=$HOME/.fsdeps/$FSDEPS_NAME
export FSDEPS_PREFIX="`pwd`/fsdeps/_prefix"

export C_INCLUDE_PATH=$FSDEPS_PREFIX/include
export CPLUS_INCLUDE_PATH=$FSDEPS_PREFIX/include
export LIBRARY_PATH=$FSDEPS_PREFIX/lib
export PKG_CONFIG_PATH=$FSDEPS_PREFIX/lib/pkgconfig

export MACOSX_DEPLOYMENT_TARGET=10.9
