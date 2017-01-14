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

#define GAMEDATA_PATH String("res://gamedata/")
#define ROOMDEF_FILE String("room.csv")
#define CHARACTER_FILE String("character.csv")
#define TILES_FILE String("tiles.csv")
#define ACHIEVEMENTS_FILE String("achievements.csv")

ObjectDefMgr *ObjectDefMgr::s_singleton = nullptr;

ObjectDefMgr::ObjectDefMgr()
{
	assert(ObjectDefMgr::s_singleton == nullptr); // Should be null here
	ObjectDefMgr::s_singleton = this;

	load_characterdefs();
	load_roomdefs();
	load_tiledefs();
	load_achievements();
}

ObjectDefMgr::~ObjectDefMgr()
{
	ObjectDefMgr::s_singleton = nullptr;
	for (auto &achievement: m_achievements) {
		delete achievement.second;
	}

	for (auto &room: m_roomdefs) {
		delete room.second;
	}

	for (auto &cdef: m_characterdefs) {
		delete cdef.second;
	}

	for (auto &tdef: m_game_tiledefs) {
		delete tdef.second;
	}
}

/**
 * Load game data for rooms
 */
void ObjectDefMgr::load_roomdefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + ROOMDEF_FILE, FileAccess::READ, &err);
	if (err != OK || file == NULL) {
		ERR_PRINT("No roomdefs game data, ignoring.");
		return;
	}

	m_roomdefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != ROOMDEF_CSV_COLS) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}
		RoomDef *room = new RoomDef();
		uint16_t room_id = (uint16_t) csv_line.get(0).to_int();

		if (m_roomdefs.find(room_id) != m_roomdefs.end()) {
			WARN_PRINT(String("ID "+ String::num(room_id) +" was already registered, overriding it").ascii().get_data());
		}

		room->id = room_id;
		room->name = csv_line.get(1).utf8();
		room->max_size = (uint8_t) csv_line.get(2).to_int();
		room->min_size = (uint8_t) csv_line.get(3).to_int();

		m_roomdefs[room_id] = room;
		csv_line = file->get_csv_line();
	}

	file->close();
}

/**
 * Load game data for characters
 */
void ObjectDefMgr::load_characterdefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + CHARACTER_FILE, FileAccess::READ, &err);
	if (err != OK || file == NULL) {
		ERR_PRINT("No characterdefs game data, ignoring.");
		return;
	}

	m_characterdefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != CHARACTERDEF_CSV_COLS) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}
		CharacterDef *character = new CharacterDef();
		uint16_t character_id = (uint16_t) csv_line.get(0).to_int();

		if (m_characterdefs.find(character_id) != m_characterdefs.end()) {
			WARN_PRINT(String("ID "+ String::num(character_id) +" was already registered, overriding it").ascii().get_data());
		}

		character->id = character_id;
		character->name = csv_line.get(1).utf8();
		character->sex = (CharacterSex) csv_line.get(2).to_int();
		character->role = (CharacterRole) csv_line.get(3).to_int();

		m_characterdefs[character_id] = character;
		csv_line = file->get_csv_line();
	}

	file->close();
}

/**
 * Load game data for tiles
 */
void ObjectDefMgr::load_tiledefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + TILES_FILE, FileAccess::READ, &err);
	if (err != OK || file == NULL) {
		ERR_PRINT("No tiledefs game data, ignoring.");
		return;
	}

	m_game_tiledefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != GAMETILEDEF_CSV_COLS) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}

		GameTileDef *tiledef = new GameTileDef();
		uint16_t tile_id = (uint16_t) csv_line.get(0).to_int();

		if (m_game_tiledefs.find((GameMapTile) tile_id) != m_game_tiledefs.end()) {
			WARN_PRINT(String("ID " + String::num(tile_id) + " was already registered, overriding it").ascii().get_data());
		}

		if (tile_id >= TILE_MAX) {
			WARN_PRINT(String("Tile ID " + String::num(tile_id) + " was invalid, ignoring").ascii().get_data());
			csv_line = file->get_csv_line();
			continue;
		}

		uint16_t tile_type = (uint16_t) csv_line.get(1).to_int();
		if (tile_type >= TILE_TYPE_MAX) {
			WARN_PRINT(String("ID " + String::num(tile_id) + " invalid tile type, ignoring").ascii().get_data());
			csv_line = file->get_csv_line();
			continue;
		}

		int tile_flags = csv_line.get(5).to_int();
		if (tile_flags >= TILE_FLAG_MAX) {
			WARN_PRINT(String("ID " + String::num(tile_id) + " invalid tile flags ("
				+ String::num(tile_flags) + "), ignoring").ascii().get_data());
			csv_line = file->get_csv_line();
			continue;
		}

		tiledef->id = (GameMapTile) tile_id;
		tiledef->type = (TileType) tile_type;
		tiledef->name = csv_line.get(2).utf8();
		tiledef->texture_name = csv_line.get(3).utf8();
		tiledef->label = csv_line.get(4).utf8();
		tiledef->flags = tile_flags;
		tiledef->cost = (uint32_t) csv_line.get(6).to_int();

		m_game_tiledefs[tiledef->id] = tiledef;
		csv_line = file->get_csv_line();
	}

	file->close();
}

/**
 * Load game data for achievements
 */
void ObjectDefMgr::load_achievements()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + ACHIEVEMENTS_FILE, FileAccess::READ, &err);
	if (err != OK || file == NULL) {
		ERR_PRINT("No achievements game data, ignoring.");
		return;
	}

	m_achievements.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != ACHIEVEMENTS_CSV_COLS) {
			ERR_PRINT("invalid CSV line (achievements), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}

		Achievement *achievement = new Achievement();
		achievement->unique_id = (uint32_t) csv_line.get(0).to_int();
		achievement->type = (AchievementType) csv_line.get(1).to_int();
		if (achievement->type == ACHIEVEMENT_TYPE_NONE
			|| achievement->type >= ACHIEVEMENT_TYPE_MAX) {
			ERR_PRINT("Invalid achievement type, ignoring.")
			continue;
		}
		achievement->title = csv_line.get(2).utf8();
		achievement->objective = (uint32_t) csv_line.get(3).to_int();
		achievement->description = csv_line.get(4).utf8();
		achievement->icon = csv_line.get(5).utf8();

		m_achievements.insert(std::pair<AchievementType, Achievement *>(achievement->type, achievement));
		csv_line = file->get_csv_line();
	}

	file->close();
}

/**
 * Returns the tiledef associated with t
 * \attention tile id should be a real tile id, else an assertion is triggered
 *
 * @param t tiledef_id
 * @return GameTileDef for tiledef_id
 */
const GameTileDef &ObjectDefMgr::get_tiledef_priv(GameMapTile t)
{
	assert(t < TILE_MAX);
	return *m_game_tiledefs[t];
}
