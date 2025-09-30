export ARCH=x86_64
export ARCHFLAGS="-arch x86_64"
export MACOSX_DEPLOYMENT_TARGET=10.9
export CROSS_COMPILE=x86_64-apple-darwin
make clean
rm -r build
make install CROSS_COMPILE=$CROSS_COMPILE ARCH=$ARCH ARCHFLAGS="$ARCHFLAGS" 
