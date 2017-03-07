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

#define LOG_WARN(fmt, ...)                                                                         \
	{                                                                                          \
		char buf[10240] = {};                                                              \
		sprintf(buf, fmt, __VA_ARGS__);                                                    \
		WARN_PRINT(buf);                                                                   \
	}

#define LOG_CRIT(fmt, ...)                                                                         \
	{                                                                                          \
		char buf[10240] = {};                                                              \
		sprintf(buf, fmt, __VA_ARGS__);                                                    \
		ERR_PRINT(buf);                                                                    \
	}
