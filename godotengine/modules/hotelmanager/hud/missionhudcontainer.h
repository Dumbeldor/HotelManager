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

#pragma once

#include <scene/gui/box_container.h>

class RichTextLabel;
struct Mission;
struct MissionObjective;

class MissionHudContainer : public VBoxContainer
{
	OBJ_TYPE(MissionHudContainer, VBoxContainer);
public:
	MissionHudContainer(const Mission &mission);
	~MissionHudContainer() {}

	void update_mission_objective(const MissionObjective &objective_def, const uint32_t count);
protected:

	static void _bind_methods() {};
	MissionHudContainer(): VBoxContainer() {}
private:
	RichTextLabel *m_mission_title = nullptr;
	RichTextLabel *m_mission_desc = nullptr;
};
