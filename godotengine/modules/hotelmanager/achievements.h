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
#include <memory>

enum AchievementType
{
	ACHIEVEMENT_TYPE_NONE = 0,
	ACHIEVEMENT_TYPE_MONEY_NUMBER,
	ACHIEVEMENT_TYPE_CLIENT_NUMBER,
	ACHIEVEMENT_TYPE_DAY_NUMBER,
	ACHIEVEMENT_TYPE_MAX,
};

struct Achievement
{
	uint32_t unique_id = 0;
	AchievementType type = ACHIEVEMENT_TYPE_NONE;
	uint32_t objective = 0;
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

static const uint8_t ACHIEVEMENTS_CSV_COLS = 6;
typedef std::shared_ptr<Achievement> AchievementPtr;
typedef std::unordered_multimap<AchievementType, AchievementPtr> AchievementList;
typedef std::unordered_map<uint32_t, AchievementProgress> AchievementProgressMap;
