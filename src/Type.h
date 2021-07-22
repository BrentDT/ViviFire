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

#if !defined(_TYPE_H_)
#define _TYPE_H_

#include <climits>
#include <list>

namespace Type {

struct Type {};

struct Primitive : public Type {
	wchar_t *id;
	size_t bits;

	Primitive(wchar_t *id, size_t bits): id(id), bits(bits) {}
};

struct Boolean : public Primitive {
	Boolean(): Primitive(L"Boolean", 1) {}
};

struct DateTime : public Primitive {
	DateTime(): Primitive(L"DateTime", 64) {}
};

struct Char : public Primitive {
	Char(): Primitive(L"Char", 32) {}
};

struct String : public Primitive {
	String(): Primitive(L"String", 0) {} // FIXME: choose a size
};

struct Int8 : public Primitive {
	Int8(): Primitive(L"Int8", 8) {}
};

struct UInt8 : public Primitive {
	UInt8(): Primitive(L"UInt8", 8) {}
};

struct Int16 : public Primitive {
	Int16(): Primitive(L"Int16", 16) {}
};

struct UInt16 : public Primitive {
	UInt16(): Primitive(L"UInt16", 16) {}
};

struct Int32 : public Primitive {
	Int32(): Primitive(L"Int32", 32) {}
};
struct UInt32 : public Primitive {
	UInt32(): Primitive(L"UInt32", 32) {}
};

struct Int : public Primitive {
	Int(): Primitive(L"Int", CHAR_BIT * sizeof(int)) {}
};

struct UInt : public Primitive {
	UInt(): Primitive(L"UInt", CHAR_BIT * sizeof(int)) {}
};

struct Int64 : public Primitive {
	Int64(): Primitive(L"Int64", 64) {}
};

struct UInt64 : public Primitive {
	UInt64(): Primitive(L"UInt64", 64) {}
};

struct Int128 : public Primitive {
	Int128(): Primitive(L"Int128", 128) {}
};

struct UInt128 : public Primitive {
	UInt128(): Primitive(L"UInt128", 128) {}
};

struct Real32 : public Primitive {
	Real32(): Primitive(L"Real32", 32) {}
};

struct Real64 : public Primitive {
	Real64(): Primitive(L"Real64", 64) {}
};

struct Real : public Primitive {
	Real(): Primitive(L"Real", 80) {} // FIXME: different sizes on other platforms
};

struct Real128 : public Primitive {
	Real128(): Primitive(L"Real128", 128) {}
};

struct Fixed64 : public Primitive {
	Fixed64(): Primitive(L"Fixed64", 64) {}
};

struct Fixed128 : public Primitive {
	Fixed128(): Primitive(L"Fixed128", 128) {}
};

struct Array : public Type {
	std::list<size_t> dims;
	Type *type;
};

struct Class : public Type {
	Class *parent;
};

struct Enum : public Primitive {
	bool flags;
	long long value = 0LL;
	Enum(bool fl, wchar_t *id, size_t sz): flags(fl), Primitive(id, sz) {}
};

struct Event : public Type {};

struct Struct : public Type {};

struct Unit : public Type {};

} // namespace

#endif // _TYPE_H_
