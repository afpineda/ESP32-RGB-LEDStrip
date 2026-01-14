@echo off
cls
powershell ./Clean.ps1
powershell ./Build.ps1 -TestName %1
if ERRORLEVEL 1 goto end
powershell ./Run.ps1 -TestName %1

:end
powershell ./Clean.ps1
