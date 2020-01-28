@echo off

@echo .
@echo %TIME%   Making Visual Studio Win Debug ...

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -G "Visual Studio 16 2019" -A Win32


@echo %TIME%   Complete
