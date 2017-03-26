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
 * 2016-2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "character.h"
#include "../objectdefmgr.h"
#include <iostream>
#include <scene/2d/navigation2d.h>

Navigation2D *Character::m_navigator = nullptr;

Character::Character(CharacterRole role) : RigidBody2D(), ActorObject(), m_character_role(role)
{
	// Ignore this part in editor
	if (!ObjectDefMgr::get_singleton()) {
		return;
	}
	m_sex = ObjectDefMgr::get_singleton()->get_random_sex();
	if (m_sex == MALE) {
		m_character_name = ObjectDefMgr::get_singleton()->get_random_male_name();
	} else if (m_sex == FEMALE) {
		m_character_name = ObjectDefMgr::get_singleton()->get_random_female_name();
	}

	set_gravity_scale(0.0f);
	set_mode(Mode::MODE_CHARACTER);
	set_fixed_process(true);
}

Character::~Character() {}

void Character::_bind_methods()
{
	ObjectTypeDB::bind_method("set_sex", &Character::set_sex__api);
	ObjectTypeDB::bind_method("set_role", &Character::set_role__api);
}

void Character::step(const double &dtime)
{
	m_action_wanted = CAW_GOTO_POINT;
	m_action_pos = Vector2(240, 445);
	switch (m_action_wanted) {
		case CAW_GOTO_POINT:
			action_move_to_tile();
			break;
		case CAW_WAIT_AROUND_POINT:
			// @TODO
			break;
		case CAW_NOTHING:
		default:
			break;
	};
}

static constexpr float distance_per_move = 1.5f;
void Character::action_move_to_tile()
{
	assert(Character::m_navigator);
	m_speed = 100.0f;
	std::cout << get_global_pos().x << "/" << get_global_pos().y << " mouse " << get_global_mouse_pos().x << " / " << get_global_mouse_pos().y  << std::endl;
	Vector<Vector2> points = m_navigator->get_simple_path(get_global_pos(), get_global_mouse_pos(), false);
	if (points.size() > 0) {
		Point2 distance = (points[0] - get_global_pos());
		Vector2 direction = distance.normalized();

		std::cout << m_character_name.ascii().get_data() << " wants to move. Distance: " << distance.length() << " points " << points.size() << std::endl;
		if (distance.length() > distance_per_move && points.size() >= 2) {
			std::cout << m_character_name.ascii().get_data() << " move" << std::endl;
			set_linear_velocity(direction * m_speed);
			return;
		}
	}

	set_linear_velocity(Vector2(0, 0));
}
