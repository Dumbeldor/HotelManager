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
	ObjectTypeDB::bind_method("get_world_limits",&GameMap::get_world_limits);
}

void GameMap::init()
{
	set_cell_size(Size2(GAME_CELL_SIZE, GAME_CELL_SIZE));

	// Init map borders
	for (uint16_t x = WORLD_LIMIT_X + 1; x < WORLD_LIMIT_X + 10; x++) {
		for (uint16_t y = 0; y < WORLD_LIMIT_Y + 10; y++) {
			set_cell(x, y, TILE_NONE);
			set_cell(-x, y, TILE_NONE);
			set_cell(x, -y, TILE_NONE);
			set_cell(-x, -y, TILE_NONE);
		}
	}

	for (uint16_t x = 0; x < WORLD_LIMIT_X + 10; x++) {
		for (uint16_t y = WORLD_LIMIT_Y + 1; y < WORLD_LIMIT_Y + 10; y++) {
			set_cell(x, y, TILE_NONE);
			set_cell(-x, y, TILE_NONE);
			set_cell(x, -y, TILE_NONE);
			set_cell(-x, -y, TILE_NONE);
		}
	}

	for (uint16_t x = 0; x < WORLD_LIMIT_X + 1; x++) {
		set_cell(x, WORLD_LIMIT_Y, TILE_GRASS);
		set_cell(-x, WORLD_LIMIT_Y, TILE_GRASS);
		set_cell(x, -WORLD_LIMIT_Y, TILE_GRASS);
		set_cell(-x, -WORLD_LIMIT_Y, TILE_GRASS);
	}

	for (uint16_t y = 0; y < WORLD_LIMIT_Y + 1; y++) {
		set_cell(WORLD_LIMIT_X, y, TILE_GRASS);
		set_cell(-WORLD_LIMIT_X, y, TILE_GRASS);
		set_cell(WORLD_LIMIT_X, -y, TILE_GRASS);
		set_cell(-WORLD_LIMIT_X, -y, TILE_GRASS);
	}
}
