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

#include <cstdint>
#include <vector>
#include <string>
#include <bits/unordered_map.h>

enum MissionObjectiveType
{
	MISSION_OBJECTIVE_TYPE_NONE,
	MISSION_OBJECTIVE_TYPE_HIRE,
	MISSION_OBJECTIVE_TYPE_MAX,
};

struct MissionObjective
{
	uint32_t id = 0;
	std::string title = "";
	MissionObjectiveType type = MISSION_OBJECTIVE_TYPE_NONE;
	uint32_t count = 0;
};

typedef std::unordered_map<uint32_t, MissionObjective *> MissionObjectiveMap;
#define MISSIONOBJECTIVES_CSV_COLS 4

struct Mission
{
	uint32_t id = 0;
	std::vector<uint32_t> parents = {};
	std::string title = "";
	std::string description = "";
	std::vector<MissionObjective *> objectives = {};
};

typedef std::unordered_map<uint32_t, Mission *> MissionMap;
#define MISSIONS_CSV_COLS 5
