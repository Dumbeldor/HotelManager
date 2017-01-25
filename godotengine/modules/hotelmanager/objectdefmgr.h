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
#include <unordered_map>
#include <core/os/file_access.h>
#include "achievements.h"
#include "character.h"
#include "tiles.h"
#include "missions.h"

struct RoomDef
{
	uint16_t id;
	std::string name;
	uint8_t min_size;
	uint8_t max_size;
};
static const uint8_t ROOMDEF_CSV_COLS = 4;

class ObjectDefMgr
{
public:
	ObjectDefMgr();
	~ObjectDefMgr();

	/**
	 *
	 * @param tile_id
	 * @return const reference of the tiledef
	 */
	static const GameTileDef &get_tiledef(const GameMapTile tile_id)
	{
		return s_singleton->get_tiledef_priv(tile_id);
	}

	static const TileGroup &get_tilegroup(const uint32_t gid)
	{
		return s_singleton->get_tilegroup_priv(gid);
	}

	static const TileGroup &get_tilegroup(const std::string &g)
	{
		return s_singleton->get_tilegroup_priv(g);
	}

	static void delete_singleton()
	{
		delete s_singleton;
		s_singleton = nullptr;
	}

private:
	void load_roomdefs();
	void load_characterdefs();
	void load_tilegroups();
	void load_tiledefs();
	void load_achievement_groups();
	void load_achievements();
	void load_mission_objectives();
	void load_missions();

	const GameTileDef &get_tiledef_priv(GameMapTile t);
	const TileGroup &get_tilegroup_priv(const uint32_t gid);
	const TileGroup &get_tilegroup_priv(const std::string &g);
	// Singleton
	static ObjectDefMgr *s_singleton;

	std::unordered_map<uint16_t, RoomDef *> m_roomdefs = {};
	std::unordered_map<uint16_t, CharacterDef *> m_characterdefs = {};
	std::unordered_map<GameMapTile, GameTileDef *> m_game_tiledefs = {};
	std::unordered_map<uint32_t, TileGroup *> m_tilegroups = {};
	std::unordered_map<std::string, TileGroup *> m_tilegroups_per_name = {};
	AchievementList m_achievements = {};
	AchievementGroupList m_achievement_groups = {};
	MissionMap m_missions = {};
	MissionObjectiveMap m_mission_objectives = {};
};
