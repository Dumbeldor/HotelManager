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


enum AchievementType
{
	ACHIEVEMENT_TYPE_NONE = 0,
	ACHIEVEMENT_TYPE_MONEY_NUMBER,
	ACHIEVEMENT_TYPE_CLIENT_NUMBER,
	ACHIEVEMENT_TYPE_DAY_NUMBER,
	ACHIEVEMENT_TYPE_MAX,
};

struct AchievementGroup
{
	uint32_t unique_id = 0;
	std::string title = "";
};

static const uint8_t ACHIEVEMENTGROUPS_CSV_COLS = 2;
typedef std::unordered_map<uint32_t, AchievementGroup *> AchievementGroupList;

struct Achievement
{
	uint32_t unique_id = 0;
	AchievementType type = ACHIEVEMENT_TYPE_NONE;
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
typedef std::unordered_multimap<AchievementType, Achievement *> AchievementList;
typedef std::unordered_map<uint32_t, AchievementProgress> AchievementProgressMap;
