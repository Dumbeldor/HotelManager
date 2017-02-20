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

#include <scene/gui/tabs.h>
#include <scene/gui/texture_button.h>
#include "modules/hotelmanager/tiles.h"

class SelectorMenu: public Tabs
{
	OBJ_TYPE(SelectorMenu, Tabs);
public:
	SelectorMenu(const String &tile_group, Panel *menu );
	void update_child_pos();
private:
	SelectorMenu() {} // Private constructor, just for Godot registration
};
