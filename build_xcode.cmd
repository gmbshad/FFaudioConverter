qmake ../FFaudioConverter.pro -spec macx-xcode -Wnone CONFIG+=sdk_no_version_check  CONFIG+=release
Xcodebuild -verbose -configuration=Release -allowProvisioningUpdates
Sudo xattr -cr Release/FFaudioConverter.app 
Sudo xattr -r -d com.apple.quarantine Release/FFaudioConverter.app
Sudo codesign --force --deep --sign - Release/FFaudioConverter.app
Say -v organ "your music app is complete oh yea I know its sweet" --rate=69 --interactive=red
Open Release/FFaudioConverter.app