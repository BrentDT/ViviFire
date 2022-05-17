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

#if !defined(_MODIFIER_H_)
#define _MODIFIER_H_

#include <assert.h>
#include "Parser.h"
#include "Scanner.h"

struct mod_type {
	bool present = false, allowed = false;
	int line = 0, col = 0;
	int order = 0;
};

class Modif {
public:
	enum mods {
		Abstract, Backed, Const, Deprecated, Flags,
		Iterator, MustUse, Open, Override, ReadOnly,
		Shared, SI, Unique, WriteOnly,
		LAST
	};

private:
	const wchar_t *m_name[LAST] = {
		L"@Abstract", L"@Backed", L"@Const", L"@Deprecated", L"@Flags",
		L"@Iterator", L"MustUse", L"@Open", L"@Override", L"@ReadOnly",
		L"@Shared", L"@SI", L"@Unique", L"@WriteOnly"
	};

	mod_type m_mods[LAST];
	int m_count;
	class Parser *m_parser;

public:
	Modif(Parser *p) : m_parser(p), m_count(0) {}

	void add(int m);
	bool allow(int m);
	int count() const { return m_count; }
	bool has(int m) const;
	void validate() const;

};

#endif
