@echo off

@echo .
@echo %TIME%   Making Visual Studio Win Debug ...

cd ../..

rmdir "generated/Win32" /s /q

cd scripts/vs_win

cmake ../../VS/CMakeLists.txt -B../../generated/Win32 -G "Visual Studio 16 2019" -A Win32 -DBUILD_USE_STATIC_RUNTIME=ON


@echo %TIME%   Complete
