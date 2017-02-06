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
#include <ustring.h>
#include <vector>
#include <string>

struct TileGroup
{
	uint32_t id = 0;
	std::string name = "";
};

static constexpr uint8_t TILEGROUPS_CSV_COLS = 2;

static constexpr uint16_t GAME_TILE_SIZE = 48;
static constexpr uint16_t TILE_NONE = 0;
static constexpr uint32_t TILE_MAX = UINT32_MAX;

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
	TILE_FLAG_SOLID = 0x02,
	TILE_FLAG_OPENABLE = 0x04,
	TILE_FLAG_MAX = 0x08,
};

struct GameTileDef
{
public:
	uint32_t id;
	std::vector<uint32_t> groups = {};
	String name;
	String texture_name;
	String label;
	int32_t flags;
	uint32_t cost;

	bool is_in_group(const uint32_t gid) const;
};

static constexpr uint8_t TILEDEF_CSV_COLS = 7;
