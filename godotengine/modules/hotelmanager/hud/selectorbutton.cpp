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
#include "../objectdefmgr.h"
#include "selectorbutton.h"

SelectorButton *SelectorButton::s_selected = nullptr;

#define OBJECTSELECTOR_MASK Color(1.0, 1.0, 1.0, 0.2)
#define OBJECTSELECTOR_SIZE 48

SelectorButton::SelectorButton()
{
	set_size(Size2(OBJECTSELECTOR_SIZE, OBJECTSELECTOR_SIZE));
	connect("pressed", this, "_change_selected_object");
	connect("draw", this, "_on_draw");
}

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

/*
 * Tile Selector
 */

TileSelectorButton::TileSelectorButton(const uint32_t tile_id):
	SelectorButton(),
	m_tile_id(tile_id)
{
	const TileDef &tile_def = ObjectDefMgr::get_singleton()->get_tiledef(m_tile_id);

	ImageTexture *texture = memnew(ImageTexture);
	texture->load(String("res://tiles/") + tile_def.texture_name);

	// Height is multiplied height/width because some textures don't have x = y
	texture->set_size_override(Size2(OBJECTSELECTOR_SIZE,
		OBJECTSELECTOR_SIZE * (texture->get_size().y / texture->get_size().x)));
	set_normal_texture(texture);
	set_tooltip(tile_def.label);
}

/*
 * NPC Selector
 */
NPCSelectorButton::NPCSelectorButton(const CharacterDef &cdef):
	SelectorButton(),
	m_npc_role(cdef.role)
{
	// @TODO set texture from definition
	ImageTexture *texture = memnew(ImageTexture);
	texture->load(String("res://icons/") + cdef.icon);

	// Height is multiplied height/width because some textures don't have x = y
	texture->set_size_override(Size2(OBJECTSELECTOR_SIZE,
		OBJECTSELECTOR_SIZE * (texture->get_size().y / texture->get_size().x)));

	set_normal_texture(texture);
	set_tooltip(cdef.tooltip);
}
