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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include <cstdint>

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
	TILE_TYPE_GENERIC,
	TILE_TYPE_GROUND,
	TILE_TYPE_WALL,
	TILE_TYPE_GAMEOBJECT,
};

enum TileFlags
{
	TILE_FLAG_NONE = 0x00,
	TILE_FLAG_UNAVAILABLE_FOR_PLAYERS = 0x01,
};

struct GameTileDef
{
	GameMapTile id;
	TileType type;
	const char* name;
	const char* texture_name;
	const char* label;
	int flags;
};

// Order is very important here
static const GameTileDef game_tile_defs[TILE_MAX] = {
	{TILE_NONE, TILE_TYPE_GENERIC, "none", "tile_empty.png", "Empty", TILE_FLAG_UNAVAILABLE_FOR_PLAYERS},

	{TILE_GROUND_GRASS,	TILE_TYPE_GROUND, "grass", "tile_grass.png", "Grass", TILE_FLAG_NONE},
	{TILE_GROUND_STONE, TILE_TYPE_GROUND, "stone", "tile_stone.png", "Stone", TILE_FLAG_NONE},
	{TILE_GROUND_FOREST, TILE_TYPE_GROUND, "forest", "tile_forest.png",	"Forest", TILE_FLAG_NONE},
	{TILE_GROUND_SEAWATER, TILE_TYPE_GROUND, "seawater", "tile_seawater.png", "Sea water", TILE_FLAG_NONE},
	{TILE_GROUND_SWIMMINGPOOLWATER,	TILE_TYPE_GROUND, "swimmingpoolwater", "tile_swimmingpoolwater.png", "Swimming pool", TILE_FLAG_NONE},
	{TILE_GROUND_FLOORTILE,	TILE_TYPE_GROUND, "floortile", "tile_floortile.png", "Floor tile", TILE_FLAG_NONE},
	{TILE_GROUND_LINOLEUM, TILE_TYPE_GROUND, "linoleum", "tile_linoleum.png", "Linoleum", TILE_FLAG_NONE},
	{TILE_GROUND_MOSAIC, TILE_TYPE_GROUND, "mosaic", "tile_mosaic.png", "Mosaic", TILE_FLAG_NONE},
	{TILE_GROUND_PARQUET, TILE_TYPE_GROUND, "parquet", "tile_parquet.png", "Parquet", TILE_FLAG_NONE},
	{TILE_GROUND_CARPET, TILE_TYPE_GROUND, "carpet", "tile_carpet.png", "Carpet", TILE_FLAG_NONE},
};
