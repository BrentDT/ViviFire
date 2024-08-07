/*
 * ViviFire Programming Language
 *
 * Copyright 2024 Brent D. Thorn
 *
 * You can get the latest version at http://vivifire.com/.
 *
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file.
 */

#define NOMINMAX
#define UNICODE
#define _UNICODE

#include <chrono>
#include <cstdio>
#include <io.h>
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
// Checks if the console cursor is at 0,0.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (!GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// This shouldn't happen!
		return false;
	}
	
	return ((!csbi.dwCursorPosition.X) && (!csbi.dwCursorPosition.Y));
#else
	return !isatty(fileno(stdin));
#endif
}

#if defined(_WIN32)
wchar_t *FormatBytes (double size, wchar_t *buf) {
// Returns a string containing a human-readable size for data given in bytes.
// The 'buf' should point to a buffer at least 16 bytes in size.
	 const wchar_t *units[] = { L"Bytes", L"KiB", L"MiB", L"GiB", L"TiB" };
	int i = 0;
	while (size > 1024.0) {
		size /= 1024.0;
		i++;
	}
	swprintf(buf, L"%.2f %ls", size, units[i]);
	return buf;
}

void PrintMemoryInfo ( DWORD processID ) {
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	
	// Print the process identifier.
	wprintf( L"\nProcess ID: %u\n", processID );
	
	// Print information about the memory usage of the process.
	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
	PROCESS_VM_READ, FALSE, processID );
	if (NULL == hProcess) {
		return;
	}
	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) ) {
		wchar_t buf[16];
		wprintf( L"\tPageFaultCount: %u\n", pmc.PageFaultCount );
		wprintf( L"\tPeakWorkingSetSize: %ls\n", FormatBytes(pmc.PeakWorkingSetSize, buf));
		wprintf( L"\tWorkingSetSize: %ls\n", FormatBytes(pmc.WorkingSetSize , buf));
		wprintf( L"\tQuotaPeakPagedPoolUsage: %ls\n", FormatBytes(pmc.QuotaPeakPagedPoolUsage, buf));
		wprintf( L"\tQuotaPagedPoolUsage: %ls\n", FormatBytes(pmc.QuotaPagedPoolUsage, buf));
		wprintf( L"\tQuotaPeakNonPagedPoolUsage: %ls\n", FormatBytes(pmc.QuotaPeakNonPagedPoolUsage, buf));
		wprintf( L"\tQuotaNonPagedPoolUsage: %ls\n", FormatBytes(pmc.QuotaNonPagedPoolUsage, buf));
		wprintf( L"\tPagefileUsage: %ls\n", FormatBytes(pmc.PagefileUsage, buf)); 
		wprintf( L"\tPeakPagefileUsage: %ls\n", FormatBytes(pmc.PeakPagefileUsage, buf));
	}
	CloseHandle( hProcess );
}
#endif

int main(int argc, char *argv[]) {
	int result = 0;

	// Make sure we are set to use Unicode.
	// Start code adapted from <https://stackoverflow.com/questions/57955871>.
	fflush(stdout);
#if defined _MSC_VER
#   pragma region WIN_UNICODE_SUPPORT_MAIN
#endif
#if defined _WIN32
	// change code page to UTF-8 UNICODE
	if ( !IsValidCodePage(CP_UTF8) || !SetConsoleCP(CP_UTF8) || !SetConsoleOutputCP(CP_UTF8) ) {
		return GetLastError();
	}
	
	// change console font - post Windows Vista only
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfie;
	const auto sz = sizeof(CONSOLE_FONT_INFOEX);
	ZeroMemory(&cfie, sz);
	cfie.cbSize = sz;
	cfie.dwFontSize.Y = 14;
	wcscpy_s(cfie.FaceName,
		L"Lucida Console");
	SetCurrentConsoleFontEx(hStdOut,
		false,
		&cfie);
		
	// change file stream translation mode
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
#endif
#if defined _MSC_VER
#   pragma endregion
#endif
	///std::ios_base::sync_with_stdio(false);
	// End adapted code.
 
	bool separate = separate_console();
	
	if (!args.Parse(argc, argv)) {
		if (separate) {
			wprintf(L"\nPress Enter to close... ");
			wscanf(L"%*0c"); // Get char.
		}
		return 1;
	}
	
	// Record start time.
	const auto start = std::chrono::steady_clock::now();
	
	wchar_t *fileName = coco_string_create(args.file);
	Parser *p = new Parser(new Scanner(fileName));
	
	p->Parse();

	if (p->errors->count) result = 2;
	
	if (args.t) {
		// Calculate the duration.
		const std::chrono::duration<double, std::milli> timer = std::chrono::steady_clock::now() - start;
		wprintf(L"-- time: %.1f ms\n", timer.count());
	}
	
	coco_string_delete(fileName);
	delete p	->scanner;
	delete p->module;
	delete p;

#if defined(_WIN32)
	if (args.m) {
		PrintMemoryInfo(GetCurrentProcessId());
	}
#endif
	return result;
}

