@Echo Off
SetLocal

Set fails=0
If Exist test.log Del test.log

For /R %%f In (*.in.txt) Do Call :Test "%%f"

Echo %fails% failed. >> test.log
GoTo :EOF

:Test
Set f=%~1
For /F "usebackq tokens=* delims=" %%a In (`..\VF -v0 %1`) Do (
  Echo %%a|FindStr /G:/ "%f:~0,-6%v0.txt" > NUL
  If ErrorLevel 1 (
    Echo %f% failed with "%%a". >> test.log
    Set /A fails+=1
    GoTo :EOF
  )
)
GoTo :EOF
