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

#include "modules/hotelmanager/tiles.h"
#include <math/math_2d.h>
#include <scene/gui/panel.h>
#include <iostream>
#include "selectormenu.h"
#include "selectorbutton.h"
#include "modules/hotelmanager/objectdefmgr.h"
#include "modules/hotelmanager/log.h"

#define LAYER_TILE_MARGIN 20
void SelectorMenu::update_child_pos()
{
	int32_t child_count = get_child_count();
	int32_t osb_id = 0;

	Panel *p = get_parent()->cast_to<Panel>();
	assert(p);

	float panel_width = p->get_size().width;
	float margin_top = LAYER_TILE_MARGIN;
	float margin_left = LAYER_TILE_MARGIN;
	for (int32_t i = 0; i < child_count; i++) {
		SelectorButton *osb = get_child(i)->cast_to<SelectorButton>();
		if (!osb) {
			continue;
		}

		osb->set_margin(MARGIN_TOP, margin_top);
		osb->set_margin(MARGIN_LEFT, margin_left);
		osb_id++;
		margin_left += osb->get_size().width + LAYER_TILE_MARGIN;
		if (margin_left >= panel_width - LAYER_TILE_MARGIN) {
			margin_left = LAYER_TILE_MARGIN;
			margin_top += osb->get_size().height + LAYER_TILE_MARGIN;
		}
	}
}

/*
 * Tile Selector Menu
 */

/**
 * Create the TileSelectorMenu for specified tile_group into menu
 * @param tile_group
 * @param menu
 */
TileSelectorMenu::TileSelectorMenu(const String &tile_group, Panel *menu):
	SelectorMenu()
{
	TileSelectorButton::init_selector();

	menu->add_child(this);

	set_name("SelectorMenu_Tile_" + tile_group);

	const TileGroup &tg_def = ObjectDefMgr::get_singleton()->
		get_tilegroup(std::string(tile_group.utf8()));

	if (tg_def.id == 0) {
		LOG_CRIT("Invalid tile_group name provided (%s), ignoring all comportment.",
			tile_group.ascii().get_data());
		return;
	}

	for (const auto &tile_def: ObjectDefMgr::get_singleton()->get_tiledefs()) {
		if (!tile_def.second->is_in_group(tg_def.id)
			|| tile_def.second->flags & TILE_FLAG_UNAVAILABLE_FOR_PLAYERS) {
			continue;
		}

		add_child(memnew(TileSelectorButton(tile_def.second->id)));
	}

	update_child_pos();
}

/*
 * NPC Selector Menu
 */

NPCSelectorMenu::NPCSelectorMenu(Panel *menu):
	SelectorMenu()
{
	menu->add_child(this);
	set_name("SelectorMenu_NPC");

	for (const auto &cdef: ObjectDefMgr::get_singleton()->get_characterdefs()) {
		add_child(memnew(NPCSelectorButton(*cdef.second)));
	}

	update_child_pos();
}
