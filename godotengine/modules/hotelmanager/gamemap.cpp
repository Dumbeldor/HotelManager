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

#include <cassert>
#include "gamemap.h"
#include "game_tiles.h"
#include "scene/audio/sample_player.h"
#include "objectselectorbutton.h"

#define SOUND_PLAYER_NODE String("GameMapSoundPlayer")
#define SOUND_POP6 String("pop-6")

void GameMap::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GameMap::init);
	ObjectTypeDB::bind_method("get_game_cell_size", &GameMap::get_game_cell_size);
	ObjectTypeDB::bind_method("get_world_limits", &GameMap::get_world_limits);
	ObjectTypeDB::bind_method("handle_event_mouse_click", &GameMap::handle_event_mouse_click);
}

void GameMap::init()
{
	m_sound_player = dynamic_cast<SamplePlayer *>(get_node(SOUND_PLAYER_NODE));
	assert(m_sound_player);

	set_cell_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));

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
		set_cell(x, WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		set_cell(-x, WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		set_cell(x, -WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		set_cell(-x, -WORLD_LIMIT_Y, TILE_GROUND_GRASS);
	}

	for (uint16_t y = 0; y < WORLD_LIMIT_Y + 1; y++) {
		set_cell(WORLD_LIMIT_X, y, TILE_GROUND_GRASS);
		set_cell(-WORLD_LIMIT_X, y, TILE_GROUND_GRASS);
		set_cell(WORLD_LIMIT_X, -y, TILE_GROUND_GRASS);
		set_cell(-WORLD_LIMIT_X, -y, TILE_GROUND_GRASS);
	}
}

void GameMap::handle_event_mouse_click(Vector2 pos)
{
	Vector2 tile_pos = world_to_map(get_local_mouse_pos());
	GameMapTile s_tile = ObjectSelectorButton::get_selected_object();

	if (get_cellv(tile_pos) != s_tile) {
		set_cellv(tile_pos, s_tile);
		m_sound_player->play(SOUND_POP6);
	}
}
