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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include <cstdint>
#include <memory>
#include <ustring.h>

static constexpr uint16_t GAME_TILE_SIZE = 48;

enum GameMapTile
{
	TILE_NONE = 0,
	TILE_GROUND_GRASS,
	TILE_GROUND_STONE,
	TILE_GROUND_FOREST,
	TILE_GROUND_SEAWATER,
	TILE_GROUND_SWIMMINGPOOLWATER,
	TILE_GROUND_FLOORTILE,
	TILE_GROUND_LINOLEUM,
	TILE_GROUND_MOSAIC,
	TILE_GROUND_PARQUET,
	TILE_GROUND_CARPET,
	TILE_MAX,
};

enum TileType
{
	TILE_TYPE_GENERIC = 1,
	TILE_TYPE_GROUND = 2,
	TILE_TYPE_FLOOR = 3,
	TILE_TYPE_MAX,
};

enum TileFlags
{
	TILE_FLAG_NONE = 0x00,
	TILE_FLAG_UNAVAILABLE_FOR_PLAYERS = 0x01,
	TILE_FLAG_MAX = 0x02,
};

struct GameTileDef
{
	GameMapTile id;
	TileType type;
	String name;
	String texture_name;
	String label;
	int flags;
};
static const uint8_t GAMETILEDEF_SIZE = 6;
typedef std::shared_ptr<GameTileDef> GameTileDefPtr;
