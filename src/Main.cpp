/*----------------------------------------------------------------------
ViviFire Programming Language
Authors:
2012-2016 Brent D. Thorn <brent@b6sw.com>

You can get the latest version at the Bay Six Software website at
http://www.b6sw.com/

To the extent possible under law, the author(s) have dedicated all 
copyright and related and neighboring rights to this software to the 
public domain worldwide. This software is distributed without any warranty. 
You should have received a copy of the CC0 Public Domain Dedication along 
with this software. 
If not, please consult the website at
http://creativecommons.org/publicdomain/zero/1.0/
----------------------------------------------------------------------*/

#define NOMINMAX
#define UNICODE
#define _UNICODE
#define __UNICODE
#include <stdio.h>
#include <windows.h>
#include <psapi.h>  // For MinGW, also give the linker "-lpsapi".
#include "Scanner.h"
#include "Parser.h"
#include "Args.h"

Args args;

bool separate_console () {
// Returns true if app was started from the Windows shell.
// Checks if the console cursor is at 0,0.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (!GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// This shouldn't happen!
		return false;
	}
	
	return ((!csbi.dwCursorPosition.X) && (!csbi.dwCursorPosition.Y));
}

char *FormatBytes (double size, char *buf) {
// Returns a string containing a human-readable size for data given in bytes.
// The 'buf' should point to a buffer at least 16 bytes in size.
	 const char *units[] = { "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	int i = 0;
	while (size > 1024.0) {
		size /= 1024.0;
		i++;
	}
	sprintf(buf, "%.2f %s", size, units[i]);
	return buf;
}

void PrintMemoryInfo ( DWORD processID ) {
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	
	// Print the process identifier.
	printf( "\nProcess ID: %u\n", processID );
	
	// Print information about the memory usage of the process.
	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
	PROCESS_VM_READ, FALSE, processID );
	if (NULL == hProcess) {
		return;
	}
	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) ) {
		char buf[16];
		printf( "\tPageFaultCount: %u\n", pmc.PageFaultCount );
		printf( "\tPeakWorkingSetSize: %s\n", FormatBytes(pmc.PeakWorkingSetSize, buf));
		printf( "\tWorkingSetSize: %s\n", FormatBytes(pmc.WorkingSetSize , buf));
		printf( "\tQuotaPeakPagedPoolUsage: %s\n", FormatBytes(pmc.QuotaPeakPagedPoolUsage, buf));
		printf( "\tQuotaPagedPoolUsage: %s\n", FormatBytes(pmc.QuotaPagedPoolUsage, buf));
		printf( "\tQuotaPeakNonPagedPoolUsage: %s\n", FormatBytes(pmc.QuotaPeakNonPagedPoolUsage, buf));
		printf( "\tQuotaNonPagedPoolUsage: %s\n", FormatBytes(pmc.QuotaNonPagedPoolUsage, buf));
		printf( "\tPagefileUsage: %s\n", FormatBytes(pmc.PagefileUsage, buf)); 
		printf( "\tPeakPagefileUsage: %s\n", FormatBytes(pmc.PeakPagefileUsage, buf));
	}
	CloseHandle( hProcess );
}

int main (int argc, char *argv[]) {
	bool separate = separate_console();
	
	if (!args.Parse(argc, argv)) {
		if (separate) {
			fputs("\nPress any key to close. ", stdout);
			getchar();
		}
		return 1;
	}
	
	wchar_t *fileName = coco_string_create(args.file);
	Scanner *s = new Scanner(fileName);
	Parser *p = new Parser(s);
	
	p->Parse();
	
	coco_string_delete(fileName);
	delete p;
	delete s;
	
	if (args.m) {
		PrintMemoryInfo(GetCurrentProcessId());
	}
	
	return 0;
}

