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

#include <math/math_2d.h>
#include "gamemap.h"

enum GameMapTiles
{
	TILE_NONE = 0,
	TILE_GRASS,
};

void GameMap::_bind_methods()
{
	ObjectTypeDB::bind_method("init",&GameMap::init);
	ObjectTypeDB::bind_method("get_game_cell_size",&GameMap::get_game_cell_size);
}

void GameMap::init()
{
	set_cell_size(Size2(GAME_CELL_SIZE, GAME_CELL_SIZE));
}
