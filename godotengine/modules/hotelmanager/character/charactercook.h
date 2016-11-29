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
 * 2016, Vincent Glize <vincent.glize@minegame.fr>
 *
 * All rights reserved
 */

#pragma once

#include "../character.h"

class CharacterCook: public Character
{
	OBJ_TYPE(CharacterCook, Character);
public:
	CharacterCook(): Character(CHARARACTER_ROLE_COOK) {}

protected:
	static void _bind_methods() {}
};
