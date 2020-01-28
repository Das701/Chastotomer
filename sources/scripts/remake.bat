@echo off

cd ..
rmdir generated /s /q
cd scripts

cd vs_keil
call make.bat
cd..

cd vs_win
call make.bat
cd ..
