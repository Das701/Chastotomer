@echo off

@echo .
@echo %TIME%   Making Visual Studio Win Debug ...

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -DCMAKE_GENERATOR="Visual Studio 15 2017"


@echo %TIME%   Complete
