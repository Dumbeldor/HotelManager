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

#pragma once

#include <scene/2d/sprite.h>

class Clock: public Sprite
{
	OBJ_TYPE(Clock, Sprite);
public:
	Clock(): Sprite() {}
	~Clock() {}

	void set(const double &time);
	String to_string() const;
private:
	double m_time = 0.0f;
};
