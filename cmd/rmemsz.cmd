@echo off
setlocal
set memkey="HKEY_LOCAL_MACHINE\hardware\resourcemap\system resources\physical memory"
set mc=-m
set pc=%1
if not defined pc set mc=
set memsz=
for /f "eol=# tokens=5" %%a in ('regdmp %mc% %pc% %memkey% ^| find "LENGTH"') do (set /a memsz = memsz + %%a / 1024)
if errorlevel 1 goto :eof
if not defined mc set pc=local machine
echo %pc% reports %memsz% KB RAM
endlocal