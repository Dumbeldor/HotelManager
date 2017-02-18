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

#include "tiles.h"
#include <math/math_2d.h>
#include <iostream>
#include "gui_tabs.h"
#include "objectselectorbutton.h"
#include "objectdefmgr.h"
#include "log.h"

LayerTileMenu::LayerTileMenu()
{
	// Init ObjectSelectorButton when init this menu element, should be good
	ObjectSelectorButton::init_selector();
}

void LayerTileMenu::init(const String &tile_group)
{

	set_name("LayerMenuTileType_" + tile_group);

	const TileGroup &tg_def = ObjectDefMgr::get_singleton()->
		get_tilegroup(std::string(tile_group.utf8()));
	if (tg_def.id == 0) {
		LOG_CRIT("Invalid tile_group name provided (%s), ignoring all comportment.",
			tile_group.ascii().get_data());
		return;
	}

	uint16_t tile_id = 0;
	for (const auto &tile_def: ObjectDefMgr::get_singleton()->get_tiledefs()) {
		if (!tile_def.second->is_in_group(tg_def.id)
			|| tile_def.second->flags & TILE_FLAG_UNAVAILABLE_FOR_PLAYERS) {
			continue;
		}

		ObjectSelectorButton::set_tile_to_init(tile_def.second->id);
		ObjectSelectorButton *tb = memnew(ObjectSelectorButton);
		add_child(tb);
		tb->set_margin(MARGIN_TOP, 20);
		tb->set_margin(MARGIN_LEFT, 20 + (48 + 20) * tile_id);
		tb->init();
		tile_id++;
	}
}

void LayerTileMenu::_bind_methods()
{
}
