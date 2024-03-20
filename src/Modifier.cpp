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

bool Modifiers::Let(const std::type_index &tid) {
	if (mods.empty() || mods.count(tid) == 0) return false;
	mods[tid]->can_do = true;
	return true;
}

bool Modifiers::Has(const std::type_index &tid) const {
	return (mods.count(tid) == 1);
}

void Modifiers::Check() const {
// Find those incorrect for a given construct.
	for (auto &m: mods) {
		if (!m.second->can_do) {
			std::wstring msg(m.second->Text());
			msg += L" is not permitted here";
			parser->errors->Error(m.second->line, m.second->col, msg.c_str());
		}
	}

	// Abstract v any except Deprecated, ReadOnly, or WriteOnly.
	if (Has(s_Abstract) && Count() > 1) {
		for (auto& m : mods) {
			if (m.first != s_Abstract && m.first != s_Deprecated && m.first != s_ReadOnly && m.first != s_WriteOnly) {
				std::wstring msg(L"Cannot use @ABSTRACT with ");
				msg += m.second->Text();
				parser->errors->Error(m.second->line, m.second->col, msg.c_str());
			}
		}
	}

	// ReadOnly v WriteOnly.
	if (Has(s_ReadOnly) && Has(s_WriteOnly)) {
		const auto &m = mods.find (s_ReadOnly)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @READONLY with @WRITEONLY");
	}

	// Iterator v WriteOnly.
	if (Has(s_Iterator) && Has(s_WriteOnly)) {
		const auto &m = mods.find(s_Iterator)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @ITERATOR with @WRITEONLY");
	}

	// Test v any.
	if (Has(s_Test) && Count() > 1) {
		const auto &m = mods.find(s_Test)->second;
		parser->errors->Error(m->line, m->col, L"Cannot use @Test with other modifiers");
	}
}
