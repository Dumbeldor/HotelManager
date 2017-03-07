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
 * 2016, Vincent Glize <vincent.glize@live.fr>
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "objectdefmgr.h"
#include "gamedatareader.h"
#include "log.h"
#include <sstream>

ObjectDefMgr *ObjectDefMgr::s_singleton = nullptr;

ObjectDefMgr::ObjectDefMgr()
{
	assert(ObjectDefMgr::s_singleton == nullptr); // Should be null here
	ObjectDefMgr::s_singleton = this;

	m_random_gen = std::mt19937(time(NULL));

	load_characterdefs();
	load_roomdefs();
	load_tilegroups();
	load_tiledefs();
	load_achievement_groups();
	load_achievements();
	load_mission_objectives();
	load_missions();
	load_names();
}

ObjectDefMgr::~ObjectDefMgr()
{
	ObjectDefMgr::s_singleton = nullptr;
	for (auto &achievement : m_achievements) {
		delete achievement.second;
	}

	for (auto &ag : m_achievement_groups) {
		delete ag.second;
	}

	for (auto &room : m_roomdefs) {
		delete room.second;
	}

	for (auto &cdef : m_characterdefs) {
		delete cdef.second;
	}

	for (auto &tdef : m_game_tiledefs) {
		delete tdef.second;
	}

	for (auto &tg : m_tilegroups) {
		delete tg.second;
	}

	for (auto &m : m_missions) {
		delete m.second;
	}

	for (auto &mo : m_mission_objectives) {
		delete mo.second;
	}
}

/**
 * Load game data for rooms
 */
void ObjectDefMgr::load_roomdefs()
{
	m_roomdefs.clear();
	GameDataReader reader("room", ROOMDEF_CSV_COLS);
	while (reader.is_good()) {
		RoomDef *room = new RoomDef();
		reader >> room->id >> room->name >> room->max_size >> room->min_size;

		if (m_roomdefs.find(room->id) != m_roomdefs.end()) {
			LOG_WARN("ID %d was already registered, overriding it", room->id);
			delete m_characterdefs[room->id];
		}

		m_roomdefs[room->id] = room;
	}
}

/**
 * Load game data for characters
 */
void ObjectDefMgr::load_characterdefs()
{
	m_characterdefs.clear();
	GameDataReader reader("character", CHARACTERDEF_CSV_COLS);
	while (reader.is_good()) {
		CharacterDef *character = new CharacterDef();
		uint16_t role;
		reader >> character->id >> character->name >> character->sex_chance >> role >>
		    character->cost >> character->tooltip >> character->icon >> character->scene;

		if (character->sex_chance >= 100.0f || character->sex_chance < 0.0f) {
			LOG_CRIT("Invalid sex_chance %f for ID %d, setting to 50%%.",
				 character->sex_chance, character->id);
			character->sex_chance = 50.0f;
		}

		if (role >= CHARACTER_ROLE_MAX) {
			LOG_CRIT("Invalid role %d for ID %d, ignoring.", role, character->id);
			delete character;
			continue;
		}

		character->role = (CharacterRole) role;

		if (m_characterdefs.find(character->id) != m_characterdefs.end()) {
			LOG_WARN("ID %d was already registered, overriding it", character->id);
			delete m_characterdefs[character->id];
		}

		m_characterdefs[character->id] = character;
		m_characterdefs_per_role[character->role] = character;
	}
}

void ObjectDefMgr::load_tilegroups()
{
	m_tilegroups.clear();
	GameDataReader reader("tilegroups", TILEGROUPS_CSV_COLS);
	while (reader.is_good()) {
		TileGroup *tg = new TileGroup();
		reader >> tg->id >> tg->name;
		if (m_tilegroups.find(tg->id) != m_tilegroups.end()) {
			LOG_WARN("ID %d was already registered, overriding it", tg->id);
			delete m_tilegroups[tg->id];
		}

		m_tilegroups[tg->id] = tg;

		if (m_tilegroups_per_name.find(tg->name) != m_tilegroups_per_name.end()) {
			LOG_WARN("Name %s was already registered, overriding it", tg->name.c_str());
			delete m_tilegroups_per_name[tg->name];
		}
		m_tilegroups_per_name[tg->name] = tg;
	}
}

