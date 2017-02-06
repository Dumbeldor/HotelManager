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
#include <modules/hotelmanager/missions.h>

class LayerTileMenu;
class Clock;
class VBoxContainer;

class Hud: public CanvasLayer
{
	OBJ_TYPE(Hud, CanvasLayer);
public:
	Hud();
	~Hud() {}

	void init();

	void set_money_label(const int64_t &money);
	void set_day_label(const uint32_t day);
	void modify_clock(const double &time);

	void add_mission(const Mission &mission);
protected:
	static void _bind_methods();
	void _on_draw();
	void _on_floormenu_pressed();
	void _on_groundmenu_pressed();
	void _on_wallmenu_pressed();
private:
	LayerTileMenu *m_ground_menu = nullptr;
	LayerTileMenu *m_floor_menu = nullptr;
	LayerTileMenu *m_wall_menu = nullptr;
	Clock *m_clock = nullptr;
	VBoxContainer *m_mission_container = nullptr;
};
