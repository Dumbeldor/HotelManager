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

class Character: public Node
{
	OBJ_TYPE(Character, Node);
public:
	Character():
		m_character_name(""),
		m_character_type(CHARACTER_TYPE_NONE),
		m_sex(MALE) {}

	Character(CharacterType type, CharacterSex sex);

	CharacterSex get_sex() const { return m_sex; }
	void set_sex(const CharacterSex sex) { m_sex = sex; }
	void set_sex__api(const uint8_t sex) { m_sex = (CharacterSex) sex; }
protected:
	static void _bind_methods();

private:
	String m_character_name;

	CharacterType m_character_type;
	CharacterSex m_sex;
};
