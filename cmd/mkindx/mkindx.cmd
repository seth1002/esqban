@echo off
setlocal
set path=%1
rem if defined %path% chdir %path% else echo %path%
for /R %path% %%f in (*.cmd) do call D:\sqba\Projekti\cmd\mkindx\mkindx0.cmd %%f
echo 
endlocal