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

uint32_t TileSelectorButton::s_tile_to_init = 0;

TileSelectorButton::TileSelectorButton():
	m_tile_id(s_tile_to_init)
{
	set_size(Size2(OBJECTSELECTOR_SIZE, OBJECTSELECTOR_SIZE));
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

	connect("pressed", this, "_change_selected_object");
	connect("draw", this, "_on_draw");
}

/**
 * Init selector static attributes
 */
void TileSelectorButton::init_selector()
{
	SelectorButton::init_selector();
	s_tile_to_init = TILE_NONE;
}
