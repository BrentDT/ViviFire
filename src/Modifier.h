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

#if !defined(_MODIFIER_H_)
#define _MODIFIER_H_

#include <assert.h>

class Parser;
class Scanner;

struct mod_type {
	bool present = false, allowed = false;
	int line = 0, col = 0;
	int order = 0;
};

#define NAME_STRING_PAIRS \
	X(Abstract, L"@ABSTRACT") \
	X(Backed, L"@BACKED") \
	X(Const, L"@CONST") \
	X(Deprecated, L"@DEPRECATED") \
	X(Flags, L"@FLAGS") \
	X(Iterator, L"@ITERATOR") \
	X(MustUse, L"@MUSTUSE") \
	X(Open, L"@OPEN") \
	X(Override, L"@OVERRIDE") \
	X(ReadOnly, L"@READONLY") \
	X(Shared, L"@SHARED") \
	X(SI, L"@SI") \
	X(Strict, L"@STRICT") \
	X(Test, L"@TEST") \
	X(Unique, L"@UNIQUE") \
	X(WriteOnly, L"@WRITEONLY")

class Modif {
public:
	#define X(name, str) name,
	enum mods {
		NAME_STRING_PAIRS
		LAST
	};
	#undef X
	
	enum si_args { SI_default, SI_Binary, SI_Large, SI_Small };
	enum test_args { Test_default, Test_Ignore };
	
	typedef union arg {
		si_args si_arg;
		test_args test_arg;
	} arg;

private:
	#define X(name, str) str,
	const wchar_t *m_name[LAST] = {
		NAME_STRING_PAIRS
	};
	#undef X

	mod_type m_mods[LAST];
	int m_count = 0;
	class Parser *m_parser;

public:
	Modif(Parser *p) : m_parser(p) {
		assert(m_name[LAST - 1] != nullptr);
	}

	void add(int m);
	bool allow(int m);
	int count() const { return m_count; }
	bool has(int m) const;
	void validate() const;

};

typedef Modif::arg ModifArg;

#endif
