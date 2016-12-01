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

#include <scene/main/canvas_layer.h>

class Hud: public CanvasLayer
{
	OBJ_TYPE(Hud, CanvasLayer);
public:
	Hud();
	~Hud() {}

	void set_money_label(const int64_t &money);
	void set_day_label(const uint32_t day);
};