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

bool Modifiers::Add(Modif_t *pMod) {
	if (pMod) {
		std::type_index tid = typeid(*pMod);
		if (!Has(tid)) {
			mods.push_back(std::make_pair(tid, std::unique_ptr<Modif_t>(pMod)));
			return true;
		}
		parser->errors->Error(pMod->line, pMod->col, L"Same modifier again");
	}
	return false;
}

bool Modifiers::Let(const std::type_index &tid) {
	if (mods.empty()) return false;
	for (auto &p : mods) {
		if (tid == p.first) return (p.second->can_do = true);
	}
	return false;
}

Modif_t * Modifiers::find(const std::type_index &tid) const {
	for (const auto &p : mods) {
		if (tid == p.first) return p.second.get();
	}
	return nullptr;
}

bool Modifiers::Has(const std::type_index &tid) const {
	return bool(find(tid));
}

void Modifiers::Check() const {
	Modif_t *pMod = nullptr;

// Find those incorrect for a given construct.
	for (const auto &p : mods) {
		if (!p.second->can_do) {
			std::wstring msg(p.second->Text());
			msg += L" is not permitted here";
			parser->errors->Error(p.second->line, p.second->col, msg.c_str());
		}
	}

	// Abstract v any except Deprecated, ReadOnly, or WriteOnly.
	if (Has(s_Abstract) && Count() > 1) {
		for (const auto&p : mods) {
			if (p.first != s_Abstract && p.first != s_Deprecated && p.first != s_ReadOnly && p.first != s_WriteOnly) {
				std::wstring msg(L"Cannot use @ABSTRACT with ");
				msg += p.second->Text();
				parser->errors->Error(p.second->line, p.second->col, msg.c_str());
			}
		}
	}

	// ReadOnly v WriteOnly.
	if ((pMod = find(s_ReadOnly)) && Has(s_WriteOnly)) {
		parser->errors->Error(pMod->line, pMod->col, L"Cannot use @READONLY with @WRITEONLY");
	}

	// Iterator v WriteOnly.
	if ((pMod = find(s_Iterator)) && Has(s_WriteOnly)) {
		parser->errors->Error(pMod->line, pMod->col, L"Cannot use @ITERATOR with @WRITEONLY");
	}

	// Test v any.
	if ((pMod = find(s_Test)) && Count() > 1) {
		parser->errors->Error(pMod->line, pMod->col, L"Cannot use @Test with other modifiers");
	}
}
