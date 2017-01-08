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
 * 2017, Vincent Glize <vincent.glize@live.fr>
 *
 * All rights reserved
 */

#include "savegame.h"
#include <os/dir_access.h>
#include <os/file_access.h>
#include "scene/2d/sprite.h"
#include <ctime>
#include <scene/main/node.h>
#include <core/io/base64.h>
#include <iostream>

#define SAVE_VERSION 1

/**
 * Save the game
 * @param name of file save
 */
void SaveGame::save(const String &name, const GameSession *game_session, const GameMap *game_map)
{
	Error err;
	Error errDir;

	DirAccess *dir = DirAccess::open("user://");

	if (!dir->dir_exists("save")) {
		errDir = dir->make_dir("save");
		ERR_FAIL_COND((errDir != OK && errDir != ERR_ALREADY_EXISTS));
	}

	FileAccess *file = FileAccess::open("user://save/" + name + ".save", FileAccess::WRITE, &err);
	ERR_FAIL_COND(err != OK || file == NULL);

	Dictionary savedict;
	savedict["version"] = SAVE_VERSION;
	savedict["money"] = game_session->get_money();
	Dictionary map;
	serialize(map, game_map);
	savedict["map"] = map;
	savedict["timestamp"] = time(NULL);
	savedict["game_time"] = game_session->get_game_time();
	savedict["game_speed"] = game_session->get_game_speed();
	file->store_line(savedict.to_json());

	file->close();
}

void SaveGame::serialize(Dictionary &result, const GameMap *game_map) const
{
	result.clear();
	/**
	 * Serialize Map
	 */
	result["limit_x"] = WORLD_LIMIT_X;
	result["limit_y"] = WORLD_LIMIT_Y;
	int WORLD_LIMIT = ((WORLD_LIMIT_X + 1) * (WORLD_LIMIT_Y + 1)) * 2;
	DVector<char> ground_map;
	DVector<char> floor_map;

	ground_map.resize(WORLD_LIMIT);
	floor_map.resize(WORLD_LIMIT);

	uint16_t i = 0;
	for (int16_t x = -WORLD_LIMIT_X; x <= WORLD_LIMIT_X; x++) {
		for (int16_t y = -WORLD_LIMIT_Y; y <= WORLD_LIMIT_Y; y++) {
			ground_map.push_back((char) game_map->get_ground_map()->get_cell(x, y));
			floor_map.push_back((char) game_map->get_floor_map()->get_cell(x, y));
			i++;
		}
	}

	int b64len = WORLD_LIMIT / 3 * 4 + 4 + 1;
	DVector<char> b64buff;
	b64buff.resize(b64len);

	DVector<char>::Write w_ground = ground_map.write();
	DVector<char>::Write w64_ground = b64buff.write();
	int strlen_ground = base64_encode((char*)(&w64_ground[0]), (char*)(&w_ground[0]), (uint32_t )WORLD_LIMIT);
	w64_ground[strlen_ground] = 0;

	DVector<char>::Write w_floor = floor_map.write();
	DVector<char>::Write w64_floor = b64buff.write();
	int strlen_floor = base64_encode((char*)(&w64_floor[0]), (char*)(&w_floor[0]), (uint32_t) WORLD_LIMIT);
	w64_floor[strlen_floor] = 0;

	result["ground"] = (char*)&w64_ground[0];
	result["floor"] = (char*)&w64_floor[0];

	/**
	 * Serialize Camera
	 */
	Dictionary camera;
	camera["pos_x"] = game_map->get_camera()->get_pos().x;
	camera["pos_y"] = game_map->get_camera()->get_pos().y;
	camera["zoom_x"] = game_map->get_camera()->get_zoom().x;
	camera["zoom_y"] = game_map->get_camera()->get_zoom().y;

	result["camera"] = camera;
}
