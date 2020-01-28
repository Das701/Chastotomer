@echo off

@echo .
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -G "Visual Studio 16 2019" -A Win32
@echo off

@echo %TIME%   Complete
