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

#if !defined(_MODIFIER_H_)
#define _MODIFIER_H_

///#include <cstdio>
#include <assert.h>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include "Parser.h"
#include "Scanner.h"

class Parser;

struct Modif abstract {
	bool can_do = false;
	const int line, col;
	Modif(int line, int col): line(line), col(col) {}
	virtual ~Modif() {}
	virtual const wchar_t *Text() const = 0;
	Modif() = delete;
};

#if defined(RAW_PTR)
typedef struct Modif* ModifPtr;
#define NEW(type) new type
#else
typedef std::shared_ptr<struct Modif> ModifPtr;
#define NEW(type) std::make_shared<type>
#endif

#define ARGUMENTLESS_MODIF(prefix, text) \
	struct prefix##Modif : public Modif { \
		prefix##Modif(int line, int col): Modif(line, col) {} \
		const wchar_t *Text() const { return text; } \
	}; \
static const std::type_index s_##prefix = typeid(prefix##Modif);

ARGUMENTLESS_MODIF(Abstract, L"@ABSTRACT")
ARGUMENTLESS_MODIF(Backed, L"@BACKED")
ARGUMENTLESS_MODIF(Const, L"@CONST")
ARGUMENTLESS_MODIF(Flags, L"@FLAGS")
ARGUMENTLESS_MODIF(Iterator, L"@ITERATOR")
ARGUMENTLESS_MODIF(MustUse, L"@MUSTUSE")
ARGUMENTLESS_MODIF(Open, L"@OPEN")
ARGUMENTLESS_MODIF(Override, L"@OVERRIDE")
ARGUMENTLESS_MODIF(ReadOnly, L"@READONLY")
ARGUMENTLESS_MODIF(Shared, L"@SHARED")
ARGUMENTLESS_MODIF(Strict, L"@STRICT")
ARGUMENTLESS_MODIF(Unique, L"@UNIQUE")
ARGUMENTLESS_MODIF(WriteOnly, L"@WRITEONLY")

#undef ARGUMENTLESS_MODIF

struct DeprecatedModif : public Modif {
	std::wstring msg;
	DeprecatedModif(int line, int col, wchar_t *msg): Modif(line, col) {
		if (msg) this->msg = msg;
	}
	const wchar_t *Text() const { return L"@DEPRECATED"; }
};
static const std::type_index s_Deprecated = typeid(DeprecatedModif);

struct SIModif : public Modif {
	enum class SIArg { _NA, BINARY, LARGE, SMALL };
	SIArg arg = SIArg::_NA;
	SIModif(int line, int col, SIArg arg): Modif(line, col), arg(arg) {}
	const wchar_t *Text() const { return L"@SI"; }
};
static const std::type_index s_SI = typeid(SIModif);

struct TestModif : public Modif {
	enum class TestArg { _NA, IGNORED };
	TestArg arg = TestArg::_NA;
	TestModif(int line, int col, TestArg arg): Modif(line, col), arg(arg) {}
	const wchar_t *Text() const { return L"@TEST"; }
};
static const std::type_index s_Test = typeid(TestModif);

struct Modifiers {
	std::vector<std::pair<std::type_index, ModifPtr>> mods;
	const Parser *parser;
	Modifiers(Parser *p): parser(p) {}
	bool Add(ModifPtr pMod);
	bool Let(const std::type_index &tid);
	ModifPtr find(const std::type_index &tid) const;
	bool Has(const std::type_index &tid) const;
	void Check() const;
	size_t Count() const { return mods.size(); }
	Modifiers() = delete;
};

#endif
