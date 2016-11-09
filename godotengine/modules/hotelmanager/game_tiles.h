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

static constexpr uint16_t FLOOR_TILE_SIZE = 96;
enum FloorTile
{
	TILE_NONE = 0,
	TILE_GRASS,
	TILE_STONE,
	TILE_FOREST,
	TILE_SEAWATER,
	TILE_SWIMMINGPOOLWATER,
	TILE_FLOORTILE,
	TILE_LINOLEUM,
	TILE_MOSAIC,
	TILE_PARQUET,
	TILE_CARPET,
	TILE_MAX,
};

struct FloorTileDef
{
	FloorTile id;
	const char* name;
	const char* texture_name;
	const char* label;
};

static const FloorTileDef floor_tile_defs[TILE_MAX] = {
	{TILE_NONE, 				"none", 				"tile_empty.png", 				"Empty"},
	{TILE_GRASS,				"grass", 				"tile_grass.png", 				"Grass"},
	{TILE_STONE,				"stone", 				"tile_stone.png", 				"Stone"},
	{TILE_FOREST,				"forest", 				"tile_forest.png",				"Forest"},
	{TILE_SEAWATER, 			"seawater", 			"tile_seawater.png", 			"Sea water"},
	{TILE_SWIMMINGPOOLWATER,	"swimmingpoolwater", 	"tile_swimmingpoolwater.png", 	"Swimming pool"},
	{TILE_FLOORTILE,			"floortile", 			"tile_floortile.png", 			"Floor tile"},
	{TILE_LINOLEUM,				"linoleum", 			"tile_linoleum.png", 			"Linoleum"},
	{TILE_MOSAIC,				"mosaic", 				"tile_mosaic.png", 				"Mosaic"},
	{TILE_PARQUET,				"parquet", 			"tile_parquet.png", 			"Parquet"},
	{TILE_CARPET,				"carpet", 				"tile_carpet.png", 				"Carpet"},
};
