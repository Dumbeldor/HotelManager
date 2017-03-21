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

#include <cstdint>
#include <string>
#include <ustring.h>

class StringFormatter
{
public:
	StringFormatter() {}
	~StringFormatter() {}
	static String format(const char *fmt, ...);
};

#define STRFMT StringFormatter::format
