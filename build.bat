@REM wsl cmake -B builds/build-mac -DCMAKE_TOOLCHAIN_FILE=../Linux\ Toolchains/toolchain-mac.cmake && cmake --build builds/build-mac --target Clipping_VST3

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake -B build -DCMAKE_PREFIX_PATH=C:\JUCE -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Installer_Win

pause