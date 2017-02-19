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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include <scene/gui/texture_button.h>
#include <cassert>

class TileSelectorButton: public TextureButton
{
	OBJ_TYPE(TileSelectorButton, TextureButton)

public:
	TileSelectorButton();
	void init();

	uint32_t get_map_tile() const { return m_tile_id; }

	void _change_selected_tile();

	static const uint32_t get_selected_tile_id()
	{
		return s_selected ? s_selected->get_map_tile() : 0;
	}

	static void set_tile_to_init(const uint32_t t) { s_tile_to_init = t; }

	void _on_draw();
	static void init_selector();
protected:
	static void _bind_methods();

private:
	static TileSelectorButton *s_selected;

	static uint32_t s_tile_to_init;
	const uint32_t m_tile_id;
};
