/**
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
#include "game_tiles.h"

class ObjectSelectorButton: public TextureButton
{
	OBJ_TYPE(ObjectSelectorButton, TextureButton)
public:
	ObjectSelectorButton();
	void init();

	void set_map_tile(const GameMapTile t)
	{
		assert(m_tile < TILE_MAX);
		m_tile = t;
	}

	void _change_selected_tile();

	static const GameMapTile get_selected_object() { return s_selected_object; }

protected:
	static void _bind_methods();

private:
	static GameMapTile s_selected_object;

	GameMapTile m_tile = TILE_NONE;
};
