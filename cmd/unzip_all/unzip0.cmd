@echo off
chdir %~d1%~p1
f:\dosutils\pkunzip.exe -d -o %1
del %1
