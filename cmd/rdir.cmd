@echo off
setlocal
set path=%1
set ext=%2
for /R %path% %%f in (%ext%) do echo %%f
endlocal