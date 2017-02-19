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
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include <math/math_2d.h>
#include "selectorbutton.h"

SelectorButton *SelectorButton::s_selected = nullptr;

#define OBJECTSELECTOR_MASK Color(1.0, 1.0, 1.0, 0.2)

void SelectorButton::init_selector()
{
	s_selected = nullptr;
}

void SelectorButton::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_change_selected_object"),
		&SelectorButton::_change_selected_object);
	ObjectTypeDB::bind_method(_MD("_on_draw"),
		&SelectorButton::_on_draw);
}

/**
 * When player select the button it changes the selected tile
 */
void SelectorButton::_change_selected_object()
{
	SelectorButton *prev_selected = SelectorButton::s_selected;
	SelectorButton::s_selected = this;

	// Redraw previous selected tile to clear hovering, etc...
	if (prev_selected) {
		prev_selected->update();
	}
}

void SelectorButton::_on_draw()
{
	if (s_selected == this) {
		static const Color col(0.2, 1.0, 0.8, 0.4);
		const Size2 size = get_size();
		const Vector2 endpoints[4] = {
			get_pos() + Vector2(-2 - get_margin(MARGIN_LEFT), -2 - get_margin(MARGIN_TOP)),
			get_pos() + Vector2(size.x + 2 - get_margin(MARGIN_LEFT), -2 -get_margin(MARGIN_TOP)),
			get_pos() + Vector2(size.x + 2 - get_margin(MARGIN_LEFT), size.y + 2 - get_margin(MARGIN_TOP)),
			get_pos() + Vector2(-2 - get_margin(MARGIN_LEFT), size.y + 2 - get_margin(MARGIN_TOP))
		};

		Vector<Vector2> points;
		for (uint8_t i = 0; i < 4; i++) {
			draw_line(endpoints[i], endpoints[(i + 1) % 4], col, 2);
			points.push_back(endpoints[i]);
		}

		draw_colored_polygon(points, OBJECTSELECTOR_MASK);
	}
}
