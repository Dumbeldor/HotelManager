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

#pragma once

#include "scene/main/node.h"

enum CharacterType
{
	CHARACTER_TYPE_NONE,
};

enum CharacterSex
{
	MALE,
	FEMALE,
};

enum CharacterRole
{
	CHARARACTER_ROLE_NONE,
	CHARARACTER_ROLE_CLIENT,
	CHARARACTER_ROLE_CEO,
	CHARARACTER_ROLE_ROOM_MADE,
	CHARARACTER_ROLE_MAINTENANCE_TECH,
	CHARARACTER_ROLE_COOK,
	CHARARACTER_ROLE_BARTENDER,
	// GROOM ?
	CHARARACTER_ROLE_GARDENER,
	CHARARACTER_ROLE_WAITER,
	CHARARACTER_ROLE_VALET,
	CHARARACTER_ROLE_RECEPTIONIST,
	CHARARACTER_ROLE_WATCHMAN,
	// More ?
};

class Character: public Node
{
	OBJ_TYPE(Character, Node);
public:
	Character() {}
	Character(CharacterType type, CharacterSex sex);

	CharacterSex get_sex() const { return m_sex; }
	void set_sex(const CharacterSex sex) { m_sex = sex; }
	void set_sex__api(const uint8_t sex) { m_sex = (CharacterSex) sex; }

	CharacterRole get_role() const { return m_character_role; }
	void set_role(const CharacterRole role) { m_character_role = role; }
	void set_role__api(const uint8_t role) { m_character_role = (CharacterRole) role; }
protected:
	static void _bind_methods();

private:
	String m_character_name = "";

	CharacterType m_character_type = CHARACTER_TYPE_NONE;
	CharacterRole m_character_role = CHARARACTER_ROLE_NONE;
	CharacterSex m_sex = MALE;
};
