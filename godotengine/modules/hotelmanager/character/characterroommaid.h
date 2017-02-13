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
 * 2016, Jeremy Lomoro <jeremy.lomoro@tuxsrv.fr>
 *
 * All rights reserved
 */

#pragma once

#include "character.h"

class CharacterRoomMaid: public Character
{
	OBJ_TYPE(CharacterRoomMaid, Character);
public:
	CharacterRoomMaid(): Character(CHARACTER_ROLE_ROOM_MADE) {}
protected:
	static void _bind_methods() {}
};
