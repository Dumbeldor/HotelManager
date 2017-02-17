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
	if (m_tile_menus.find(name) != m_tile_menus.end()) {
		LOG_CRIT("Creating menu %s another time, aborting.", name.c_str());
		assert(false);
	}

	TileMenu *menu = memnew(TileMenu);
	assert(menu);
	menu->init(String(name.c_str()));
	Node *bottom_pane = get_node(String("ControlPane_Bottom"));
	assert(bottom_pane);
	bottom_pane->add_child(menu);

	m_tile_menus[name] = menu;

	menu->connect("pressed", this, ("_on_menu_pressed_" + name).c_str());

	// @TODO this should be more flexible, especially for left & right margin
	// adding a menu index could be the solution

	menu->set_margin(MARGIN_TOP, 0);
	menu->set_margin(MARGIN_BOTTOM, 48);

	int margin_padding = bottom_pane->get_child_count() * 80;
	menu->set_margin(MARGIN_LEFT, margin_padding);
	menu->set_margin(MARGIN_RIGHT, 48 + margin_padding);
}

/**
 * Bind methods for script
 */
void Hud::_bind_methods()
{
	// @TODO try to use templates functions for bound methods ?
	ObjectTypeDB::bind_method(_MD("_on_menu_pressed_floor"), &Hud::_on_tilemenu_pressed, String("floor"));
	ObjectTypeDB::bind_method(_MD("_on_menu_pressed_ground"), &Hud::_on_tilemenu_pressed, String("ground"));
	ObjectTypeDB::bind_method(_MD("_on_menu_pressed_wall"), &Hud::_on_tilemenu_pressed, String("wall"));
	ObjectTypeDB::bind_method(_MD("_on_draw"), &Hud::_on_draw);
}

/**
 * Generic function to swap between tile menus
 * @param menu_name
 */
void Hud::_on_tilemenu_pressed(const String &menu_name)
{
	std::string m_name(menu_name.ascii().get_data());
	for (auto &menu: m_tile_menus) {
		// If menu is current clicked menu
		if (menu.first == m_name) {
			// If it's visible hide it, else show it
			if (menu.second->get_menu()->is_visible()) {
				menu.second->hide_menu();
			}
			else {
				menu.second->show_menu();
			}
		}
		// hide other menus
		else if (menu.second->get_menu()->is_visible()) {
			menu.second->hide_menu();
		}
	}
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
