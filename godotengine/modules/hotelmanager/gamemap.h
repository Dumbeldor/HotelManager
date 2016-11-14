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

class SamplePlayer;

static constexpr uint16_t WORLD_LIMIT_X = 180;
static const uint16_t WORLD_LIMIT_Y = (uint16_t) floor(WORLD_LIMIT_X * 1080 / 1920);

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
private:
	uint16_t get_game_cell_size() const { return GAME_TILE_SIZE; }
	Vector2 get_world_limits() const { return Vector2(WORLD_LIMIT_X, WORLD_LIMIT_Y); }
	void handle_event_mouse_click(Vector2 pos);

	Control *m_control = nullptr;
	bool m_mouse_over = false;
	TileMap *m_tile_map = nullptr;
	Camera *m_camera = nullptr;
	SamplePlayer *m_sound_player = nullptr;
};
