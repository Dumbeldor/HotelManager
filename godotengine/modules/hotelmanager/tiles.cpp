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

#include <algorithm>
#include "tiles.h"

bool TileDef::is_in_group(const uint32_t gid) const
{
	return (std::find(groups.begin(), groups.end(), gid) != groups.end());
}
