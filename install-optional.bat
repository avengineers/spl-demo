@echo off

pushd %~dp0
powershell -ExecutionPolicy Bypass -File build.ps1 -install -installOptional || exit /b 1
popd
pause
