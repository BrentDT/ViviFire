:: Generates 'ViviFire.rc', a Windows resource script.
:: The version number is related to the date.
@Echo Off
SetLocal

:: Get date/time into _LocalDateTime.
For /F "skip=2 delims=" %%a In ('
	WMIC OS Get LocalDateTime /Value
') Do Set _%%~na 2> NUL

:: Sanity check.
If "%_LocalDateTime%" LSS "20200101000000" (
	Echo Error: Could not get local date.
	Pause
	Goto :EOF
)

:: Break date into components.
Set year=%_LocalDateTime:~0,4%
Set month=%_LocalDateTime:~4,2%
Set day=%_LocalDateTime:~6,2%

:: Trim leading zero.
If %month:~0,1% EQU 0 Set month=%month:~1,1%
If %day:~0,1% EQU 0 Set day=%day:~1,1%

:: Write the file.
> ViviFire.rc (
	Echo /*
	Echo * ViviFire Programming Language
	Echo *
	Echo * Copyright 2012-%year% Brent D. Thorn
	Echo *
	Echo *You can get the latest version at http://vivifire.com/.
	Echo *
	Echo * Use of this source code is governed by an MIT-style license that can be
	Echo * found in the LICENSE file.
	Echo */
	Echo.
	Echo #include ^<winver.h^>
	Echo.
	Echo 1 VERSIONINFO
	Echo FILEVERSION 0,%year%,%month%,%day%
	Echo PRODUCTVERSION 0,%year%,%month%,%day%
	Echo FILEFLAGSMASK VS_FFI_FILEFLAGSMASK
	Echo #ifdef _DEBUG
	Echo FILEFLAGS VS_FF_PRERELEASE^|VS_FF_DEBUG
	Echo #else
	Echo FILEFLAGS VS_FF_PRERELEASE
	Echo #endif
	Echo FILEOS VOS_NT_WINDOWS32
	Echo FILETYPE VFT_APP
	Echo BEGIN
	Echo     BLOCK "StringFileInfo"
	Echo     BEGIN
	Echo         BLOCK "040904b0"
	Echo         BEGIN
	Echo             VALUE "Comments", "By Brent D Thorn\0"
	Echo             VALUE "CompanyName", "vivifire.com\0"
	Echo             VALUE "FileDescription", "ViviFire programming language alpha test\0"
	Echo             VALUE "FileVersion", "0.%year%.%month%.%day%\0"
	Echo             VALUE "InternalName", "VF\0"
	Echo             VALUE "LegalCopyright", "Copyright ©2012-%year%\0"
	Echo             VALUE "OriginalFilename", "VF.exe\0"
	Echo             VALUE "ProductName", "ViviFire\0"
	Echo             VALUE "ProductVersion", "0.%year%.%month%.%day%\0"
	Echo             VALUE "SpecialBuild", "\0"
	Echo         END
	Echo     END
	Echo     BLOCK "VarFileInfo"
	Echo     BEGIN
	Echo         VALUE "Translation", 0x409, 800
	Echo     END
	Echo END
)
