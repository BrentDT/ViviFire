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

#include <cstdint>
#include <inttypes.h>
#include "AST.h"

namespace AST {

void removeChar(wchar_t *str, wchar_t garbage) {
	wchar_t *src, *dst;
	for (src = dst = str; *src != L'\0'; src++) {
		*dst = *src;
		if (*dst != garbage) dst++;
	}
	*dst = L'\0';
}

// Pure virtual destructor requires implementation.
Expr::~Expr() {}
Node::~Node() {}
Statement::~Statement() {}

IntConst::IntConst(wchar_t *s, int line, int col): Constant(line , col) {
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
	val = 0LL;
	for (long long place = 1; pc >= s; place *= base, pc--) {
		if (*pc == L'_') continue;
		else if (*pc >= L'a') val += (*pc - L'a' + 10) * place;
		else if (*pc >= L'A') val += (*pc - L'A' + 10) * place;
		else val += (*pc - L'0') * place;
	}
}

RealConst::RealConst(wchar_t *s, int line, int col): Constant<long double>(line, col) {
	// Strip out underscores
	removeChar(s, L'_');
	// Convert string to double
	wchar_t *stopper;
	val = wcstod(s, &stopper);
}

CharConst::CharConst(wchar_t *s, int line, int col): Constant<uint32_t>(line, col) {

}

DTConst::DTConst(wchar_t *s, int line, int col): Constant<uint64_t>(line, col) {
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

bool CompareConditional::Add(int op, Expr *expr) {
	int c = NOP;
	switch (op) {
		case LESS:          if (!(comps & LT)) c = LT; break;
		case LESS_EQUAL:    if (!(comps & LE)) c = LE; break;
		case GREATER:       if(!(comps & GT)) c = GT; break;
		case GREATER_EQUAL: if (!(comps & GE)) c = GE; break;
		case NOT_EQUAL:     if (!(comps & NE)) c = NE; break;
		case EQUAL:         if (!(comps & EQ)) c = EQ; break;
	}
	if (c == NOP) return false;
	
	comps |= c;
	result[results].op = op;
	result[results].expr = expr;
	results++;
	
	return true;
}

void ModuleWhere::normalize() {
	// Removes unnecessary 0s from 'val'.
	wchar_t *norm = coco_string_create(val);
	wchar_t *src = val, *dest = norm;
	int group = 1;
	while (*src) {
		while (*src == L'0') src++; // Move past zeros.
		if (*src == L'\0' || *src == L'.') {
			if (group < 4) *dest++ = L'0'; // Group is a single zero.
			else { dest--; break; } // Ignore fourth group of zeros.
		} else {
			while (*src && *src != L'.') *dest++ = *src++; // Copy group.
		}
		if (*src == L'.') {
			// Move to next group.
			*dest++ = *src++;
			group++;
		}
	}
	*dest = L'\0';
	wcscpy_s(val, coco_string_length(val) + 1, norm);
	coco_string_delete(norm);
}

#define GENERATE_ACCEPT(T)   \
	void T::Accept(Visitor *v) { \
		v->Visit(this);             \
	}

GENERATE_ACCEPT(Assignment)
GENERATE_ACCEPT(BeginLoop)
GENERATE_ACCEPT(BinaryOp)
GENERATE_ACCEPT(Block)
GENERATE_ACCEPT(BooleanConditional)
GENERATE_ACCEPT(Call)
GENERATE_ACCEPT(Case)
GENERATE_ACCEPT(CaseExpr)
GENERATE_ACCEPT(Catch)
GENERATE_ACCEPT(CharConst)
GENERATE_ACCEPT(CompareConditional)
GENERATE_ACCEPT(ComparisonChain)
GENERATE_ACCEPT(ComparisonOp)
GENERATE_ACCEPT(DTConst)
GENERATE_ACCEPT(Do)
GENERATE_ACCEPT(ElseIf)
GENERATE_ACCEPT(Enum)
GENERATE_ACCEPT(Exit)
GENERATE_ACCEPT(Field)
GENERATE_ACCEPT(For)
GENERATE_ACCEPT(ForEach)
GENERATE_ACCEPT(GotoCase)
GENERATE_ACCEPT(Ident)
GENERATE_ACCEPT(If)
GENERATE_ACCEPT(IntConst)
GENERATE_ACCEPT(Module)
GENERATE_ACCEPT(ModuleWhere)
GENERATE_ACCEPT(New)
GENERATE_ACCEPT(Parameter)
GENERATE_ACCEPT(Procedure)
GENERATE_ACCEPT(Raise)
GENERATE_ACCEPT(RealConst)
GENERATE_ACCEPT(Require)
GENERATE_ACCEPT(Return)
GENERATE_ACCEPT(Select)
GENERATE_ACCEPT(Symbol)
GENERATE_ACCEPT(Try)
GENERATE_ACCEPT(UnaryOp)
GENERATE_ACCEPT(When)
GENERATE_ACCEPT(While)

} // namespace AST
