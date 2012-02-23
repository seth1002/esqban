@echo off
setlocal
set path=%1
rem if defined %path% chdir %path% else echo %path%
for /R %path% %%f in (*.zip) do call f:\unzip0.cmd %%f
rem f:\dosutils\pkunzip.exe -d %%f
endlocal