call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake -B build -DCMAKE_PREFIX_PATH=C:\JUCE -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Clipping_VST3
copy ".\build\Clipping_artefacts\Release\VST3\Clipping.vst3\Contents\x86_64-win\Clipping.vst3" "..\dev-install\Clipping.vst3"
