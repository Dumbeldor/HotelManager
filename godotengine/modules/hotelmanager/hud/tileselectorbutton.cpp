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

#include "tileselectorbutton.h"
#include "../objectdefmgr.h"
#include <scene/gui/label.h>

#define OBJECTSELECTOR_SIZE 48
#define OBJECTSELECTOR_MASK Color(1.0, 1.0, 1.0, 0.2)

TileSelectorButton *TileSelectorButton::s_selected = nullptr;
uint32_t TileSelectorButton::s_tile_to_init = 0;

TileSelectorButton::TileSelectorButton():
	m_tile_id(s_tile_to_init)
{
	set_size(Size2(OBJECTSELECTOR_SIZE, OBJECTSELECTOR_SIZE));
}

void TileSelectorButton::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_change_selected_tile"),
			&TileSelectorButton::_change_selected_tile);
	ObjectTypeDB::bind_method(_MD("_on_draw"),
			&TileSelectorButton::_on_draw);

}

void TileSelectorButton::init()
{
	const GameTileDef &tile_def = ObjectDefMgr::get_singleton()->get_tiledef(m_tile_id);

	ImageTexture *texture = memnew(ImageTexture);
	texture->load(String("res://tiles/") + tile_def.texture_name);

	// Height is multiplied height/width because some textures don't have x = y
	texture->set_size_override(Size2(OBJECTSELECTOR_SIZE,
		OBJECTSELECTOR_SIZE * (texture->get_size().y / texture->get_size().x)));
	set_normal_texture(texture);

	Label *label = memnew(Label);
	label->set_text(tile_def.label);
	label->set_align(Label::ALIGN_CENTER);
	label->set_margin(MARGIN_RIGHT, OBJECTSELECTOR_SIZE);
	label->set_margin(MARGIN_TOP, OBJECTSELECTOR_SIZE + 5);
	add_child(label);

	connect("pressed",this,"_change_selected_tile");
	connect("draw", this, "_on_draw");
}

/**
 * When player select the button it changes the selected tile
 */
void TileSelectorButton::_change_selected_tile()
{
	TileSelectorButton *prev_selected = TileSelectorButton::s_selected;
	TileSelectorButton::s_selected = this;

	// Redraw previous selected tile to clear hovering, etc...
	if (prev_selected) {
		prev_selected->update();
	}
}

void TileSelectorButton::_on_draw()
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
void TileSelectorButton::init_selector()
{
	s_selected = nullptr;
	s_tile_to_init = TILE_NONE;
}
