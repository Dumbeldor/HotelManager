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

#pragma once

#include <scene/main/node.h>
#include <string>
#include <scene/2d/physics_body_2d.h>
#include "modules/hotelmanager/actorobject.h"

enum CharacterSex: uint8_t
{
	MALE = 1,
	FEMALE = 2,
	SEX_MAX,
};

enum CharacterRole: uint8_t
{
	CHARACTER_ROLE_NONE = 0,
	CHARACTER_ROLE_CLIENT = 1,
	CHARACTER_ROLE_CEO = 2,
	CHARACTER_ROLE_ROOM_MADE = 3,
	CHARACTER_ROLE_MAINTENANCE_TECH = 4,
	CHARACTER_ROLE_COOK = 5,
	CHARACTER_ROLE_BARTENDER = 6,
	// GROOM ?
	CHARACTER_ROLE_GARDENER = 7,
	CHARACTER_ROLE_WAITER = 8,
	CHARACTER_ROLE_VALET = 9,
	CHARACTER_ROLE_RECEPTIONIST = 10,
	CHARACTER_ROLE_WATCHMAN = 11,
	// More ?
	CHARACTER_ROLE_MAX = 12,
};

struct CharacterDef
{
	CharacterDef() {}
	uint16_t id = 0;
	std::string name = "";
	String tooltip = "";
	String icon = "";
	String scene = "";
	CharacterRole role = CHARACTER_ROLE_NONE;
	float sex_chance = 50.0f;
	uint32_t cost = 0;
};
static const uint8_t CHARACTERDEF_CSV_COLS = 8;

class Character: public RigidBody2D, public ActorObject
{
	OBJ_TYPE(Character, RigidBody2D);
public:
	Character(): RigidBody2D(), ActorObject() {}
	Character(CharacterRole role);

	virtual ~Character();

	CharacterSex get_sex() const { return m_sex; }
	void set_sex(const CharacterSex sex) { m_sex = sex; }
	void set_sex__api(const uint8_t sex) { m_sex = (CharacterSex) sex; }

	CharacterRole get_role() const { return m_character_role; }
	void set_role(const CharacterRole role) { m_character_role = role; }
	void set_role__api(const uint8_t role) { m_character_role = (CharacterRole) role; }

	virtual const ActorObject::Type get_ao_type() const
	{
		return ActorObject::TYPE_CHARACTER;
	}

	virtual Point2 get_ao_position() const
	{
		return get_pos();
	}

	virtual void step(const double &dtime);

protected:
	static void _bind_methods();
	CharacterRole m_character_role = CHARACTER_ROLE_NONE;

private:
	String m_character_name = "";

	CharacterSex m_sex = MALE;
};
