/*
* ViviFire Programming Language
*
* Copyright 2021 Brent D. Thorn
*
* You can get the latest version at http://vivifire.com/.
*
* Use of this source code is governed by an MIT-style license that can be
* found in the LICENSE file.
*/

#if !defined(_ARGS_H_)
#define _ARGS_H_

struct Args {
	bool m;
	int v;
	char *file;
	
	// If you change the following, don't forget to change the text in Args.cpp.
	enum { default_v = 1 };
	
	Args(): m(false), v(default_v), file(NULL) {}
	
	bool Parse (int argc, char *argv[]);
};
#endif