/**
 * Load game data for tiles
 */
void ObjectDefMgr::load_tiledefs()
{
	m_game_tiledefs.clear();
	GameDataReader reader("tiles", TILEDEF_CSV_COLS);
	while (reader.is_good()) {
		TileDef *tiledef = new TileDef();
		uint32_t tile_id;

		reader >> tile_id;
		if (tile_id >= TILE_MAX) {
			LOG_CRIT("Tile ID %d was invalid, ignoring", tile_id);
			delete tiledef;
			continue;
		}

		tiledef->id = tile_id;
		Vector<uint32_t> groups;
		reader >> groups;
		for (uint32_t i = 0; i < groups.size(); i++) {
			uint32_t group_id = groups[i];
			if (m_tilegroups.find(group_id) == m_tilegroups.end()) {
				LOG_WARN("Invalid group id %d for tile %d, ignoring.", group_id,
					 tiledef->id);
				continue;
			}
			tiledef->groups.push_back(group_id);
		}

		int32_t tile_flags;
		reader >> tiledef->name >> tiledef->texture_name >> tiledef->label >> tile_flags >>
		    tiledef->cost;

		if (tile_flags >= TILE_FLAG_MAX) {
			LOG_CRIT("ID %d invalid tile flags (%d), ignoring", tile_id, tile_flags);
			delete tiledef;
			continue;
		}

		tiledef->flags = tile_flags;

		if (m_game_tiledefs.find(tile_id) != m_game_tiledefs.end()) {
			LOG_WARN("ID %d was already registered, overriding it", tile_id);
			delete m_game_tiledefs[tiledef->id];
		}

		m_game_tiledefs[tiledef->id] = tiledef;
	}
}

/**
 * Load achievement groups
 */
void ObjectDefMgr::load_achievement_groups()
{
	m_achievement_groups.clear();
	GameDataReader reader("achievementgroups", ACHIEVEMENTGROUPS_CSV_COLS);
	while (reader.is_good()) {
		AchievementGroup *ag = new AchievementGroup();
		reader >> ag->id >> ag->title;
		if (m_achievement_groups.find(ag->id) != m_achievement_groups.end()) {
			LOG_WARN("AchievementGroup %d was already registered, overriding it",
				 ag->id);
			delete m_achievement_groups[ag->id];
		}
		m_achievement_groups[ag->id] = ag;
	}
}

/**
 * Load game data for achievements
 */
void ObjectDefMgr::load_achievements()
{
	m_achievements.clear();
	GameDataReader reader("achievements", ACHIEVEMENTS_CSV_COLS);
	while (reader.is_good()) {
		Achievement *achievement = new Achievement();

		uint32_t achievement_type;
		reader >> achievement->unique_id >> achievement_type;

		if (achievement_type == Achievement::Type::NONE ||
		    achievement_type >= Achievement::Type::MAX) {
			LOG_WARN("Invalid achievement type %d, ignoring.", achievement_type);
			delete achievement;
			continue;
		}
		achievement->type = (Achievement::Type) achievement_type;

		reader >> achievement->title >> achievement->objective >>
		    achievement->description >> achievement->icon >> achievement->group_id;

		if (m_achievement_groups.find(achievement->group_id) ==
		    m_achievement_groups.end()) {
			LOG_WARN("Achievement %s has invalid group id %d, resetting to 0",
				 achievement->title.c_str(), achievement->group_id);
			achievement->group_id = 0;
		}

		m_achievements.insert(
		    std::pair<Achievement::Type, Achievement *>(achievement->type, achievement));
	}
}

void ObjectDefMgr::load_mission_objectives()
{
	m_mission_objectives.clear();
	GameDataReader reader("missionobjectives", MISSIONOBJECTIVES_CSV_COLS);
	while (reader.is_good()) {
		MissionObjective *mission_objective = new MissionObjective();
		uint16_t type;
		reader >> mission_objective->id >> mission_objective->title >> type >>
		    mission_objective->count;

		if (type >= MissionObjective::Type::MAX) {
			LOG_WARN("Invalid missionobjective type %d, ignoring.", type);
			delete mission_objective;
			continue;
		}

		mission_objective->type = (MissionObjective::Type) type;

		if (m_mission_objectives.find(mission_objective->id) !=
		    m_mission_objectives.end()) {
			LOG_WARN("MissionObjective %d was already registered, overriding it",
				 mission_objective->id);
			delete m_achievement_groups[mission_objective->id];
		}

		m_mission_objectives[mission_objective->id] = mission_objective;
	}
}

