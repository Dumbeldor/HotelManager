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

#include "objectselectorbutton.h"
#include <math/math_2d.h>
#include <scene/gui/label.h>
#include <iostream>

GameMapTile ObjectSelectorButton::s_selected_object = TILE_NONE;

ObjectSelectorButton::ObjectSelectorButton()
{
	assert(m_tile < TILE_MAX);
	set_size(Size2(GAME_TILE_SIZE, GAME_TILE_SIZE));
}

void ObjectSelectorButton::_bind_methods()
{
	ObjectTypeDB::bind_method( _MD("_change_selected_tile"),
			&ObjectSelectorButton::_change_selected_tile);

}

void ObjectSelectorButton::init()
{
	ImageTexture *texture = memnew(ImageTexture);
	texture->load(String("res://tiles/") + game_tile_defs[m_tile].texture_name);
	set_normal_texture(texture);

	Label *label = memnew(Label);
	label->set_text(game_tile_defs[m_tile].label);
	label->set_align(Label::ALIGN_CENTER);
	label->set_margin(MARGIN_RIGHT, GAME_TILE_SIZE);
	label->set_margin(MARGIN_TOP, GAME_TILE_SIZE + 5);
	add_child(label);

	connect("pressed",this,"_change_selected_tile");
}


// When player select the button it changes the selected tile for this
void ObjectSelectorButton::_change_selected_tile()
{
	ObjectSelectorButton::s_selected_object = m_tile;
}
