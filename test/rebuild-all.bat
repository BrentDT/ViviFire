@Echo Off
SetLocal EnableDelayedExpansion
For /R %%f In (*.in.txt) Do   Call :Run "%%f"
GoTo :EOF
:Run
Set f=%~1
..\VF -v0 %1 > "%f:~0,-6%v0.txt"
GoTo :EOF
