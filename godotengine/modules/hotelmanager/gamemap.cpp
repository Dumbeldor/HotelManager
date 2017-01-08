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

#include <scene/main/canvas_layer.h>
#include <scene/main/viewport.h>
#include <iostream>
#include <vector>
#include <io/base64.h>
#include <os/input.h>
#include <scene/audio/sample_player.h>
#include "gamemap.h"
#include "mapcontrol.h"
#include "objectselectorbutton.h"
#include "objectdefmgr.h"
#include "gamesession.h"
#include "mapgen.h"

#define GROUNDMAP_NODE String("GroundMap")
#define FLOORMAP_NODE String("GroundMap/FloorMap")
#define OBJECTMAP_NODE String("GroundMap/FloorMap/ObjectMap")
#define CAMERA_NODE String("GroundMap/Camera2D")
#define MAPCONTROL_NODE String("Hud/ControlPane/MapControl")
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
	ObjectTypeDB::bind_method(_MD("_canvas_draw"),&GameMap::_canvas_draw);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_enter"),&GameMap::_canvas_mouse_enter);
	ObjectTypeDB::bind_method(_MD("_canvas_mouse_exit"),&GameMap::_canvas_mouse_exit);
	ObjectTypeDB::bind_method(_MD("_on_input_event"),&GameMap::_on_input_event);
}

/**
 * Initialize game map
 *
 * @param game_session
 */
void GameMap::init(GameSession *game_session)
{
	m_game_session = game_session;
	m_sound_player = get_parent()->get_node(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();
	m_ground_map = get_node(GROUNDMAP_NODE)->cast_to<TileMap>();
	m_floor_map = get_node(FLOORMAP_NODE)->cast_to<TileMap>();
	m_object_map = get_node(OBJECTMAP_NODE)->cast_to<TileMap>();
	m_camera = get_node(CAMERA_NODE)->cast_to<Camera2D>();
	m_control = get_node(MAPCONTROL_NODE)->cast_to<MapControl>();
	assert(m_sound_player &&
		m_ground_map && m_floor_map && m_object_map &&
		m_camera && m_control);

	m_control->init(this);

	m_camera->set_limit(MARGIN_LEFT, -(WORLD_LIMIT_X + 1) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_RIGHT, (WORLD_LIMIT_X + 1) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_BOTTOM, (WORLD_LIMIT_Y + 1 + 4) * GAME_TILE_SIZE);
	m_camera->set_limit(MARGIN_TOP, -(WORLD_LIMIT_Y + 1) * GAME_TILE_SIZE);
	m_camera->set_enable_follow_smoothing(true);
	m_camera->set_pos(BASE_RESOLUTION / 2);

	m_ground_map->set_cell_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));
	m_floor_map->set_cell_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));
	m_object_map->set_cell_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));

	generate_map_borders();
	generate_map();
}

/**
 * Delimits the map borders with a NONE tile
 */
void GameMap::generate_map_borders()
{
	for (uint32_t x = WORLD_LIMIT_X + 1; x < WORLD_LIMIT_X + 10; x++) {
		for (uint32_t y = 0; y < WORLD_LIMIT_Y + 10; y++) {
			m_ground_map->set_cell(x, y, TILE_NONE);
			m_ground_map->set_cell(-x, y, TILE_NONE);
			m_ground_map->set_cell(x, -y, TILE_NONE);
			m_ground_map->set_cell(-x, -y, TILE_NONE);
		}
	}

	for (uint32_t x = 0; x < WORLD_LIMIT_X + 5; x++) {
		for (uint32_t y = WORLD_LIMIT_Y + 1; y < WORLD_LIMIT_Y + 5; y++) {
			m_ground_map->set_cell(x, y, TILE_NONE);
			m_ground_map->set_cell(-x, y, TILE_NONE);
			m_ground_map->set_cell(x, -y, TILE_NONE);
			m_ground_map->set_cell(-x, -y, TILE_NONE);
		}
	}
}

