mkdir -p build; cd build
qmake ../FFaudioConverter.pro -spec macx-g++ CC=/usr/bin/cc CXX=/usr/bin/c++ -Wnone CONFIG+=sdk_no_version_check  CONFIG+=release
VERBOSE=1 gnumake all -d 
Say "your music app is complete oh yea I know its sweet" --rate=69 --interactive=red
Open FFaudioConverter.app