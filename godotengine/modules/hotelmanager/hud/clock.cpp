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
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "clock.h"
#include <cmath>
#include <iostream>

#define CLOCK_MIN_SPRITE "ClockMin"
#define CLOCK_HOUR_SPRITE "ClockHour"

void Clock::set(const double &time)
{
	m_time = time;
	Sprite *minclock = $(CLOCK_MIN_SPRITE)->cast_to<Sprite>();
	assert(minclock);
	Sprite *hourclock = $(CLOCK_HOUR_SPRITE)->cast_to<Sprite>();
	assert(hourclock);

	minclock->set_rotd(360.0f - ((uint64_t)(m_time * 60) % 3600) / 10.0f);
	hourclock->set_rotd(360.0f - (((uint64_t) m_time % 720) / 2.0f));
}

String Clock::to_string() const
{
	uint64_t newtime = (uint64_t) std::floor(m_time);

	uint64_t hour_num = ((uint64_t) std::floor(m_time / 60)) % 24;
	uint64_t min_num = newtime % 60;

	String hour = (hour_num % 24 < 10 ? String("0") : String("")) + String::num(hour_num);
	String min = (min_num < 10 ? "0" : "") + String::num(min_num);
	return hour + ":" + min;
}
