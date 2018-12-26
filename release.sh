#!/bin/sh
# Generate translations
lrelease FFaudioConverter.pro

VERSION=$(awk '{if(/QCoreApplication::setApplicationVersion/) print $0}' src/main.cpp)
VERSION=${VERSION/QCoreApplication::setApplicationVersion(\"}
VERSION=${VERSION/\");}
VERSION=$(echo $VERSION | tr -d '\n')

# Set version in appdata.xml
sed -i "/    <release date=/c\    <release date=\"$(date +%Y-%m-%d)\" version=\"$VERSION\"/>" com.github.Bleuzen.FFaudioConverter.appdata.xml

# Package
mkdir -p release
tar -cJf release/FFaudioConverter-$VERSION.tar.xz \
    src/ \
    translations/ \
    FFaudioConverter.pro \
    build_linux.sh \
    build_win32.bat \
    LICENSE \
    com.github.Bleuzen.FFaudioConverter.appdata.xml \
    com.github.Bleuzen.FFaudioConverter.desktop \
    com.github.Bleuzen.FFaudioConverter.png \
    com.github.Bleuzen.FFaudioConverter.ico \
    ffaudioconverter.qrc

cd release/
sha256sum FFaudioConverter-$VERSION.tar.xz
