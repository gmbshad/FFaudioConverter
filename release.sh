#!/bin/sh
VERSION=$(awk '{if(/VERSION = /) print $3}' FFaudioConverter.pro)
VERSION=${VERSION/QCoreApplication::setApplicationVersion(\"}
VERSION=${VERSION/\");}
VERSION=$(echo $VERSION | tr -d '\n')

# Set version in appdata.xml
sed -i "/    <release date=/c\    <release date=\"$(date +%Y-%m-%d)\" version=\"$VERSION\"/>" com.github.REDDDev.FFaudioConverter.appdata.xml

# Package
mkdir -p release
tar -cJf release/FFaudioConverter-$VERSION-src.tar.xz \
    src/ \
    translations/ \
    FFaudioConverter.pro \
    build_linux.sh \
    build_win32.bat \
    LICENSE \
    com.github.REDDDev.FFaudioConverter.appdata.xml \
    com.github.REDDDev.FFaudioConverter.desktop \
    com.github.REDDDev.FFaudioConverter.png \
    com.github.REDDDev.FFaudioConverter.ico \
    ffaudioconverter.qrc

cd release/
sha256sum FFaudioConverter-$VERSION-src.tar.xz
