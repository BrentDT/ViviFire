/*
 * ViviFire Programming Language
 *
 * Copyright 2022 Brent D. Thorn
 *
 * You can get the latest version at http://vivifire.com/.
 *
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file.
 */

#define NOMINMAX
#define UNICODE
#define _UNICODE
#define __UNICODE

#include <cstdio>
#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>  // For MinGW, also give the linker "-lpsapi".
#else
#include <unistd.h> // For 'isatty'.
#endif
#include "Scanner.h"
#include "Parser.h"
#include "Args.h"

Args args;

bool separate_console () {
// Returns true if app was started from the desktop.
#if defined(_WIN32)
	return !_isatty(fileno(stdin));
#else
	return !isatty(fileno(stdin));
#endif

#if 0
// Checks if the console cursor is at 0,0.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (!GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// This shouldn't happen!
		return false;
	}
	
	return ((!csbi.dwCursorPosition.X) && (!csbi.dwCursorPosition.Y));
#endif
}

#if defined(_WIN32)
char *FormatBytes (double size, char *buf) {
// Returns a string containing a human-readable size for data given in bytes.
// The 'buf' should point to a buffer at least 16 bytes in size.
	 const char *units[] = { "Bytes", "KiB", "MiB", "GiB", "TiB" };
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
#endif

int main (int argc, char *argv[]) {
	bool separate = separate_console();
	
	if (!args.Parse(argc, argv)) {
		if (separate) {
			printf("\nPress Enter to close... ");
			getchar();
		}
		return 1;
	}
	
	wchar_t *fileName = coco_string_create(args.file);
	Parser *p = new Parser(new Scanner(fileName));
	
	p->Parse();
	
	coco_string_delete(fileName);
	delete p->scanner;
	delete p;

#if defined(_WIN32)
	if (args.m) {
		PrintMemoryInfo(GetCurrentProcessId());
	}
#endif
	return 0;
}

