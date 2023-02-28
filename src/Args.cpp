/*
 * ViviFire Programming Language
 *
 * Copyright 2023 Brent D. Thorn
 *
 * You can get the latest version at http://vivifire.com/.
 *
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file.
 */

#include <ctype.h>
#include <stdio.h>
#include "Args.h"
#include "Now.inc"

extern Args args;

bool Args::Parse (int argc, char *argv[]) {
	// Returns true if the command line args are valid.
	// Otherwise, prints an error msg and returns false.
	bool show_help = false;
	if (argc >= 2) {
		for (int i = 1; i < argc; i++) {
			char *arg = argv[i];
			// Look for args starting with dash or slash.
			if (arg[0] == '-' || arg[0] == '/') {
				switch (tolower(arg[1])) {
				case '?':
					show_help = true;
					break;
				case 'm':
					if (arg[2] == '\0') this->m = true;
					else goto bad_flag;
					break;
				case 't':
					if (arg[2] == '\0') this->t = true;
					else goto bad_flag;
					break;
				case 'v':
					switch (arg[2]) {
					case '0': case '1': case '2': case '3':
						if (arg[3] == '\0') this->v = arg[2] - '0';
						else goto bad_flag;
						break;
					case '\0':
						v = default_v;
						break;
					default:
						goto bad_flag;
					}
					break;
				default:
				bad_flag:
					fprintf(stderr, "Invalid switch '%s'.\nUse '-?' for help.", arg);
					return false;
				} // switch
			} else if (file == NULL) {
				// Doesn't look like a switch, assume it's a filename.
				this->file = arg;
			} else {
				fprintf(stderr, "Cannot specify more than one file.");
				return false;
			} // if
		} // for
	}
	// If not enough args or show_help is true, display syntax.
	if (argc < 2 || show_help) {
		fprintf((show_help) ? stdout : stderr,
		"ViviFire Test Parser by Brent D. Thorn, " NOW "\n"
		"Syntax:\n"
		"\tvf [-m] [-t] [-v[0-3]] file\n"
		"Switches:\n"
		"\t-m   Display memory usage\n"
		"\t-t   Display time elapsed\n"
		"\t-v0  Minimal verbosity; Errors only\n"
		"\t-v1  Displays tokens and errors (default)\n"
		"\t-v2  Displays statements, tokens, and errors\n"
		"\t-v3  Maximum verbosity"
		);
		return false;
	}
	// Just avout done, but make sure we've got a file.
	if (file == NULL) {
		fprintf(stderr, "No file specified.");
		return false;
	}
	return true;
}
