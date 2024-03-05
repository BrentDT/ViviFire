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

#include "Modifier.h"

#if defined(RAW_PTR)
typedef struct Modif* ModifPtr;
#else
typedef std::shared_ptr<struct Modif> ModifPtr;
#endif

bool Modifiers::Add(ModifPtr pMod) {
	if (!pMod) return false;
	std::type_index tid = typeid(*pMod);
	if (mods.count(tid) == 0) {
		mods[tid] = pMod;
		return true;
	}
	parser->errors->Error(pMod->line, pMod->col, L"Same modifier again");
	return false;
}

bool Modifiers::Let(std::type_index tid) {
	if (mods.count(tid) == 1) {
		mods[tid]->can_do = true;
		return true;
	}
	return false;
}

bool Modifiers::Has(std::type_index tid) const {
	return (mods.count(tid) == 1);
}

void Modifiers::Check() const {
	static const std::type_index
		Abstract = typeid(AbstractModif),
		Deprecated = typeid(DeprecatedModif),
		Iterator= typeid(IteratorModif),
		ReadOnly = typeid(ReadOnlyModif),
		Test= typeid(TestModif),
		WriteOnly = typeid(WriteOnlyModif);

// Find those incorrect for a given construct.
	for (auto &m: mods) {
		if (!m.second->can_do) {
			std::wstring msg(m.second->Text());
			msg += L" is not permitted here";
			parser->errors->Error(m.second->line, m.second->col, msg.c_str());
		}
	}

	// Abstract v any except Deprecated, ReadOnly, or WriteOnly.
	if (Has(Abstract) && Count() > 1) {
		for (auto& m : mods) {
			if (m.first != Abstract && m.first != Deprecated && m.first != ReadOnly && m.first != WriteOnly) {
				std::wstring msg(L"Cannot use @ABSTRACT with ");
				msg += m.second->Text();
				parser->errors->Error(m.second->line, m.second->col, msg.c_str());
			}
		}
	}

	// ReadOnly v WriteOnly.
	if (Has(ReadOnly) && Has(WriteOnly)) {
		const auto &m = mods.find (ReadOnly)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @READONLY with @WRITEONLY");
	}

	// Iterator v WriteOnly.
	if (Has(Iterator) && Has(WriteOnly)) {
		const auto &m = mods.find(Iterator)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @ITERATOR with @WRITEONLY");
	}

	// Test v any.
	if (Has(Test) && Count() > 1) {
		const auto &m = mods.find(Test)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @Test with other modifiers");
	}
}
