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

#include "missionhudcontainer.h"
#include <scene/gui/label.h>
#include <scene/gui/rich_text_label.h>
#include <math/math_2d.h>
#include "../missions.h"

MissionHudContainer::MissionHudContainer(const Mission &mission): VBoxContainer()
{
	m_mission_title = memnew(RichTextLabel);
	m_mission_title->set_use_bbcode(true);
	m_mission_title->set_bbcode(String("[b]") + String(mission.title.c_str()) + String("[/b]"));
	m_mission_title->set_name("mission_" + String::num(mission.id));
	m_mission_title->set_custom_minimum_size(Size2(200, 15));

	add_child(m_mission_title);

	m_mission_desc = memnew(RichTextLabel);
	m_mission_desc->set_use_bbcode(true);
	m_mission_desc->set_bbcode(String("[i]") + String(mission.description.c_str()) +
		String("[/i]"));
	m_mission_desc->set_name("mission_" + String::num(mission.id) + "_desc");
	m_mission_desc->set_custom_minimum_size(Size2(200, 15));
	add_child(m_mission_desc);

	VBoxContainer *objectives_container = memnew(VBoxContainer);
	add_child(objectives_container);

	for (const auto &o : mission.objectives) {
		Label *objective = memnew(Label);
		objective->set_text(String("- ") + o->title.c_str() +
			" (0/" + String::num(o->count) + ")");
		objective->set_name("mission_obj_" + String::num(o->id));
		objectives_container->set_margin(MARGIN_LEFT, 20);
		objectives_container->add_child(objective);
	}
}

void MissionHudContainer::update_mission_objective(const MissionObjective &objective_def,
	const uint32_t count)
{
	Node *node = find_node("mission_obj_" + String::num(objective_def.id),
		true, false);
	assert(node);
	Label *objective_label = node->cast_to<Label>();
	assert(objective_label);
	objective_label->set_text(String("- ") + objective_def.title.c_str() +
		" (" + String::num(count) + "/" + String::num(objective_def.count) + ")");
}
