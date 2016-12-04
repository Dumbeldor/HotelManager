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
#include <cmath>
#include <iostream>
#include "hud.h"

Hud::Hud(): CanvasLayer()
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
	Label *money_label = get_node(String("ControlPane_Top/DayLabel/MoneyLabel"))->
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
	Label *day_label = get_node(String("ControlPane_Top/DayLabel"))->cast_to<Label>();
	assert(day_label);
	day_label->set_text("Day: " + String::num_int64(day));
}

/**
 * Change the hour (temp) label in the player's HUD
 * \attention This function triggers an assert if the HUD element doesn't exists
 *
 * @param time
 */
void Hud::set_hour_clock_label(const double &time)
{
	uint64_t newtime = (uint64_t) std::floor(time);

	Label *clock_label = get_node(String("ControlPane_Top/ClockLabel"))->cast_to<Label>();
	assert(clock_label);

	uint64_t hour_num = ((uint64_t) std::floor(time / 60)) % 24;
	uint64_t min_num = newtime % 60;

	String hour = (hour_num % 24 < 10 ? String("0") : String("")) + String::num(hour_num);
	String min = (min_num < 10 ? "0" : "") + String::num(min_num);
	clock_label->set_text(hour + ":" + min);
}
