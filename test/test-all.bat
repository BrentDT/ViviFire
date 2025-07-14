@Echo Off
SetLocal EnableDelayedExpansion

Set "VF_EXE=..\src\release\vf.exe"
Set "FLAGS=-v0"

If Not Exist %VF_EXE% (
	Echo FATAL ERROR: ViviFire is not at %VF_EXE%
	Exit /B 1
)

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
	Call :GetRunOutput %1
	:: If test file does not have an associated '.err' file, test must have no errors.
	If Not Exist "%~p1%~n1.err" (
		If "%output%" NEQ "-- 0 errors" (
			Set "absolute=%~1"
			:: Show relative path and output.
			Echo "!absolute:%CD%\=!" failed with {{%output%}}.
			Set /A fails+=1
		)
	) Else (
		:: Compare output to '.err' file.
		FindStr /B /C:"%output%" "%~p1%~n1.err" > NUL
		:: If they are different...
		If ErrorLevel 1 (
			Set "absolute=%~1"
			:: Show relative path and output.
			Echo "!absolute:%CD%\=!" failed with {{%output%}}.
			:: Show expected output on one line.
			Set /P=Expected output is {{< NUL & Type "%~p1%~n1.err" & Echo }}.
			Set /A fails+=1
		)
	)
	GoTo :EOF

:GetRunOutput (%1 -> file) -> %output%
	:: Run 'vf.exe' and record stdout to %output%.
	:: Workaround to convert UTF-16 to ASCII/ANSI also removes control chars.
	Set "output="
	For /F "usebackq tokens=* delims=" %%a In (`
		%VF_EXE% %FLAGS% %1 ^| Find /V ""
	`) Do Set "output=!output!%%a"
	:: Escape chars that can cause problems: " -> \x22, ) -> \x29.
	Set output=%output:"=\x22%
	Set output=%output:)=\x29%
	GoTo :EOF