void GameMap::generate_map()
{
	MapGen mg;
	for (int32_t x = -WORLD_LIMIT_X; x <= WORLD_LIMIT_X; x++) {
		for (int32_t y = -WORLD_LIMIT_Y; y <= WORLD_LIMIT_Y; y++) {
			m_ground_map->set_cell(x, y, mg.get_tile_for_pos(x, y));
		}
	}
}

/**
 *
 * @param delta
 */
void GameMap::on_process(float delta)
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
			GameMapTile selected_tile_id = ObjectSelectorButton::get_selected_tile_id();
			const GameTileDef &tiledef = ObjectDefMgr::get_tiledef(selected_tile_id);

			TileMap *selected_tilemap = nullptr;
			switch (tiledef.type) {
				case TILE_TYPE_GROUND: selected_tilemap = m_ground_map; break;
				case TILE_TYPE_FLOOR: selected_tilemap = m_floor_map; break;
				default: assert(false); // Unhandled, invalid data
			}

			Matrix32 cell_xf = selected_tilemap->get_cell_transform();
			Matrix32 xform = selected_tilemap->get_global_transform() * m_camera->get_canvas_transform();

			Vector2 endpoints[4];
			Vector2 start_tile = Vector2(MIN(m_over_tile.x, m_selection_init_pos.x),
					MIN(m_over_tile.y, m_selection_init_pos.y)),
			end_tile = Vector2(MAX(m_over_tile.x, m_selection_init_pos.x),
					MAX(m_over_tile.y, m_selection_init_pos.y));

			if (m_selection_in_progress) {
				// If a selection is in progress, points define area rectangle
				endpoints[0] = selected_tilemap->map_to_world(start_tile, true);
				endpoints[1] = selected_tilemap->map_to_world(Vector2(
					MAX(m_over_tile.x, m_selection_init_pos.x),
					MIN(m_over_tile.y, m_selection_init_pos.y)) + Point2(1, 0), true);
				endpoints[2] = selected_tilemap->map_to_world(end_tile + Point2(1, 1), true);
				endpoints[3] = selected_tilemap->map_to_world(Vector2(
					MIN(m_over_tile.x, m_selection_init_pos.x),
					MAX(m_over_tile.y, m_selection_init_pos.y)) + Point2(0, 1), true);
			}
			else {
				endpoints[0] = selected_tilemap->map_to_world(m_over_tile, true);
				endpoints[1] = selected_tilemap->map_to_world((m_over_tile + Point2(1, 0)), true);
				endpoints[2] = selected_tilemap->map_to_world((m_over_tile + Point2(1, 1)), true);
				endpoints[3] = selected_tilemap->map_to_world((m_over_tile + Point2(0, 1)), true);
			}

			for (uint8_t i = 0; i < 4; i++) {
				if (selected_tilemap->get_half_offset() == TileMap::HALF_OFFSET_X
					&& ABS(m_over_tile.y) & 1) {
					endpoints[i] += cell_xf[0] * 0.5;
				}
				if (selected_tilemap->get_half_offset() == TileMap::HALF_OFFSET_Y
					&& ABS(m_over_tile.x) & 1) {
					endpoints[i] += cell_xf[1] * 0.5;
				}
				endpoints[i] = xform.xform(endpoints[i]);
			}

			int tile_id = selected_tilemap->get_cellv(m_over_tile);

			Color border_color;
			if (tile_id != TileMap::INVALID_CELL && tile_id != selected_tile_id) {
				border_color = SELECTOR_BORDER_NEWTILE;
			}
			else {
				border_color = SELECTOR_BORDER_OLDTILE;
			}

			int32_t cost = tiledef.cost;
			int32_t area_width = (int32_t) (ABS(end_tile.x) - ABS(start_tile.x) + 1);
			int32_t area_height = (int32_t) (ABS(end_tile.y) - ABS(start_tile.y) + 1);
			if (m_selection_in_progress) {
				cost *= area_width * area_height;
			}

			for (float x = start_tile.x; x <= end_tile.x; x++) {
				for (float y = start_tile.y; y <= end_tile.y; y++) {
					Vector2 tile_pos(x, y);
					// If tile is identical, reduce cost
					if (selected_tilemap->get_cellv(tile_pos) == selected_tile_id) {
						cost -= tiledef.cost;
					}
				}
			}

			Color selection_color;
			if (is_out_of_bounds(start_tile) || is_out_of_bounds(end_tile) ||
				!m_game_session->has_money(cost)) {
				selection_color = SELECTOR_RECT_COLOR_INVALID;
				border_color = SELECTOR_BORDER_INVALID;
			}
			else {
				selection_color = SELECTOR_RECT_COLOR_VALID;
			}

			MapSelectionInfos selection_infos;
			selection_infos.border_color = border_color;
			selection_infos.selection_color = selection_color;
			selection_infos.endpoints = endpoints;
			selection_infos.area_height = area_height;
			selection_infos.area_width = area_width;
			selection_infos.selection_in_progress = m_selection_in_progress;
			selection_infos.cost = cost;
			selection_infos.tile_hovering =
				(ObjectSelectorButton::get_selected_tile_id() != tile_id);

			m_control->draw_selection(selection_infos);
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

