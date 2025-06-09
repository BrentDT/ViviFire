@Echo Off
SetLocal EnableDelayedExpansion

:: Number of failed test.
Set fails=0

:: All stdout gets written to a new 'test.log' with each run.
> test.log (
  :: Recursively test each file with the extensions '.vfire' and '.vflib'.
  For /R %%f In (*.vfire *.vflib) Do Call :Test "%%~f"
)

:: Show success, or if failures, ask to show the log file.
If %fails% EQU 0 (
  Echo All tests passed.
  :: If run from Windows Explorer...
  If %~0 NEQ %0 Pause
) Else (
  :: Wait a maximum of 10 seconds.
  Choice /M "%fails% failed. Open test.log" /T:10 /D:N
  If !ErrorLevel! EQU 1 Start "" test.log
)

GoTo :EOF

:Test (%1 -> file)
  :: If test file does not have an associated '.err' file, test must be correct.
  If Not Exist "%~p1%~n1.err" GoTo TestCorrect

  Call :GetRunOutput %1
  :: Compare output to '.err' file.
  Echo %output%|FindStr /G:/ "%~p1%~n1.err" > NUL
  :: If they are different...
  If ErrorLevel 1 (
    Set "absolute=%~1"
    :: Show relative path and output.
    Echo "!absolute:%CD%\=!" failed with {{%output%}}.
    :: Show expected output on one line.
    Set /P=Expected output is {{< NUL & Type "%~p1%~n1.err" & Echo }}.
    Set /A fails+=1
  )
  GoTo :EOF

:TestCorrect
  Call :GetRunOutput %1
  If "%output%" NEQ "-- 0 errors" (
    Set "absolute=%~1"
    :: Show relative path and output.
    Echo "!absolute:%CD%\=!" failed with {{%output%}}.
    Set /A fails+=1
  )
  GoTo :EOF

:GetRunOutput (%1 -> file) -> %output%
  :: Run 'vf.exe' and record stdout to %output%.
  :: Workaround to convert UTF-16 to ASCII/ANSI also removes control chars.
  Set "output="
  For /F "usebackq tokens=* delims=" %%a In (`
    ..\src\release\vf.exe -v0 %1 ^| Find /V ""
  `) Do Set "output=!output!%%a"
  GoTo :EOF
