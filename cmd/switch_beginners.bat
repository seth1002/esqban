@ECHO OFF
D:
CD D:\sqba\Projekti\Swish\www\OPT
IF EXIST Beginners_new.htm (
  REN Beginners.htm Beginners_old.htm
  REN Beginners_new.htm Beginners.htm
  ECHO Using new Beginners.htm
) else (
  REN Beginners.htm Beginners_new.htm
  REN Beginners_old.htm Beginners.htm
  ECHO Using old Beginners.htm
)