setlocal

set start = %1
set end = %2
rem for /L %%number in (%start%, 1, %end%) do call template.bat %number%

rem for /L %%number in (0, 1, 99) do call template.bat %number%

for /L %%number in (0, 1, 99) do echo %%number