void ObjectDefMgr::load_missions()
{
	m_missions.clear();
	GameDataReader reader("missions", MISSIONS_CSV_COLS);
	while (reader.is_good()) {
		Mission *mission = new Mission();

		std::vector<uint32_t> objectives = {};
		reader >> mission->id >> mission->title >> mission->description >>
		    mission->parents >> objectives;

		for (const auto &o : objectives) {
			const auto &mo = m_mission_objectives.find(o);
			if (mo == m_mission_objectives.end()) {
				LOG_WARN("MissionObjective %d doesn't exists. It will not be added "
					 "to mission %d",
					 o, mission->id);
				continue;
			}

			mission->objectives.push_back(mo->second);
		}

		m_missions[mission->id] = mission;
	}
}

#define NAMES_CSV_COLS 2
void ObjectDefMgr::load_names()
{
	m_female_names.clear();
	m_male_names.clear();
	GameDataReader reader("names", NAMES_CSV_COLS);
	while (reader.is_good()) {
		std::string female_name;
		std::string male_name;
		reader >> female_name >> male_name;
		m_female_names.push_back(female_name);
		m_male_names.push_back(male_name);
	}
}

/**
 * Returns the tiledef associated with t
 * \attention tile id should be a real tile id, else an assertion is triggered
 *
 * @param t tiledef_id
 * @return GameTileDef for tiledef_id
 */
const TileDef &ObjectDefMgr::get_tiledef(uint32_t t) const
{
	const auto &td = m_game_tiledefs.find(t);
	if (td == m_game_tiledefs.end()) {
		LOG_CRIT("Trying to get tiledef %d, but it's not defined, aborting.", t);
		assert(false);
	}
	return *td->second;
}

static const TileGroup null_tilegroup = {};
const TileGroup &ObjectDefMgr::get_tilegroup(const uint32_t gid) const
{
	const auto &tg = m_tilegroups.find(gid);
	if (tg == m_tilegroups.end()) {
		return null_tilegroup;
	}
	return *tg->second;
}

const TileGroup &ObjectDefMgr::get_tilegroup(const std::string &g) const
{
	const auto &tg = m_tilegroups_per_name.find(g);
	if (tg == m_tilegroups_per_name.end()) {
		return null_tilegroup;
	}
	return *tg->second;
}

const Mission &ObjectDefMgr::get_mission(const uint32_t id) const
{
	const auto &m = m_missions.find(id);
	if (m == m_missions.end()) {
		static const Mission null_mission = {};
		return null_mission;
	}
	return *m->second;
}

static const CharacterDef null_def = {};
const CharacterDef &ObjectDefMgr::get_characterdef(const uint16_t id) const
{
	const auto &m = m_characterdefs.find(id);
	if (m == m_characterdefs.end()) {
		return null_def;
	}
	return *m->second;
}

const CharacterDef &ObjectDefMgr::get_characterdef_by_role(const CharacterRole id) const
{
	const auto &m = m_characterdefs_per_role.find(id);
	if (m == m_characterdefs_per_role.end()) {
		static const CharacterDef null_def = {};
		return null_def;
	}
	return *m->second;
}

String ObjectDefMgr::get_random_female_name()
{
	if (m_female_names.size() == 0) {
		return String("null");
	}

	std::uniform_int_distribution<uint32_t> rd_name(0, (uint32_t)(m_female_names.size() - 1));
	return String(m_female_names[rd_name(m_random_gen)].c_str());
}

String ObjectDefMgr::get_random_male_name()
{
	if (m_male_names.size() == 0) {
		return String("null");
	}

	std::uniform_int_distribution<uint32_t> rd_name(0, (uint32_t)(m_male_names.size() - 1));
	return String(m_male_names[rd_name(m_random_gen)].c_str());
}

CharacterSex ObjectDefMgr::get_random_sex()
{
	static std::uniform_int_distribution<uint8_t> rd_sex(MALE, FEMALE);
	return (CharacterSex) rd_sex(m_random_gen);
}
