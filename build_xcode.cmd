mkdir -p build; cd build
qmake ../FFaudioConverter.pro -spec macx-xcode -Wnone CONFIG+=sdk_no_version_check  CONFIG+=release
Xcodebuild -verbose -configuration=Release -allowProvisioningUpdates
Say "your music app is complete oh yea I know its sweet" --rate=69 --interactive=red
Open Release/FFaudioConverter.app