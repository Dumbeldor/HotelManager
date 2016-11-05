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

#include "scene/2d/tile_map.h"

#define GAME_CELL_SIZE 96

class GameMap: public TileMap
{
	OBJ_TYPE(GameMap, TileMap);
public:
	GameMap() {}

	void init();
protected:
	static void _bind_methods();
private:
	uint16_t get_game_cell_size() { return GAME_CELL_SIZE; }
};
