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

#include <scene/main/canvas_layer.h>
#include <scene/main/viewport.h>
#include <iostream>
#include <os/input.h>
#include "gamemap.h"
#include "scene/audio/sample_player.h"
#include "objectselectorbutton.h"

#define TILEMAP_NODE String("TileMap")
#define CAMERA_NODE String("TileMap/Camera2D")
#define HUD_NODE String("Hud")
#define MAPCONTROL_NODE String("Hud/ControlPane_Top/MapControl")
#define SOUND_PLAYER_NODE String("MapSoundPlayer")
#define SOUND_POP6 String("pop-6")

#define SELECTOR_BORDER_NEWTILE Color(0.2, 1.0, 0.8, 0.9)
#define SELECTOR_BORDER_OLDTILE Color(1.0, 0.4, 0.2, 0.9)
#define SELECTOR_BORDER_INVALID Color(1, 0, 0, 0.4)
#define SELECTOR_RECT_COLOR_VALID Color(0.2, 1, 0.4, 0.4)
#define SELECTOR_RECT_COLOR_INVALID Color(1, 0.2, 0.3, 0.4)

static constexpr float ZOOMOUT_LIMIT = 8;
static constexpr float ZOOMIN_LIMIT = 0.55;

GameMap::GameMap()
{
	set_process(true);
}

void GameMap::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GameMap::init);
	ObjectTypeDB::bind_method("get_world_limits", &GameMap::get_world_limits);
	ObjectTypeDB::bind_method(_MD("_canvas_draw"),&GameMap::_canvas_draw);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_enter"),&GameMap::_canvas_mouse_enter);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_exit"),&GameMap::_canvas_mouse_exit);
	ObjectTypeDB::bind_method(_MD("_on_input_event"),&GameMap::_on_input_event);
	ObjectTypeDB::bind_method(_MD("_process"),&GameMap::_process);
}

