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
#include "hud.h"
#include "modules/hotelmanager/gui_tabs.h"
#include "clock.h"

Hud::Hud(): CanvasLayer()
{

}

void Hud::init()
{
	TextureButton *ground_menu = get_node(String("ControlPane_Bottom/GroundMenu"))->
		cast_to<TextureButton>();
	assert(ground_menu);

	TextureButton *floor_menu = get_node(String("ControlPane_Bottom/FloorMenu"))->
		cast_to<TextureButton>();
	assert(floor_menu);

	TextureButton *wall_menu = get_node(String("ControlPane_Bottom/WallMenu"))->
		cast_to<TextureButton>();
	assert(wall_menu);


	m_mission_container = get_node(String("ControlPane/MissionPanel/MissionContainer"))->cast_to<VBoxContainer>();
	assert(m_mission_container);

	// Init some HUD elements: note this should be done using a Hud element (see issue #17)
	m_ground_menu = memnew(LayerTileMenu);
	m_ground_menu->init("ground");
	ground_menu->add_child(m_ground_menu);

	m_floor_menu = memnew(LayerTileMenu);
	m_floor_menu->init("floor");
	floor_menu->add_child(m_floor_menu);

	m_wall_menu = memnew(LayerTileMenu);
	m_wall_menu->init("wall");
	wall_menu->add_child(m_wall_menu);

	m_clock = get_node(String("ControlPane/Clock"))->cast_to<Clock>();
	assert(m_clock);
}

/**
 * Bind methods for script
 */
void Hud::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_on_floormenu_pressed"), &Hud::_on_floormenu_pressed);
	ObjectTypeDB::bind_method(_MD("_on_groundmenu_pressed"), &Hud::_on_groundmenu_pressed);
	ObjectTypeDB::bind_method(_MD("_on_wallmenu_pressed"), &Hud::_on_wallmenu_pressed);
	ObjectTypeDB::bind_method(_MD("_on_draw"), &Hud::_on_draw);
}

/**
 * Event when user clicks on ground menu
 */

void Hud::_on_groundmenu_pressed()
{
	// If this menu is shown, hide it
	if (m_ground_menu->is_visible()) {
		m_ground_menu->hide();
		return;
	}

	// Hide other menus
	if (m_floor_menu->is_visible()) {
		m_floor_menu->hide();
	}

	if (m_wall_menu->is_visible()) {
		m_wall_menu->hide();
	}

	m_ground_menu->show();
}

/**
 * Event when user click on floor menu
 */
void Hud::_on_floormenu_pressed()
{
	// If this menu is shown, hide it
	if (m_floor_menu->is_visible()) {
		m_floor_menu->hide();
		return;
	}

	if (m_ground_menu->is_visible()) {
		m_ground_menu->hide();
	}

	if (m_wall_menu->is_visible()) {
		m_wall_menu->hide();
	}

	m_floor_menu->show();
}

/**
 * Event when user clicks on wall menu
 */

void Hud::_on_wallmenu_pressed()
{
	// If this menu is shown, hide it
	if (m_wall_menu->is_visible()) {
		m_wall_menu->hide();
		return;
	}

	// Hide other menus
	if (m_floor_menu->is_visible()) {
		m_floor_menu->hide();
	}

	if (m_ground_menu->is_visible()) {
		m_ground_menu->hide();
	}

	m_wall_menu->show();
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
