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

/*
 * Generic Selector menu interface
 */
class SelectorMenu: public Tabs
{
	OBJ_TYPE(SelectorMenu, Tabs);
public:
	void update_child_pos();
protected:
	SelectorMenu(): Tabs() {} // Protected constructor, just for Godot registration
};

/*
 * Tile selector menu
 */
class TileSelectorMenu: public SelectorMenu
{
	OBJ_TYPE(TileSelectorMenu, SelectorMenu);
public:
	TileSelectorMenu(const String &tile_group, Panel *menu);
private:
	TileSelectorMenu() {} // Private constructor, just for Godot registration
};

class NPCSelectorMenu: public SelectorMenu
{
OBJ_TYPE(NPCSelectorMenu, SelectorMenu);
public:
	NPCSelectorMenu(Panel *menu);
private:
	NPCSelectorMenu() {} // Private constructor, just for Godot registration
};
