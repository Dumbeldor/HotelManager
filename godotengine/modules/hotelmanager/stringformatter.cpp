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

#include <cstring>
#include <cstdarg>
#include "stringformatter.h"

String StringFormatter::format(const char *fmt, ...)
{
	char buf[1024];
	assert(strlen(fmt) < 1024);
	va_list argptr;
	va_start(argptr, fmt);
	vsnprintf(buf, sizeof(buf), fmt, argptr);
	va_end(argptr);
	return String(buf);
}
