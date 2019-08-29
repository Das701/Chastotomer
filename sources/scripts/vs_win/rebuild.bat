@echo off

@echo .
@echo %TIME%   Rebuild GUI version ...

MSBuild.exe ..\..\generated\Win32\Device\Device57.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo -t:rebuild
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Success
@echo %TIME%   Complete

:Exit

call copy_wxWidgets_dlls.bat
