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

	GameMapTile get_map_tile() const { return m_tile_id; }

	void _change_selected_tile();

	static const GameMapTile get_selected_tile_id()
	{
		return s_selected ? s_selected->get_map_tile() : TILE_NONE;
	}

	static void set_tile_to_init(const GameMapTile t) { s_tile_to_init = t; }

	void _on_draw();
protected:
	static void _bind_methods();

private:
	static ObjectSelectorButton *s_selected;

	static GameMapTile s_tile_to_init;
	const GameMapTile m_tile_id;
};
