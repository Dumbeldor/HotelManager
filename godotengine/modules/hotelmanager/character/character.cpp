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

#include "character.h"
#include "../objectdefmgr.h"

Character::Character(CharacterRole role):
	RigidBody2D(), ActorObject(),
	m_character_role(role)
{
	// Ignore this part in editor
	if (!ObjectDefMgr::get_singleton()) {
		return;
	}
	m_sex = ObjectDefMgr::get_singleton()->get_random_sex();
	if (m_sex == MALE) {
		m_character_name = ObjectDefMgr::get_singleton()->get_random_male_name();
	}
	else if (m_sex == FEMALE){
		m_character_name = ObjectDefMgr::get_singleton()->get_random_female_name();
	}
}

Character::~Character()
{
}

void Character::_bind_methods()
{
	ObjectTypeDB::bind_method("set_sex",&Character::set_sex__api);
	ObjectTypeDB::bind_method("set_role",&Character::set_role__api);
}
