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

#include <modules/hotelmanager/missions.h>
#include <scene/main/canvas_layer.h>

class SelectorMenu;
class Clock;
class MissionHudContainer;
class TileMenu;
class SamplePlayer;
class CanvasItem;

class Hud : public CanvasLayer
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
	void update_mission_objective(const MissionObjective &objective_def, const uint32_t count);
	void terminate_mission(const uint32_t id);

	void step(float dtime);
	void add_pending_deletion(CanvasItem *n) { m_pending_nodes_for_deletion.push_back(n); }
	void add_user_error(const String &msg);

protected:
	static void _bind_methods();
	void _on_draw();

private:
	void create_tilemenu(const std::string &name);
	void create_npcmenu();

	Clock *m_clock = nullptr;
	SamplePlayer *m_sound_player = nullptr;
	MissionHudContainer *m_mission_container = nullptr;
	std::vector<CanvasItem *> m_pending_nodes_for_deletion = {};
	float m_pending_removal_timer = 0.0f;
};
