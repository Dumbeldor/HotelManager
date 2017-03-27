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
#include "modules/hotelmanager/console.h"
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
#include "scene/main/viewport.h"
#include <algorithm>

#define SOUND_PLAYER_NODE "MapSoundPlayer"
#define ERROR_MESSAGE_GAP 20

Hud::Hud() : CanvasLayer() {}

void Hud::init()
{
	m_mission_container =
	    $("ControlPane/MissionPanel/MissionContainer")->cast_to<MissionHudContainer>();
	assert(m_mission_container);
	m_mission_container->set_hud(this);

	m_sound_player = get_parent()->
		get_parent()->$(SOUND_PLAYER_NODE)->cast_to<SamplePlayer>();

	create_tilemenu("ground");
	create_tilemenu("floor");
	create_tilemenu("wall");
	create_npcmenu();

	m_clock = $("ControlPane/Clock")->cast_to<Clock>();
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
		menu = $("ControlPane_Bottom/HireMenuButton/GroundMenuButton/GroundMenu")
			   ->cast_to<Panel>();
	} else if (name == "floor") {
		menu =
		    $("ControlPane_Bottom/HireMenuButton/FloorMenuButton/FloorMenu")
			->cast_to<Panel>();
	} else if (name == "wall") {
		menu = $("ControlPane_Bottom/HireMenuButton/WallMenuButton/WallMenu")
			   ->cast_to<Panel>();
	}

	assert(menu);
	memnew(TileSelectorMenu(String(name.c_str()), menu));
}

void Hud::create_npcmenu()
{
	Panel *menu = $("ControlPane_Bottom/HireMenuButton/HireMenu")->cast_to<Panel>();
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

				if (!n) {
					continue;
				}

				Node *parent = n->get_parent();

				if (!parent) {
					n->queue_delete();
					continue;
				}

				// Now remove node
				parent->remove_child(n);

				// If parent has no child and is a MissionHudContainer, remove it too
				// if it's not the mission container itself
				if (parent != m_mission_container && parent->get_child_count() == 0
					&& parent->get_type_name() == "MissionHudContainer") {
					parent->get_parent()->remove_child(parent);
					resize_mission_panel();
				}
			}
		}
	}

	if (m_user_errormsg_count > 0) {
		m_user_error_timer -= dtime;
		if (m_user_error_timer <= 0.0f) {
			m_user_error_timer = 10.0f;
			remove_user_error(0);
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
	Label *money_label = $("ControlPane/LabelTop/MoneyPanel/MoneyLabel")->cast_to<Label>();
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
	Label *day_label = $("ControlPane/LabelTop/DayPanel/DayLabel")->cast_to<Label>();
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

	Label *clock_label = $("ControlPane/ClockLabel")->cast_to<Label>();
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
	resize_mission_panel();
}

void Hud::resize_mission_panel()
{
	Panel *mission_panel = $("ControlPane/MissionPanel")->cast_to<Panel>();
	Control *mission_title_label = mission_panel->$("MissionTitleLabel")->cast_to<Control>();
	assert(mission_panel && mission_title_label);

	// 10 * 3 is margin bottom/top of the mission panel + margin top mission container
	float cum_height = mission_title_label->get_size().height + 10 * 3;
	for (uint32_t i = 0; i < m_mission_container->get_child_count(); i++) {
		Control *child = m_mission_container->get_child(i)->cast_to<Control>();
		if (!child) {
			continue;
		}
		cum_height += child->get_size().height;
	}
	mission_panel->set_size(Size2(mission_panel->get_size().width, cum_height));
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

/**
 * Add the specified msg as a user error
 *
 * @param msg
 */
void Hud::add_user_error(const String &msg)
{
	if (m_user_errormsg_count >= 4) {
		remove_user_error(0);
	}

	m_user_errormsg_count++;

	Label *label = memnew(Label);
	label->set_text(msg);
	label->add_color_override("font_color", Color(1, 0, 0));
	label->set_anchor(MARGIN_LEFT, Control::ANCHOR_CENTER);
	$("ErrorContainer")->cast_to<Container>()->add_child(label);
	label->set_margin(MARGIN_LEFT, label->get_size().width / 2);
	label->set_pos(Point2(label->get_pos().x, label->get_pos().y + (m_user_errormsg_count * ERROR_MESSAGE_GAP)));

	LOG_CRIT(msg.utf8().get_data(), "");

    Console *console = get_tree()->get_root()->$("Root/MainMenuLayer/Console")->cast_to<Console>();
    assert(console);
    console->add_error(std::string(msg.utf8().get_data()));
}

/**
 * Remove message with given id from listed errors
 * @param id
 */
void Hud::remove_user_error(const uint8_t id)
{
	Container *error_container = $("ErrorContainer")->cast_to<Container>();
	assert(error_container);
	Label *label = error_container->get_child(id)->cast_to<Label>();
	if (!label) {
		return;
	}

	label->queue_delete();
	if (m_user_errormsg_count != 0) {
		m_user_errormsg_count--;
    }

	// Reinit pos
	
	if (error_container->get_child_count() > id) {
		for (uint8_t i = id; i < error_container->get_child_count(); i++) {
			Label *l = error_container->get_child(i)->cast_to<Label>();
			assert(l);
			l->set_pos(Point2(l->get_pos().x, l->get_pos().y - ERROR_MESSAGE_GAP));
		}
	}

}
