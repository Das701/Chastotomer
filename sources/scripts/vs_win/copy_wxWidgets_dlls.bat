@echo %TIME%   Copying files *.dll wxWidgtes

xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\*.dll ..\..\generated\Win32\Panel\Debug /Y /Q
xcopy ..\..\ThirdParty\wxWidgets\generated\lib\vc_dll\*.dll ..\..\generated\Win32\Saver\Debug /Y /Q

