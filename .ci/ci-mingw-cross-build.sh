#!/bin/sh

export PREFIX=/root/EFL/ewpi_64
export HOST=x86_64-w64-mingw32
export DISABLE_STATIC="--disable-static"
export CFLAGS="-pipe -g3 -ggdb3 -Og"
export CXXFLAGS="-pipe -g3 -ggdb3 -Og"
export CPPFLAGS="-I$PREFIX/include -DECORE_WIN32_WIP_POZEFLKSD"
export LDFLAGS="-L$PREFIX/lib/"
#export PKG_CONFIG_LIBDIR=$PREFIX/lib/pkgconfig/
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig/

# epp is looked for in a hardcoded /usr/local/lib, need to fix in efl
ln -s /usr/lib64/edje/ /usr/local/lib/edje

autoreconf -vif
./configure --prefix=$PREFIX --host=$HOST --with-eolian-gen=/usr/bin/eolian_gen  --with-edje-cc=/usr/bin/edje_cc --disable-static --with-tests=regular --with-crypto=openssl --disable-gstreamer1 --disable-gstreamer --disable-libmount --disable-valgrind --disable-avahi --disable-spectre --disable-libraw --disable-librsvg --disable-pulseaudio --disable-cxx-bindings --disable-image-loader-jp2k
make
