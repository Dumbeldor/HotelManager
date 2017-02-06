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

#include "objectselectorbutton.h"
#include "objectdefmgr.h"
#include <math/math_2d.h>
#include <scene/gui/label.h>
#include <iostream>

#define OBJECTSELECTOR_MASK Color(1.0, 1.0, 1.0, 0.2)

ObjectSelectorButton *ObjectSelectorButton::s_selected = nullptr;
uint32_t ObjectSelectorButton::s_tile_to_init = 0;

ObjectSelectorButton::ObjectSelectorButton():
	m_tile_id(s_tile_to_init)
{
	set_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));
}

void ObjectSelectorButton::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_change_selected_tile"),
			&ObjectSelectorButton::_change_selected_tile);
	ObjectTypeDB::bind_method(_MD("_on_draw"),
			&ObjectSelectorButton::_on_draw);

}

void ObjectSelectorButton::init()
{
	const GameTileDef &tile_def = ObjectDefMgr::get_singleton()->get_tiledef(m_tile_id);

	ImageTexture *texture = memnew(ImageTexture);
	texture->load(String("res://tiles/") + tile_def.texture_name);
	set_normal_texture(texture);

	Label *label = memnew(Label);
	label->set_text(tile_def.label);
	label->set_align(Label::ALIGN_CENTER);
	label->set_margin(MARGIN_RIGHT, GAME_TILE_SIZE);
	label->set_margin(MARGIN_TOP, GAME_TILE_SIZE + 5);
	add_child(label);

	connect("pressed",this,"_change_selected_tile");
	connect("draw", this, "_on_draw");
}

/**
 * When player select the button it changes the selected tile
 */
void ObjectSelectorButton::_change_selected_tile()
{
	ObjectSelectorButton *prev_selected = ObjectSelectorButton::s_selected;
	ObjectSelectorButton::s_selected = this;

	// Redraw previous selected tile to clear hovering, etc...
	if (prev_selected) {
		prev_selected->update();
	}
}

void ObjectSelectorButton::_on_draw()
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

/**
 * Init selector static attributes
 */
void ObjectSelectorButton::init_selector()
{
	s_selected = nullptr;
	s_tile_to_init = TILE_NONE;
}
