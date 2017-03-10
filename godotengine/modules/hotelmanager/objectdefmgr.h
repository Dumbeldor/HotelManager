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
 * 2016, Vincent Glize <vincent.glize@live.fr>
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */
#pragma once
#include "achievements.h"
#include "missions.h"
#include "modules/hotelmanager/character/character.h"
#include "tiles.h"
#include <core/os/file_access.h>
#include <random>
#include <unordered_map>

struct RoomDef
{
	uint16_t id = 0;
	std::string name = "";
	uint8_t min_size = 1;
	uint8_t max_size = 1;
};
static const uint8_t ROOMDEF_CSV_COLS = 4;
typedef std::unordered_map<uint32_t, TileDef *> TileDefMap;

class ObjectDefMgr
{
public:
	ObjectDefMgr();
	~ObjectDefMgr();

	static ObjectDefMgr *get_singleton() { return s_singleton; }

	/**
	 *
	 * @param tile_id
	 * @return const reference of the tiledef
	 */
	const TileDef &get_tiledef(uint32_t t) const;
	const std::unordered_map<uint32_t, TileDef *> &get_tiledefs() const
	{
		return m_game_tiledefs;
	}

	const std::unordered_map<CharacterRole, CharacterDef *> &get_characterdefs() const
	{
		return m_characterdefs_per_role;
	};

	const TileGroup &get_tilegroup(const uint32_t gid) const;
	const TileGroup &get_tilegroup(const std::string &g) const;
	const Mission &get_mission(const uint32_t id) const;
	const MissionObjective &get_mission_objective(const uint32_t id) const;
	const CharacterDef &get_characterdef(const uint16_t id) const;
	const CharacterDef &get_characterdef_by_role(const CharacterRole id) const;

	CharacterSex get_random_sex();
	String get_random_male_name();
	String get_random_female_name();

private:
	void load_roomdefs();
	void load_characterdefs();
	void load_tilegroups();
	void load_tiledefs();
	void load_achievement_groups();
	void load_achievements();
	void load_mission_objectives();
	void load_missions();
	void load_names();

	// Singleton
	static ObjectDefMgr *s_singleton;

	std::unordered_map<uint16_t, RoomDef *> m_roomdefs = {};
	std::unordered_map<uint16_t, CharacterDef *> m_characterdefs = {};
	std::unordered_map<CharacterRole, CharacterDef *> m_characterdefs_per_role = {};
	TileDefMap m_game_tiledefs = {};
	std::unordered_map<uint32_t, TileGroup *> m_tilegroups = {};
	std::unordered_map<std::string, TileGroup *> m_tilegroups_per_name = {};
	AchievementList m_achievements = {};
	AchievementGroupList m_achievement_groups = {};
	MissionMap m_missions = {};
	MissionObjectiveMap m_mission_objectives = {};

	std::vector<std::string> m_female_names = {};
	std::vector<std::string> m_male_names = {};

	std::mt19937 m_random_gen;
};
