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

#include <math/math_2d.h>
#include <scene/gui/control.h>

class GameMap;

struct MapSelectionInfos
{
	Vector2 *endpoints = nullptr;
	Color border_color;
	Color selection_color;
	int32_t area_width = 0;
	int32_t area_height = 0;
	bool selection_in_progress = false;
	bool tile_hovering = false;
	int32_t cost = 0;
};

class MapControl : public Control
{
	OBJ_TYPE(MapControl, Control);

public:
	MapControl();

	void init(GameMap *map);

	void draw_selection(const MapSelectionInfos &si);

private:
	inline void draw_borders(const Vector2 endpoints[4], const Color &color);
	inline void draw_height_line(const Vector2 start_point, const Vector2 end_point,
				     const Color &color);
	inline void draw_width_line(const Vector2 start_point, const Vector2 end_point,
				    const Color &color);

	Label *m_label_cost = nullptr;
	Label *m_label_width = nullptr;
	Label *m_label_height = nullptr;
};
