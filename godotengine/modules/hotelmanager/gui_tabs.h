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
#include "game_tiles.h"

class LayerTileMenu: public Tabs
{
	OBJ_TYPE(LayerTileMenu, Tabs);
public:
	LayerTileMenu();
	void init(const TileType tt);
protected:
	static void _bind_methods();
};
