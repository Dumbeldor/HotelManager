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
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct MissionObjective
{
	enum Type : uint8_t
	{
		NONE,
		HIRE,
		PLACE_TILE,
		MAX,
	};

	MissionObjective() {}
	uint32_t id = 0;
	std::string title = "";
	MissionObjective::Type type = MissionObjective::Type::NONE;
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

struct MissionObjectiveProgress
{
	uint32_t id = 0;
	uint32_t progress = 0;
	bool done = false;
};

typedef std::unordered_map<uint32_t, MissionObjectiveProgress> MissionObjectiveProgressMap;

enum MissionState : uint8_t
{
	MISSION_STATE_NOT_STARTED,
	MISSION_STATE_IN_PROGRESS,
	MISSION_STATE_DONE,
	MISSION_STATE_MAX,
};

struct MissionProgress
{
	uint32_t id = 0;
	MissionState state = MISSION_STATE_NOT_STARTED;
	MissionObjectiveProgressMap objectives_progress = {};
};

typedef std::shared_ptr<MissionProgress> MissionProgressPtr;
typedef std::unordered_map<uint32_t, MissionProgressPtr> MissionProgressMap;
