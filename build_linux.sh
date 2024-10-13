#!/bin/sh
mkdir -p build
cd build/

qmake PREFIX=/usr ../FFaudioConverter.pro -spec linux-icc-64 CONFIG+=release
make
strip -s ffaudioconverter*
