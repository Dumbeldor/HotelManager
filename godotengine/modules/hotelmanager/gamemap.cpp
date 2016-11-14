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
#define CAMERA_NODE String("TileMap/Camera2D")
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
	ObjectTypeDB::bind_method(_MD("_canvas_draw"),&GameMap::_canvas_draw);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_enter"),&GameMap::_canvas_mouse_enter);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_exit"),&GameMap::_canvas_mouse_exit);
	ObjectTypeDB::bind_method(_MD("_on_input_event"),&GameMap::_on_input_event);
}

void GameMap::init()
{
	m_sound_player = get_parent()->get_node(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();
	m_tile_map = get_node(TILEMAP_NODE)->cast_to<TileMap>();
	m_camera = get_node(CAMERA_NODE)->cast_to<Camera2D>();
	assert(m_sound_player && m_tile_map && m_camera);

	m_camera->set_limit(MARGIN_LEFT, -(WORLD_LIMIT_X + 3) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_RIGHT, (WORLD_LIMIT_X + 3) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_BOTTOM, (WORLD_LIMIT_Y + 3 + 4) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_TOP, -(WORLD_LIMIT_Y + 3) * GAME_TILE_SIZE);
	m_camera->set_enable_follow_smoothing(true);
	m_camera->set_pos(Point2(960, 540));

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

	for (uint16_t x = 0; x < WORLD_LIMIT_X + 5; x++) {
		for (uint16_t y = WORLD_LIMIT_Y + 1; y < WORLD_LIMIT_Y + 5; y++) {
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
	// If mouse is over a node
	if (m_mouse_over) {
		int tile_id = m_tile_map->get_cell(m_over_tile.x, m_over_tile.y);
		// We have an object selected and current tile is valid
		if (ObjectSelectorButton::get_selected_object() != TILE_NONE) {
			Vector2 endpoints[4] = {
				m_tile_map->map_to_world(m_over_tile, true),
				m_tile_map->map_to_world((m_over_tile + Point2(1, 0)), true),
				m_tile_map->map_to_world((m_over_tile + Point2(1, 1)), true),
				m_tile_map->map_to_world((m_over_tile + Point2(0, 1)), true)
			};

			Color col;
			if (tile_id != TileMap::INVALID_CELL) {
				col = Color(0.2, 1.0, 0.8, 0.9);
			}
			else {
				col = Color(1.0, 0.4, 0.2, 0.9);
			}

			// Hovering lines
			Vector<Vector2> points;
			for (uint8_t i = 0; i < 4; i++) {
				m_control->draw_line(endpoints[i], endpoints[(i + 1) % 4], col, 2);
				points.push_back(endpoints[i]);
			}

			// Hovering rectangle
			m_control->draw_colored_polygon(points, Color(0.2, 1, 0.4, 0.4));
		}
	}
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

void GameMap::_on_input_event(const InputEvent &p_event)
{
	if (!m_tile_map || !m_tile_map->get_tileset().is_valid() || !m_tile_map->is_visible()) {
		return;
	}

	switch(p_event.type) {
		case InputEvent::MOUSE_MOTION: {
			Vector2 new_over_tile = m_tile_map->world_to_map(get_local_mouse_pos());
			if (new_over_tile != m_over_tile) {
				m_over_tile = new_over_tile;
				m_control->update();
			}
		}
		break;
		case InputEvent::MOUSE_BUTTON: {
			const InputEventMouseButton &mb = p_event.mouse_button;

			if (mb.button_index == BUTTON_LEFT && p_event.is_pressed()) {
				if (!m_mouse_pressed_on_map) {
					m_mouse_pressed_on_map = true;
				}

				place_selected_tile();
			}
		}
		break;
		default: break;
	}
}

void GameMap::place_selected_tile()
{
	GameMapTile s_tile = ObjectSelectorButton::get_selected_object();
	// Ignore none tiles
	if (s_tile == TILE_NONE) {
		return;
	}

	Vector2 tile_pos = m_tile_map->world_to_map(get_local_mouse_pos());
	if (m_tile_map->get_cellv(tile_pos) != s_tile) {
		m_tile_map->set_cellv(tile_pos, s_tile);
		m_sound_player->play(SOUND_POP6);
	}
}
