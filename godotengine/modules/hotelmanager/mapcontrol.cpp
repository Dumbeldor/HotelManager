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

#include "mapcontrol.h"
#include "gamemap.h"

MapControl::MapControl()
{
}

void MapControl::init(GameMap *map)
{
	if (!is_connected("draw", map, "_canvas_draw")) {
		connect("draw", map, "_canvas_draw");
	}

	if (!is_connected("mouse_enter", map, "_canvas_mouse_enter")) {
		connect("mouse_enter", map, "_canvas_mouse_enter");
	}

	if (!is_connected("mouse_exit", map, "_canvas_mouse_exit")) {
		connect("mouse_exit", map, "_canvas_mouse_exit");
	}
}
