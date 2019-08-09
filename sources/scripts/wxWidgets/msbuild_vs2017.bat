@echo off
MSBuild.exe ..\..\ThirdParty\wxWidgets\generated\wxWidgets.sln -t:rebuild
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success
echo .
echo .
echo .
echo                          Build success
echo .
echo .
echo .

:Exit

