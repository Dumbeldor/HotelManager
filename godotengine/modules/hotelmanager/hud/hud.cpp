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

#include "hud.h"
#include "clock.h"
#include "modules/hotelmanager/log.h"
#include "selectormenu.h"
#include "missionhudcontainer.h"
#include <iostream>
#include <math/math_2d.h>
#include <scene/2d/node_2d.h>
#include <scene/gui/box_container.h>
#include <scene/gui/label.h>
#include <scene/gui/panel.h>
#include <scene/gui/rich_text_label.h>
#include <scene/gui/texture_button.h>
#include <scene/audio/sample_player.h>
#include <queue>
#include <algorithm>

#define SOUND_PLAYER_NODE String("MapSoundPlayer")

Hud::Hud() : CanvasLayer() {}

void Hud::init()
{
	m_mission_container =
	    get_node(String("ControlPane/MissionPanel/MissionContainer"))
			->cast_to<MissionHudContainer>();
	assert(m_mission_container);
	m_mission_container->set_hud(this);

	m_sound_player = get_parent()->
		get_parent()->get_node(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();

	create_tilemenu("ground");
	create_tilemenu("floor");
	create_tilemenu("wall");
	create_npcmenu();

	m_clock = get_node(String("ControlPane/Clock"))->cast_to<Clock>();
	assert(m_clock);
}

/**
 * Create menu supporting tiles with name
 *
 * @param name
 */
void Hud::create_tilemenu(const std::string &name)
{
	Panel *menu = nullptr;

	if (name == "ground") {
		menu = get_node(
			   String("ControlPane_Bottom/HireMenuButton/GroundMenuButton/GroundMenu"))
			   ->cast_to<Panel>();
	} else if (name == "floor") {
		menu =
		    get_node(String("ControlPane_Bottom/HireMenuButton/FloorMenuButton/FloorMenu"))
			->cast_to<Panel>();
	} else if (name == "wall") {
		menu = get_node(String("ControlPane_Bottom/HireMenuButton/WallMenuButton/WallMenu"))
			   ->cast_to<Panel>();
	}

	assert(menu);
	memnew(TileSelectorMenu(String(name.c_str()), menu));
}

void Hud::create_npcmenu()
{
	Panel *menu =
	    get_node(String("ControlPane_Bottom/HireMenuButton/HireMenu"))->cast_to<Panel>();
	assert(menu);
	// SelectorMenu for NPCs
	memnew(NPCSelectorMenu(menu));
}

/**
 * Bind methods for script
 */
void Hud::_bind_methods() { ObjectTypeDB::bind_method(_MD("_on_draw"), &Hud::_on_draw); }

/**
 * Hud draw event
 */
void Hud::_on_draw() {}

/**
 * Hud event loop
 * @param dtime
 */
void Hud::step(float dtime)
{
	if (!m_pending_nodes_for_deletion.empty()) {
		m_pending_removal_timer -= dtime;
		if (m_pending_removal_timer <= 0.0f) {
			m_pending_removal_timer = 0.05f;
			std::queue<CanvasItem *> removal_queue;
			for (auto &n: m_pending_nodes_for_deletion) {
				n->set_opacity(n->get_opacity() - 0.1f);
				if (n->get_opacity() <= 0.0f) {
					removal_queue.push(n);
				}
			}

			while (!removal_queue.empty()) {
				CanvasItem *n = removal_queue.front();
				removal_queue.pop();
				m_pending_nodes_for_deletion.erase(
					std::remove(m_pending_nodes_for_deletion.begin(),
						m_pending_nodes_for_deletion.end(), n),
					m_pending_nodes_for_deletion.end());

				n->get_parent()->remove_child(n);
			}
		}
	}
}
/**
 * Change money label in the player's HUD
 * \attention This function triggers an assert if the HUD element doesn't exists
 *
 * @param money
 */
void Hud::set_money_label(const int64_t &money)
{
	Label *money_label = get_node(String("ControlPane/DayLabel/MoneyLabel"))->cast_to<Label>();
	assert(money_label);
	money_label->set_text(String::num_int64(money) + " $");
}

/**
 * Change the day label in the player's HUD
 * \attention This function triggers an assert if the HUD element doesn't exists
 *
 * @param day
 */
void Hud::set_day_label(const uint32_t day)
{
	Label *day_label = get_node(String("ControlPane/DayLabel"))->cast_to<Label>();
	assert(day_label);
	day_label->set_text("Day: " + String::num_int64(day));
}

/**
 * Change the hour (temp) label in the player's HUD
 * \attention This function triggers an assert if the HUD element doesn't exists
 *
 * @param time
 */
void Hud::modify_clock(const double &time)
{
	assert(m_clock);
	m_clock->set(time);

	Label *clock_label = get_node(String("ControlPane/ClockLabel"))->cast_to<Label>();
	assert(clock_label);

	clock_label->set_text(m_clock->to_string());
}

/**
 * Add mission to HUD
 *
 * @param mission
 */
void Hud::add_mission(const Mission &mission)
{
	m_mission_container->get_parent()->cast_to<Panel>()->show();
	MissionHudContainer *mc = memnew(MissionHudContainer(mission));
	m_mission_container->add_child(mc);
}

void Hud::update_mission_objective(const MissionObjective &objective_def, const uint32_t count)
{
	m_mission_container->update_mission_objective(objective_def, count);
}

void Hud::terminate_mission(const uint32_t id)
{
	m_sound_player->play("missionsuccess");
	m_mission_container->terminate_mission(id);
}
