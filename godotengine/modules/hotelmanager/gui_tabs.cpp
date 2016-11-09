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

#include "game_tiles.h"
#include <scene/gui/label.h>
#include <scene/gui/texture_button.h>
#include <math/math_2d.h>
#include "gui_tabs.h"

void GroundTab::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GroundTab::init);
}

void GroundTab::init()
{
	TextureButton *tb = nullptr;
	for (uint16_t i = 0; i < TILE_MAX; i++) {
		TextureButton *tmp = memnew(TextureButton);
		if (tb) {
			tb->add_child(tmp);
			tmp->set_margin(MARGIN_LEFT, FLOOR_TILE_SIZE + 20);
		}
		else {
			add_child(tmp);
			tmp->set_margin(MARGIN_TOP, 20);
			tmp->set_margin(MARGIN_LEFT, 20);
		}

		tb = tmp;

		tb->set_size(Size2(FLOOR_TILE_SIZE, FLOOR_TILE_SIZE));

		ImageTexture *texture = memnew(ImageTexture);
		texture->load(String("res://tiles/") + floor_tile_defs[i].texture_name);
		tb->set_normal_texture(texture);

		Label *label = memnew(Label);
		label->set_text(floor_tile_defs[i].label);
		label->set_align(Label::ALIGN_CENTER);
		label->set_margin(MARGIN_RIGHT, FLOOR_TILE_SIZE);
		label->set_margin(MARGIN_TOP, FLOOR_TILE_SIZE + 5);
		tb->add_child(label);
	}
}
