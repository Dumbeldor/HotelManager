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
 * 2016-2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include "scene/2d/tile_map.h"
#include "tiles.h"
#include <math.h>
#include <math/math_2d.h>
#include <scene/gui/box_container.h>

class Camera2D;
class CanvasModulate;
class GameSession;
class MapControl;
class SamplePlayer;
class SaveGame;

static constexpr uint16_t WORLD_LIMIT_X = 180;
static const Vector2 BASE_RESOLUTION(1920, 1080);
static const uint16_t WORLD_LIMIT_Y = (uint16_t) floor(WORLD_LIMIT_X * BASE_RESOLUTION.y / BASE_RESOLUTION.x);

class GameMap: public VBoxContainer
{
	OBJ_TYPE(GameMap, VBoxContainer);
public:
	GameMap();

	bool init(GameSession *game_session, const Dictionary &map);
	void on_process(float delta);
	void serialize(Dictionary &result) const;
	TileMap *get_ground_map() const { return m_ground_map; };
	TileMap *get_floor_map() const { return m_floor_map; };
	Camera2D *get_camera() const { return m_camera; };
	void apply_daynight_cycle(const double &time);
protected:
	static void _bind_methods();
	void _canvas_draw();
	void _canvas_mouse_enter();
	void _canvas_mouse_exit();
	void _on_input_event(const InputEvent &p_event);
private:
	void generate_map_borders();
	void generate_map();

	static int32_t get_area_cost(const GameTileDef &tiledef, const Vector2 &pos1,
		const Vector2 &pos2);
	static bool is_out_of_bounds(const Vector2 &pos);
	TileMap *get_tilemap(const GameTileDef &tiledef);

	// Selection & Placement
	void init_selection();
	void place_tiles_in_selected_area();
	void reset_selection()
	{
		m_selection_in_progress = false;
		m_selection_init_pos = Vector2(0, 0);
	}

	Vector2 m_selection_init_pos = Vector2(0, 0);
	bool m_selection_in_progress = false;
	bool m_selection_is_valid = true;

	// Camera
	void zoom_camera(const float multiplier);
	void move_camera(Vector2 movement);

	MapControl *m_control = nullptr;
	bool m_mouse_over = false;
	Point2i m_over_tile;

	GameSession *m_game_session = nullptr;
	TileMap *m_ground_map = nullptr;
	TileMap *m_floor_map = nullptr;
	TileMap *m_object_map = nullptr;
	Camera2D *m_camera = nullptr;
	SamplePlayer *m_sound_player = nullptr;
	CanvasModulate *m_canvas_modulate = nullptr;
};
