/*
* ViviFire Programming Language
*
* Copyright 2012-2020 Brent D. Thorn
*
* You can get the latest version at http://vivifire.com/.
*
* Use of this source code is governed by an MIT-style license that can be
* found in the LICENSE file.
*/

#include "AST.h"

void removeChar(wchar_t *str, wchar_t garbage) {
	wchar_t *src, *dst;
	for (src = dst = str; *src != L'\0'; src++) {
		*dst = *src;
		if (*dst != garbage) dst++;
	}
	*dst = L'\0';
}

namespace AST {

// Pure virtual destructor requires implementation.
Node::~Node() {}
Expression::~Expression() {}
Statement::~Statement() {}

IntConst::IntConst(wchar_t *s) {
	int base = 10;
	if (s[0] == L'&') {
		switch (s[1]) {
		case L'B': case L'b': base = 2; break;
		case L'O': case L'o': base = 8; break;
		case L'H': case L'h': base = 16; break;
		}
		s += 2;
	}
	wchar_t *pc = wcschr(s, L'\0') - 1;
	val = 0;
	for (int64_t place = 1; pc >= s; place *= base, pc--) {
		if (*pc == L'_') continue;
		else if (*pc >= L'a') val += (*pc - L'a' + 10) * place;
		else if (*pc >= L'A') val += (*pc - L'A' + 10) * place;
		else val += (*pc - L'0') * place;
	}
}

RealConst::RealConst(wchar_t *s) {
	// Strip out underscores
	removeChar(s, L'_');
	// Convert string to double
	wchar_t *stopper;
	val = wcstod(s, &stopper);
}

CharConst::CharConst(wchar_t *s) {

}

DTConst::DTConst(wchar_t *s) {
	const uint64_t ticks_per_sec = UINT64_C(10000000);
	wchar_t *sep = wcschr(s, L':');
	if (sep) {  // time constant
		val = 3600 * ticks_per_sec * wcstol(s + 1, &sep, 10);  // hours
		val += 60 * ticks_per_sec * wcstol(sep + 1, &sep, 10);  // minutes
		val += ticks_per_sec * wcstod(sep + 1, &sep);  // seconds
	} else {  // date constant
//		val = something * ticks_per_sec * wcstol(s + 1, &sep);  // year
//		val += something * ticks_per_sec * wcstol(sep + 1, &sep);  // month
//		val += something * ticks_per_sec * wcstol(sep + 1, &sep);  // day
	}
}

void Enum::Add(wchar_t *id, int ln, int c) {
	int64_t val;
	if (!flags) {
		if (value == INT64_MAX) { /* TODO: Error: overflow */ }
		val = value++;
	} else  {
		val = 1;
		while (val && value == value | val) val <<= 1;
		if (val == 0) { /* TODO: Error: no unused bits */ }
		value |= val;
	}
	consts.push_back(new constant(id, val, ln, c);
}

void ModuleWhere::normalize() {
	// Removes unnecessary 0s from 'value'.
	int size = coco_string_length(value);
	wchar_t *norm = coco_string_create(value);
	wchar_t *src = value, *dest = norm;
	while (*src) {
		while (*src == L'0') src++; // Move past zeros.
		if (*src == L'\0' || *src == L'.') {
			*dest++ = L'0'; // Group is a single zero.
		} else {
			while (*src && *src != L'.') *dest++ = *src++; // Copy group.
		}
		if (*src == L'.') *dest++ = *src++; // Move to next group.
	}
	*dest = L'\0';
	wcscpy_s(value, size, norm);
	coco_string_delete(norm);
}

#define GENERATE_ACCEPT(T)   \
	void T::Accept(Visitor *v) { \
		v->Visit(this);             \
	}

GENERATE_ACCEPT(Assignment)
GENERATE_ACCEPT(BinaryOp)
GENERATE_ACCEPT(Block)
GENERATE_ACCEPT(Call)
GENERATE_ACCEPT(Case)
GENERATE_ACCEPT(CaseExpression)
GENERATE_ACCEPT(CaseIs)
GENERATE_ACCEPT(CaseTo)
GENERATE_ACCEPT(Catch)
GENERATE_ACCEPT(ComparisonChain)
GENERATE_ACCEPT(ComparisonOp)
GENERATE_ACCEPT(CharConst)
GENERATE_ACCEPT(Do)
GENERATE_ACCEPT(DTConst)
GENERATE_ACCEPT(ElseIf)
GENERATE_ACCEPT(For)
GENERATE_ACCEPT(ForEach)
GENERATE_ACCEPT(RealConst)
GENERATE_ACCEPT(GotoCase)
GENERATE_ACCEPT(Exit)
GENERATE_ACCEPT(If)
GENERATE_ACCEPT(BooleanCond)
GENERATE_ACCEPT(IntConst)
GENERATE_ACCEPT(Library)
GENERATE_ACCEPT(Loop)
GENERATE_ACCEPT(Program)
GENERATE_ACCEPT(Return)
GENERATE_ACCEPT(Select)
GENERATE_ACCEPT(Symbol)
GENERATE_ACCEPT(Throw)
GENERATE_ACCEPT(Try)
GENERATE_ACCEPT(UnaryOp)
GENERATE_ACCEPT(Wait)
GENERATE_ACCEPT(While)

} // namespace AST
