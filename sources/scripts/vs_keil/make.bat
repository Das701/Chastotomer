@echo off

@echo .
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil -DCMAKE_GENERATOR="Visual Studio 15 2017" -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -A win32
@echo off

@echo %TIME%   Complete
