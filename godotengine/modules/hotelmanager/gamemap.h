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

#pragma once

#include "scene/2d/tile_map.h"
#include "game_tiles.h"
#include <math.h>
#include <math/math_2d.h>
#include <scene/gui/box_container.h>
#include <scene/2d/camera_2d.h>

class GameSession;
class MapControl;
class SamplePlayer;

static constexpr uint16_t WORLD_LIMIT_X = 180;
static const Vector2 BASE_RESOLUTION(1920, 1080);
static const uint16_t WORLD_LIMIT_Y = (uint16_t) floor(WORLD_LIMIT_X * BASE_RESOLUTION.y / BASE_RESOLUTION.x);

class GameMap: public VBoxContainer
{
	OBJ_TYPE(GameMap, VBoxContainer);
public:
	GameMap();

	void init(GameSession *game_session);
	void on_process(float delta);
protected:
	static void _bind_methods();
	void _canvas_draw();
	void _canvas_mouse_enter();
	void _canvas_mouse_exit();
	void _on_input_event(const InputEvent &p_event);
private:
	static bool is_out_of_bounds(const Vector2 &pos);

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
};
