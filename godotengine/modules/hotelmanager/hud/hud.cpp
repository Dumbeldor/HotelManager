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

	m_mission_panel = get_node(String("ControlPane/MissionPanel"))->cast_to<Panel>();
	assert(m_mission_panel);

	// Init some HUD elements: note this should be done using a Hud element (see issue #17)
	m_ground_menu = memnew(LayerTileMenu);
	m_ground_menu->init("ground");
	ground_menu->add_child(m_ground_menu);

	m_floor_menu = memnew(LayerTileMenu);
	m_floor_menu->init("floor");
	floor_menu->add_child(m_floor_menu);

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

	m_floor_menu->show();
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
	// @ TODO create mission + objectives onto hud
}