/**
 *
 * @param p_event
 */
void GameMap::_on_input_event(const InputEvent &p_event)
{
	if (!m_ground_map || !m_ground_map->get_tileset().is_valid() || !m_ground_map->is_visible()) {
		return;
	}

	switch(p_event.type) {
		case InputEvent::MOUSE_MOTION: {
			Vector2 new_over_tile = m_ground_map->world_to_map(get_local_mouse_pos());
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

/**
 *
 * @param multiplier
 */
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
	m_camera->update();
}

/**
 *
 * @param movement
 */
void GameMap::move_camera(Vector2 movement)
{
	movement *= m_camera->get_zoom() * m_camera->get_zoom();

	// Limit camera movement to borders
	// @ TODO take account the zoom value because when zooming we are not in map borders
	if ((ABS(m_camera->get_pos().x + movement.x) + BASE_RESOLUTION.x / 2) *
		(1 + m_camera->get_zoom().x / 10) > (WORLD_LIMIT_X - 1) * GAME_TILE_SIZE) {
		movement.x = 0;
	}

	if ((ABS(m_camera->get_pos().y + movement.y) + BASE_RESOLUTION.y / 2) *
		(1 + m_camera->get_zoom().y / 10) > (WORLD_LIMIT_Y - 1) * GAME_TILE_SIZE) {
		movement.y = 0;
	}

	// If there is no movement, abort
	if (movement == Vector2(0, 0)) {
		return;
	}

	m_camera->global_translate(movement);
}

void GameMap::init_selection()
{
	m_selection_in_progress = true;
	m_selection_init_pos = m_ground_map->world_to_map(get_local_mouse_pos());
}

// @TODO checks for tile validity, etc will be added to this function

void GameMap::place_tiles_in_selected_area()
{
	GameMapTile s_tile = ObjectSelectorButton::get_selected_tile_id();
	const GameTileDef &tile_def = ObjectDefMgr::get_tiledef(s_tile);
	// Ignore none tiles & tile unavailable to players
	if (s_tile == TILE_NONE || tile_def.flags & TILE_FLAG_UNAVAILABLE_FOR_PLAYERS) {
		return;
	}

	// If we try to place tiles but no selection in progress, abort
	if (!m_selection_in_progress || !m_selection_is_valid) {
		reset_selection();
		return;
	}

	TileMap *interact_tilemap = nullptr;
	switch (tile_def.type) {
		case TILE_TYPE_GROUND: interact_tilemap = m_ground_map; break;
		case TILE_TYPE_FLOOR: interact_tilemap = m_floor_map; break;
		default: return;
	}

	Vector2 cur_pos = interact_tilemap->world_to_map(get_local_mouse_pos());
	// Don't place nodes if cur_pos is out of bounds
	if (is_out_of_bounds(cur_pos)) {
		reset_selection();
		return;
	}

	const GameTileDef &tiledef = ObjectDefMgr::get_tiledef(s_tile);

	if (cur_pos == m_selection_init_pos) {
		if (!m_game_session->has_money(tiledef.cost)) {
			// m_sound_player->play(SOUND_ERROR);
		}
		if (interact_tilemap->get_cellv(cur_pos) != s_tile) {
			interact_tilemap->set_cellv(cur_pos, s_tile);
			m_sound_player->play(SOUND_POP6);
			m_game_session->remove_money(tiledef.cost);
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

		int32_t cost = (int32_t) (tiledef.cost * ABS(end_tile.x - start_tile.x + 1) *
			ABS(end_tile.y - start_tile.y + 1));

		// Don't place anything if start or end is out of bounds or if we don't have
		// money
		if (is_out_of_bounds(start_tile) || is_out_of_bounds(end_tile)
			|| !m_game_session->has_money(cost)) {
			reset_selection();
			return;
		}

		bool tile_changed = false;

		for (float x = start_tile.x; x <= end_tile.x; x++) {
			for (float y = start_tile.y; y <= end_tile.y; y++) {
				Vector2 tile_pos(x, y);
				// If tile is different, change it
				if (interact_tilemap->get_cellv(tile_pos) != s_tile) {
					tile_changed = true;
					interact_tilemap->set_cellv(tile_pos, s_tile);
				}
				// else tile is identical, just reduce cost
				else {
					cost -= tiledef.cost;
				}
			}
		}

		if (tile_changed) {
			m_sound_player->play(SOUND_POP6);
			m_game_session->remove_money(cost);
		}
	}

	reset_selection();
}

/**
 *
 * @param pos
 * @return
 */
bool GameMap::is_out_of_bounds(const Vector2 &pos)
{
	return (pos.x > WORLD_LIMIT_X || pos.x < -WORLD_LIMIT_X ||
		pos.y > WORLD_LIMIT_Y || pos.y < -WORLD_LIMIT_Y);
}

void GameMap::serialize(Dictionary &result) const
{
	result.clear();
	/**
	 * Serialize Map
	 */
	result["limit_x"] = WORLD_LIMIT_X;
	result["limit_y"] = WORLD_LIMIT_Y;
	int WORLD_LIMIT = ((WORLD_LIMIT_X + 1) * (WORLD_LIMIT_Y + 1)) * 2;
	DVector<char> ground_map;
	DVector<char> floor_map;

	ground_map.resize(WORLD_LIMIT);
	floor_map.resize(WORLD_LIMIT);

	uint16_t i = 0;
	for (int16_t x = -WORLD_LIMIT_X; x <= WORLD_LIMIT_X; x++) {
		for (int16_t y = -WORLD_LIMIT_Y; y <= WORLD_LIMIT_Y; y++) {
			ground_map.push_back((char) m_ground_map->get_cell(x, y));
			floor_map.push_back((char) m_floor_map->get_cell(x, y));
			i++;
		}
	}

	int b64len = WORLD_LIMIT / 3 * 4 + 4 + 1;
	DVector<char> b64buff;
	b64buff.resize(b64len);

	DVector<char>::Write w_ground = ground_map.write();
	DVector<char>::Write w64_ground = b64buff.write();
	int strlen_ground = base64_encode((char*)(&w64_ground[0]), (char*)(&w_ground[0]), (uint32_t )WORLD_LIMIT);
	w64_ground[strlen_ground] = 0;

	DVector<char>::Write w_floor = floor_map.write();
	DVector<char>::Write w64_floor = b64buff.write();
	int strlen_floor = base64_encode((char*)(&w64_floor[0]), (char*)(&w_floor[0]), (uint32_t) WORLD_LIMIT);
	w64_floor[strlen_floor] = 0;

	result["ground"] = (char*)&w64_ground[0];
	result["floor"] = (char*)&w64_floor[0];

	/**
	 * Serialize Camera
	 */
	Dictionary camera;
	camera["pos_x"] = m_camera->get_pos().x;
	camera["pos_y"] = m_camera->get_pos().y;
	camera["zoom_x"] = m_camera->get_zoom().x;
	camera["zoom_y"] = m_camera->get_zoom().y;

	result["camera"] = camera;
}
