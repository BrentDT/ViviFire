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

#if !defined(_ARGS_H_)
#define _ARGS_H_

struct Args {
	// If you change the following, don't forget to change the text in Args.cpp.
	enum {
		default_v = 1,
		STMT = 2,
		EXPR = 3
	};

	bool  m    = false;
	bool  t    = false;
	short v    = default_v;
	char *file = nullptr;
	
	bool Parse (int argc, char *argv[]);
};

#endif
