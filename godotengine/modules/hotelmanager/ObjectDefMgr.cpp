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
 *
 * All rights reserved
 */

#include "ObjectDefMgr.h"

#define GAMEDATA_PATH String("res://gamedata/")
#define ROOMDEF_FILE String("room.csv")
#define CHARACTER_FILE String("character.csv")
#define TILES_FILE String("tiles.csv")

ObjectDefMgr::ObjectDefMgr()
{

}

ObjectDefMgr::~ObjectDefMgr() {

}

void ObjectDefMgr::load_roomdefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + ROOMDEF_FILE, FileAccess::READ, &err);
	if (err != OK) {
		file = NULL;
		ERR_FAIL_COND(err!=OK);
	}
	m_roomdefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != ROOMDEF_SIZE) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}
		RoomDefPtr room(new RoomDef());
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
}

void ObjectDefMgr::load_characterdefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + CHARACTER_FILE, FileAccess::READ, &err);
	if (err != OK) {
		file = NULL;
		ERR_FAIL_COND(err!=OK);
	}
	m_characterdefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != CHARACTERDEF_SIZE) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}
		CharacterDefPtr character(new CharacterDef());
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
}

void ObjectDefMgr::load_tilesdefs()
{
	Error err;
	FileAccess *file = FileAccess::open(GAMEDATA_PATH + TILES_FILE, FileAccess::READ, &err);
	if (err != OK) {
		file = NULL;
		ERR_FAIL_COND(err!=OK);
	}
	m_game_tiledefs.clear();

	Vector<String> csv_line = file->get_csv_line();

	while (csv_line.size() > 1) {
		if (csv_line.size() != GAMETILEDEF_SIZE) {
			ERR_PRINT("invalid CSV line (gametiledef), ignoring.");
			csv_line = file->get_csv_line();
			continue;
		}
		GameTileDefPtr tiles(new GameTileDef());
		uint16_t tiles_id = (uint16_t) csv_line.get(0).to_int();

		if (m_game_tiledefs.find(tiles_id) != m_game_tiledefs.end()) {
			WARN_PRINT(String("ID "+ String::num(tiles_id) +" was already registered, overriding it").ascii().get_data());
		}

		tiles->id = (GameMapTile) tiles_id;
		tiles->type = (TileType) csv_line.get(1).to_int();
		tiles->name = csv_line.get(2).utf8();
		tiles->texture_name = csv_line.get(3).utf8();
		tiles->label = csv_line.get(4).utf8();
		tiles->flags = csv_line.get(5).to_int();

		m_game_tiledefs[tiles_id] = tiles;
		csv_line = file->get_csv_line();
	}
}
