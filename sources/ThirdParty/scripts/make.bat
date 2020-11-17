rmdir ..\wxWidgets\generated /s /q
cmake.exe ..\wxWidgets\CMakeLists.txt -B..\wxWidgets\generated -G "Visual Studio 16 2019" -A Win32 -DwxBUILD_SHARED=ON -DwxBUILD_USE_STATIC_RUNTIME=ON
