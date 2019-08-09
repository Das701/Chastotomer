rmdir ../ThirdParty/ /s /q

@echo Installation SDL2

cd SDL2
start install_and_build.bat
cd ..

@echo Installation wxWidgets

cd wxWidgets
start install_and_build.bat
cd ..
