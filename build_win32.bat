SET PATH=C:\Qt\bin;%MSYS_PATH%;%MINGW64_PATH%;%PATH%

mkdir build
cd build

qmake.exe ..\FFaudioConverter.pro -spec win32-g++ CONFIG+=release
make.exe -j4

cd release

for /f "delims=" %%F in ('dir /b /a-d ^| findstr /vile ".exe"') do del "%%F"

windeployqt.exe --release --compiler-runtime .

rmdir /s /q iconengines imageformats
del D3Dcompiler_47.dll Qt5Svg.dll opengl32sw.dll libGLESV2.dll libEGL.dll

echo .
echo Build done. Now download FFmpeg and place it in the applications directory.
echo https://github.com/BtbN/FFmpeg-Builds/releases
