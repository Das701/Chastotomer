@echo off

@echo .

@echo %TIME%   Building VS-Keil version

MSBuild.exe ..\..\generated\Keil\CH3-96-2.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

goto Success

:Failed
@echo %TIME%   !!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!! Build Failed !!!!!!
goto Exit

:Success
@echo %TIME%   Complete

:Exit

