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

#include <scene/main/viewport.h>
#include <iostream>
#include "gamemap.h"
#include "scene/audio/sample_player.h"
#include "objectselectorbutton.h"

#define TILEMAP_NODE String("TileMap")
#define SOUND_PLAYER_NODE String("MapSoundPlayer")
#define SOUND_POP6 String("pop-6")

GameMap::GameMap()
{
	m_control = memnew(Control);
	if (!m_control->is_connected("draw", this, "_canvas_draw")) {
		m_control->connect("draw", this, "_canvas_draw");
	}

	if (!m_control->is_connected("mouse_enter", this, "_canvas_mouse_enter")) {
		m_control->connect("mouse_enter", this, "_canvas_mouse_enter");
	}

	if (!m_control->is_connected("mouse_exit", this, "_canvas_mouse_exit")) {
		m_control->connect("mouse_exit", this, "_canvas_mouse_exit");
	}

	add_child(m_control);
}

void GameMap::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GameMap::init);
	ObjectTypeDB::bind_method("get_game_cell_size", &GameMap::get_game_cell_size);
	ObjectTypeDB::bind_method("get_world_limits", &GameMap::get_world_limits);
	ObjectTypeDB::bind_method("handle_event_mouse_click", &GameMap::handle_event_mouse_click);
	ObjectTypeDB::bind_method(_MD("_canvas_draw"),&GameMap::_canvas_draw);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_enter"),&GameMap::_canvas_mouse_enter);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_exit"),&GameMap::_canvas_mouse_exit);
}

void GameMap::init()
{
	m_sound_player = get_parent()->get_node(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();
	m_tile_map = get_node(TILEMAP_NODE)->cast_to<TileMap>();
	assert(m_sound_player && m_tile_map);

	m_tile_map->set_cell_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));

	// Init map borders
	for (uint16_t x = WORLD_LIMIT_X + 1; x < WORLD_LIMIT_X + 10; x++) {
		for (uint16_t y = 0; y < WORLD_LIMIT_Y + 10; y++) {
			m_tile_map->set_cell(x, y, TILE_NONE);
			m_tile_map->set_cell(-x, y, TILE_NONE);
			m_tile_map->set_cell(x, -y, TILE_NONE);
			m_tile_map->set_cell(-x, -y, TILE_NONE);
		}
	}

	for (uint16_t x = 0; x < WORLD_LIMIT_X + 10; x++) {
		for (uint16_t y = WORLD_LIMIT_Y + 1; y < WORLD_LIMIT_Y + 10; y++) {
			m_tile_map->set_cell(x, y, TILE_NONE);
			m_tile_map->set_cell(-x, y, TILE_NONE);
			m_tile_map->set_cell(x, -y, TILE_NONE);
			m_tile_map->set_cell(-x, -y, TILE_NONE);
		}
	}

	for (uint16_t x = 0; x < WORLD_LIMIT_X + 1; x++) {
		m_tile_map->set_cell(x, WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(-x, WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(x, -WORLD_LIMIT_Y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(-x, -WORLD_LIMIT_Y, TILE_GROUND_GRASS);
	}

	for (uint16_t y = 0; y < WORLD_LIMIT_Y + 1; y++) {
		m_tile_map->set_cell(WORLD_LIMIT_X, y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(-WORLD_LIMIT_X, y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(WORLD_LIMIT_X, -y, TILE_GROUND_GRASS);
		m_tile_map->set_cell(-WORLD_LIMIT_X, -y, TILE_GROUND_GRASS);
	}
}

void GameMap::_canvas_draw()
{
}

void GameMap::_canvas_mouse_enter()
{
	m_mouse_over = true;
	m_control->update();
}

void GameMap::_canvas_mouse_exit()
{
	m_mouse_over = false;
	m_control->update();
}

void GameMap::handle_event_mouse_click(Vector2 pos)
{
	Vector2 tile_pos = m_tile_map->world_to_map(get_local_mouse_pos());
	GameMapTile s_tile = ObjectSelectorButton::get_selected_object();

	if (m_tile_map->get_cellv(tile_pos) != s_tile) {
		m_tile_map->set_cellv(tile_pos, s_tile);
		m_sound_player->play(SOUND_POP6);
	}
}
