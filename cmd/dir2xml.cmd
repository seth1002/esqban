@echo off
setlocal
set path=%1
echo ^<links^> > %path%\..\dir.xml
rem for /R %path% %%f in (*.*) do echo ^<link^> %%~nf ^</link^> >> %path%\..\dir.xml
for /R %path% %%f in (*.*) do echo ^<link^> %%f ^</link^> >> %path%\..\dir.xml
echo ^</links^> >> %path%\..\dir.xml
endlocal