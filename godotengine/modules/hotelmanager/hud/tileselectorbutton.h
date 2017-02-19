/*
 * This game is under its authors' proprietary license and is property of:
 *
 * No commercial usage of this program could be done without its authors
 * agreement or the current game directors.
 *
 * This license can change to a free license if the game directors decide it.
 *
 * Copyright:
 *
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include "selectorbutton.h"
#include <cassert>

class TileSelectorButton: public SelectorButton
{
	OBJ_TYPE(TileSelectorButton, SelectorButton)

public:
	TileSelectorButton();
	void init();

	uint32_t get_map_tile() const { return m_tile_id; }

	static const uint32_t get_selected_tile_id()
	{
		TileSelectorButton *ts = s_selected->cast_to<TileSelectorButton>();
		return ts ? ts->get_map_tile() : 0;
	}

	static void set_tile_to_init(const uint32_t t) { s_tile_to_init = t; }
	static void init_selector();
private:
	static void _bind_methods() {}

	static uint32_t s_tile_to_init;
	const uint32_t m_tile_id;
};
