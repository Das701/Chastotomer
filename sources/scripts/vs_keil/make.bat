@echo off

@echo .
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil -DCMAKE_GENERATOR="Visual Studio 15 2017"
@echo off

@echo %TIME%   Complete
