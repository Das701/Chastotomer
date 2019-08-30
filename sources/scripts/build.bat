@echo off

cd keil
call build.bat
cd ..

cd vs_keil
call build.bat
cd ..

cd vs_win
call build.bat
cd ..
