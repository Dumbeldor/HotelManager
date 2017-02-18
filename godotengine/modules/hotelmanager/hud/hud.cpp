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

#include <scene/gui/label.h>
#include <iostream>
#include <scene/2d/node_2d.h>
#include <scene/gui/panel.h>
#include <scene/gui/texture_button.h>
#include <scene/gui/box_container.h>
#include <math/math_2d.h>
#include <scene/gui/rich_text_label.h>
#include "modules/hotelmanager/log.h"
#include "hud.h"
#include "modules/hotelmanager/gui_tabs.h"
#include "clock.h"

Hud::Hud(): CanvasLayer()
{

}

void Hud::init()
{
	m_mission_container = get_node(String("ControlPane/MissionPanel/MissionContainer"))->cast_to<VBoxContainer>();
	assert(m_mission_container);

	create_tilemenu("ground");
	create_tilemenu("floor");
	create_tilemenu("wall");

	m_clock = get_node(String("ControlPane/Clock"))->cast_to<Clock>();
	assert(m_clock);
}

/**
 * Create menu with name
 *
 * @param name
 */
void Hud::create_tilemenu(const std::string &name)
{
	Panel *menu = nullptr;

	if (name == "ground") {
		menu = get_node(String("ControlPane_Bottom/HireMenuButton/GroundMenuButton/GroundMenu"))->cast_to<Panel>();
	}
	else if (name == "floor") {
		menu = get_node(String("ControlPane_Bottom/HireMenuButton/FloorMenuButton/FloorMenu"))->cast_to<Panel>();
	}
	else if (name == "wall") {
		menu = get_node(String("ControlPane_Bottom/HireMenuButton/WallMenuButton/WallMenu"))->cast_to<Panel>();
	}

	assert(menu);
	LayerTileMenu *ltm = memnew(LayerTileMenu);
	menu->add_child(ltm);
	ltm->init(String(name.c_str()));
	//ltm->set_scale(Vector2(1 / menu->get_scale().x, 1 / menu->get_scale().y));
}

/**
 * Bind methods for script
 */
void Hud::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_on_draw"), &Hud::_on_draw);
}

/**
 * Hud draw event
 */
void Hud::_on_draw()
{

}

/**
 * Change money label in the player's HUD
 * \attention This function triggers an assert if the HUD element doesn't exists
 *
 * @param money
 */
void Hud::set_money_label(const int64_t &money)
{
	Label *money_label = get_node(String("ControlPane/DayLabel/MoneyLabel"))->
		cast_to<Label>();
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
	RichTextLabel *mission_title = memnew(RichTextLabel);
	mission_title->set_use_bbcode(true);
	mission_title->set_bbcode(String("[b]") + String(mission.title.c_str()) + String("[/b]"));
	mission_title->set_name("mission_" + String::num(mission.id));
	mission_title->set_custom_minimum_size(Size2(200, 15));
	m_mission_container->add_child(mission_title);

	RichTextLabel *mission_desc = memnew(RichTextLabel);
	mission_desc->set_use_bbcode(true);
	mission_desc->set_bbcode(String("[i]") + String(mission.description.c_str()) + String("[/i]"));
	mission_desc->set_name("mission_" + String::num(mission.id) + "_desc");
	mission_desc->set_custom_minimum_size(Size2(200, 15));
	m_mission_container->add_child(mission_desc);

	VBoxContainer *objectives_container = memnew(VBoxContainer);
	m_mission_container->add_child(objectives_container);

	for (const auto &o: mission.objectives) {
		Label *objective = memnew(Label);
		objective->set_text(String("- ") + o->title.c_str());
		objective->set_name("mission_obj_" + String::num(o->id));
		objectives_container->set_margin(MARGIN_LEFT, 20);
		objectives_container->add_child(objective);
	}
}
