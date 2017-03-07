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
#include <unordered_map>

struct AchievementGroup
{
	uint32_t id = 0;
	std::string title = "";
};

static const uint8_t ACHIEVEMENTGROUPS_CSV_COLS = 2;
typedef std::unordered_map<uint32_t, AchievementGroup *> AchievementGroupList;

struct Achievement
{
	enum Type : uint8_t
	{
		NONE = 0,
		MONEY_NUMBER,
		CLIENT_NUMBER,
		DAY_NUMBER,
		MAX,
	};

	Achievement() {}
	uint32_t unique_id = 0;
	Achievement::Type type = Achievement::Type::NONE;
	uint32_t objective = 0;
	uint32_t group_id = 0;
	std::string title = "";
	std::string description = "";
	std::string icon = "";
};

struct AchievementProgress
{
	uint32_t unique_id = 0;
	uint32_t progress = 0;
	bool done = false;
};

static const uint8_t ACHIEVEMENTS_CSV_COLS = 7;
typedef std::unordered_multimap<Achievement::Type, Achievement *> AchievementList;
typedef std::unordered_map<uint32_t, AchievementProgress> AchievementProgressMap;