void GameMap::init()
{
	m_sound_player = get_parent()->get_node(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();
	m_tile_map = get_node(TILEMAP_NODE)->cast_to<TileMap>();
	m_camera = get_node(CAMERA_NODE)->cast_to<Camera2D>();
	m_control = get_node(MAPCONTROL_NODE)->cast_to<Control>();
	assert(m_sound_player && m_tile_map && m_camera && m_control);

	if (!m_control->is_connected("draw", this, "_canvas_draw")) {
		m_control->connect("draw", this, "_canvas_draw");
	}

	if (!m_control->is_connected("mouse_enter", this, "_canvas_mouse_enter")) {
		m_control->connect("mouse_enter", this, "_canvas_mouse_enter");
	}

	if (!m_control->is_connected("mouse_exit", this, "_canvas_mouse_exit")) {
		m_control->connect("mouse_exit", this, "_canvas_mouse_exit");
	}

	m_camera->set_limit(MARGIN_LEFT, -(WORLD_LIMIT_X + 3) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_RIGHT, (WORLD_LIMIT_X + 3) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_BOTTOM, (WORLD_LIMIT_Y + 3 + 4) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_TOP, -(WORLD_LIMIT_Y + 3) * GAME_TILE_SIZE);
	m_camera->set_enable_follow_smoothing(true);
	m_camera->set_pos(BASE_RESOLUTION / 2);


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

void GameMap::_process(float delta)
{
	{
		bool should_move_camera = false;
		Vector2 camera_movement(0, 0);
		if (Input::get_singleton()->is_action_pressed("ui_up")) {
			should_move_camera = true;
			camera_movement.y -= 10;
		}

		if (Input::get_singleton()->is_action_pressed("ui_down")) {
			should_move_camera = true;
			camera_movement.y += 10;
		}

		if (Input::get_singleton()->is_action_pressed("ui_left")) {
			should_move_camera = true;
			camera_movement.x -= 10;
		}

		if (Input::get_singleton()->is_action_pressed("ui_right")) {
			should_move_camera = true;
			camera_movement.x += 10;
		}

		if (should_move_camera) {
			move_camera(camera_movement);
		}
	}
}

void GameMap::_canvas_draw()
{
	// If mouse is over a node
	if (m_mouse_over) {
		// We have an object selected and current tile is valid
		if (ObjectSelectorButton::get_selected_tile_id() != TILE_NONE) {
			Matrix32 cell_xf = m_tile_map->get_cell_transform();
			Matrix32 xform = m_tile_map->get_global_transform() * m_camera->get_canvas_transform();

			Vector2 endpoints[4];
			Vector2 start_tile = Vector2(MIN(m_over_tile.x, m_selection_init_pos.x),
					MIN(m_over_tile.y, m_selection_init_pos.y)),
			end_tile = Vector2(MAX(m_over_tile.x, m_selection_init_pos.x),
					MAX(m_over_tile.y, m_selection_init_pos.y));

			if (m_selection_in_progress) {
				// If a selection is in progress, points define area rectangle
				endpoints[0] = m_tile_map->map_to_world(start_tile, true);
				endpoints[1] = m_tile_map->map_to_world(Vector2(
					MAX(m_over_tile.x, m_selection_init_pos.x),
					MIN(m_over_tile.y, m_selection_init_pos.y)) + Point2(1, 0), true);
				endpoints[2] = m_tile_map->map_to_world(end_tile + Point2(1, 1), true);
				endpoints[3] = m_tile_map->map_to_world(Vector2(
					MIN(m_over_tile.x, m_selection_init_pos.x),
					MAX(m_over_tile.y, m_selection_init_pos.y)) + Point2(0, 1), true);
			}
			else {
				endpoints[0] = m_tile_map->map_to_world(m_over_tile, true);
				endpoints[1] = m_tile_map->map_to_world((m_over_tile + Point2(1, 0)), true);
				endpoints[2] = m_tile_map->map_to_world((m_over_tile + Point2(1, 1)), true);
				endpoints[3] = m_tile_map->map_to_world((m_over_tile + Point2(0, 1)), true);
			}

			for (uint8_t i = 0; i < 4;i++) {
				if (m_tile_map->get_half_offset() == TileMap::HALF_OFFSET_X
					&& ABS(m_over_tile.y) & 1) {
					endpoints[i] += cell_xf[0] * 0.5;
				}
				if (m_tile_map->get_half_offset() == TileMap::HALF_OFFSET_Y
					&& ABS(m_over_tile.x) & 1) {
					endpoints[i] += cell_xf[1] * 0.5;
				}
				endpoints[i] = xform.xform(endpoints[i]);
			}

			int tile_id = m_tile_map->get_cellv(m_over_tile);

			Color border_color;
			if (tile_id != TileMap::INVALID_CELL &&
				tile_id != ObjectSelectorButton::get_selected_tile_id()) {
				border_color = SELECTOR_BORDER_NEWTILE;
			}
			else {
				border_color = SELECTOR_BORDER_OLDTILE;
			}

			Color selection_color;
			if (is_out_of_bounds(start_tile) || is_out_of_bounds(end_tile)) {
				selection_color = SELECTOR_RECT_COLOR_INVALID;
				border_color = SELECTOR_BORDER_INVALID;
			}
			else {
				selection_color = SELECTOR_RECT_COLOR_VALID;
			}

			// Hovering lines
			for (uint8_t i = 0; i < 4; i++) {
				m_control->draw_line(endpoints[i], endpoints[(i + 1) % 4], border_color, 2);
			}



			// Hovering rectangle
			if (ObjectSelectorButton::get_selected_tile_id() != tile_id) {
				Vector<Vector2> points;
				for (uint8_t i = 0; i < 4; i++) {
					points.push_back(endpoints[i]);
				}

				m_control->draw_colored_polygon(points, selection_color);
			}
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
			if (p_event.is_action("ui_zoomin")) {
				zoom_camera(0.9);
			}
			else if (p_event.is_action("ui_zoomout")) {
				zoom_camera(1.11);
			}
			else if (mb.button_index == BUTTON_LEFT) {
				if (p_event.is_action_pressed("ui_mouseclick_left")) {
					init_selection();
				}
				else if (p_event.is_action_released("ui_mouseclick_left")) {
					place_tiles_in_selected_area();
				}
			}
		}
		break;
		default: break;
	}
}

void GameMap::zoom_camera(const float multiplier)
{
	Vector2 new_zoom = m_camera->get_zoom() * multiplier;
	if (new_zoom.x > ZOOMOUT_LIMIT || new_zoom.y >= ZOOMOUT_LIMIT
		|| new_zoom.x < ZOOMIN_LIMIT || new_zoom.y < ZOOMIN_LIMIT) {
		return;
	}
	m_camera->set_zoom(new_zoom);

	// This update permits to refresh hovering
	m_control->update();
}

void GameMap::move_camera(Vector2 movement)
{
	movement.x *= 0.65;
	movement.y *= 0.5;
	movement *= m_camera->get_zoom();

	// Limit camera movement to borders
	// @ TODO take account the zoom value because when zooming we are not in map borders
	if (ABS(m_camera->get_pos().x + movement.x - BASE_RESOLUTION.x) > WORLD_LIMIT_X * GAME_TILE_SIZE) {
		movement.x = 0;
	}

	if (ABS(m_camera->get_pos().y + movement.y - BASE_RESOLUTION.y) > WORLD_LIMIT_Y * GAME_TILE_SIZE) {
		movement.y = 0;
	}

	// If there is no movement, abort
	if (movement == Vector2(0, 0)) {
		return;
	}

	m_camera->global_translate(movement * m_camera->get_zoom());
}

void GameMap::init_selection()
{
	m_selection_in_progress = true;
	m_selection_init_pos = m_tile_map->world_to_map(get_local_mouse_pos());
}

// @TODO checks for money, tile validity, etc will be added to this function
void GameMap::place_tiles_in_selected_area()
{
	GameMapTile s_tile = ObjectSelectorButton::get_selected_tile_id();
	// Ignore none tiles
	if (s_tile == TILE_NONE) {
		return;
	}

	// If we try to place tiles but no selection in progress, abort
	if (!m_selection_in_progress || !m_selection_is_valid) {
		reset_selection();
		return;
	}

	Vector2 cur_pos = m_tile_map->world_to_map(get_local_mouse_pos());
	// Don't place nodes if cur_pos is out of bounds
	if (is_out_of_bounds(cur_pos)) {
		reset_selection();
		return;
	}

	if (cur_pos == m_selection_init_pos) {
		if (m_tile_map->get_cellv(cur_pos) != s_tile) {
			m_tile_map->set_cellv(cur_pos, s_tile);
			m_sound_player->play(SOUND_POP6);
		}
	}
	else {
		Vector2 start_tile = Vector2(
			MIN(cur_pos.x, m_selection_init_pos.x),
			MIN(cur_pos.y, m_selection_init_pos.y)
		),
		end_tile = Vector2(
			MAX(cur_pos.x, m_selection_init_pos.x),
			MAX(cur_pos.y, m_selection_init_pos.y)
		);

		// Don't place anything if start or end is out of bounds
		if (is_out_of_bounds(start_tile) || is_out_of_bounds(end_tile)) {
			reset_selection();
			return;
		}

		bool tile_changed = false;

		for (float x = start_tile.x; x <= end_tile.x; x++) {
			for (float y = start_tile.y; y <= end_tile.y; y++) {
				Vector2 tile_pos(x, y);
				if (m_tile_map->get_cellv(tile_pos) != s_tile) {
					tile_changed = true;
					m_tile_map->set_cellv(tile_pos, s_tile);
				}
			}
		}

		if (tile_changed) {
			m_sound_player->play(SOUND_POP6);
		}
	}

	reset_selection();
}

bool GameMap::is_out_of_bounds(const Vector2 &pos)
{
	return (pos.x > WORLD_LIMIT_X || pos.x < -WORLD_LIMIT_X ||
		pos.y > WORLD_LIMIT_Y || pos.y < -WORLD_LIMIT_Y);
}
