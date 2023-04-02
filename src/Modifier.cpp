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

#include <assert.h>
#include "Modifier.h"
#include "Parser.h"
#include "Scanner.h"

void Modif::add(int m) {
	assert(m >= 0 && m < LAST);
	if (m_mods[m].present) {
		m_parser->errors->Error(m_parser->t->line, m_parser->t->col, L"Same modifier again");
	}
	else {
		m_mods[m].present = true;
		m_mods[m].allowed = false;
		m_mods[m].line = m_parser->t->line;
		m_mods[m].col = m_parser->t->col;
		m_mods[m].order = m_count++;
	}
}

bool Modif::allow(int m) {
	assert(m < LAST);
	if (m_mods[m].present) {
		m_mods[m].allowed = true;
	}
	return m_mods[m].present;
}

bool Modif::has(int m) const {
	assert(m >= 0 && m < LAST);
	return m_mods[m].present; 
}

void Modif::validate() const {
	if (m_count == 0) return; // No modifiers.

	for (int m = 0; m < LAST; m++) {
		if (m_mods[m].present && !m_mods[m].allowed) {
			wchar_t *msg = coco_string_create_append(m_name[m], L" is not permitted here");
			m_parser->errors->Error(m_mods[m].line, m_mods[m].col, msg);
			coco_string_delete(msg);
		}
	}

	// Abstract v any except Deprecated, ReadOnly, or WriteOnly
	if (m_mods[Abstract].present && m_count > 1) {
		for (int m = 0; m < LAST; m++) {
			switch (m) {
			case Abstract: case Deprecated: case ReadOnly: case WriteOnly:
				break; // Skip permitted modifiers.
			default:
				if (m_mods[m].present) {
					wchar_t *msg = coco_string_create_append(L"Cannot use @Abstract with ", m_name[m]);
					m_parser->errors->Error(m_mods[m].line, m_mods[m].col, msg);
					coco_string_delete(msg);
				}
			}
		}
	}

	// ReadOnly v WriteOnly
	if (m_mods[ReadOnly].present && m_mods[WriteOnly].present) {
		m_parser->Err(m_mods[WriteOnly].line, m_mods[WriteOnly].col, L"Cannot use @ReadOnly and @WriteOnly together");
	}
}
