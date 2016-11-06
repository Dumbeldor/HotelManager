/**
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

#include "character.h"

Character::Character(CharacterRole role):
	m_character_role(role)
{
	// @ TODO generate random name
}

void Character::_bind_methods()
{
	ObjectTypeDB::bind_method("get_sex",&Character::get_sex);
	ObjectTypeDB::bind_method("set_sex",&Character::set_sex__api);

	ObjectTypeDB::bind_method("get_role",&Character::get_role);
	ObjectTypeDB::bind_method("set_role",&Character::set_role__api);
}
