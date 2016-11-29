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
 *
 * All rights reserved
 */
#pragma once
#include <iostream>
#include <unordered_map>
#include "core/os/file_access.h"
#include <memory>
#include "character.h"
#include "game_tiles.h"

struct RoomDef
{
	uint16_t id;
	String name;
	uint8_t min_size;
	uint8_t max_size;
};
static const uint8_t ROOMDEF_SIZE = 4;

typedef std::shared_ptr<RoomDef> RoomDefPtr;

class ObjectDefMgr
{
public:
	ObjectDefMgr();
	~ObjectDefMgr();
	void load_roomdefs();
	void load_characterdefs();
	void load_tilesdefs();

	static const GameTileDef &get_tiledef(GameMapTile t)
	{
		return s_singleton->get_tiledef_priv(t);
	}

private:
	const GameTileDef &get_tiledef_priv(GameMapTile t);
	// Singleton
	static ObjectDefMgr *s_singleton;

	std::unordered_map<uint16_t, RoomDefPtr> m_roomdefs;
	std::unordered_map<uint16_t, CharacterDefPtr> m_characterdefs;
	std::unordered_map<GameMapTile, GameTileDefPtr> m_game_tiledefs;
};
