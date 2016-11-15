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
#include "game_tiles.h"
#include <math.h>
#include <math/math_2d.h>
#include <scene/gui/box_container.h>
#include <scene/2d/camera_2d.h>

class SamplePlayer;

static constexpr uint16_t WORLD_LIMIT_X = 180;
static const Vector2 BASE_RESOLUTION(1920, 1080);
static const uint16_t WORLD_LIMIT_Y = (uint16_t) floor(WORLD_LIMIT_X * BASE_RESOLUTION.y / BASE_RESOLUTION.x);


class GameMap: public VBoxContainer
{
	OBJ_TYPE(GameMap, VBoxContainer);
public:
	GameMap();

	void init();
protected:
	static void _bind_methods();
	void _canvas_draw();
	void _canvas_mouse_enter();
	void _canvas_mouse_exit();
	void _on_input_event(const InputEvent &p_event);
	void _process(float delta);
private:
	uint16_t get_game_cell_size() const { return GAME_TILE_SIZE; }
	Vector2 get_world_limits() const { return Vector2(WORLD_LIMIT_X, WORLD_LIMIT_Y); }
	void place_selected_tile();
	void zoom_camera(const float multiplier);
	void move_camera(Vector2 movement);

	Control *m_control = nullptr;
	bool m_mouse_over = false;
	bool m_mouse_pressed_on_map = false;
	Point2i m_over_tile;

	TileMap *m_tile_map = nullptr;
	Camera2D *m_camera = nullptr;
	SamplePlayer *m_sound_player = nullptr;
};
