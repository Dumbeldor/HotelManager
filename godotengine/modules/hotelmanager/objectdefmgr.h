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
#include <iostream>
#include <unordered_map>
#include "core/os/file_access.h"
#include "achievements.h"
#include "character.h"
#include "game_tiles.h"

struct RoomDef
{
	uint16_t id;
	String name;
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

	static void delete_singleton()
	{
		delete s_singleton;
		s_singleton = nullptr;
	}

private:
	void load_roomdefs();
	void load_characterdefs();
	void load_tiledefs();
	void load_achievements();

	const GameTileDef &get_tiledef_priv(GameMapTile t);
	// Singleton
	static ObjectDefMgr *s_singleton;

	std::unordered_map<uint16_t, RoomDef *> m_roomdefs;
	std::unordered_map<uint16_t, CharacterDef *> m_characterdefs;
	std::unordered_map<GameMapTile, GameTileDef *> m_game_tiledefs;
	AchievementList m_achievements;
};
