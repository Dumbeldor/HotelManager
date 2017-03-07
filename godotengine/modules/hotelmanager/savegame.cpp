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
#include "scene/2d/sprite.h"
#include <core/io/base64.h>
#include <ctime>
#include <iostream>
#include <os/dir_access.h>
#include <os/file_access.h>
#include <scene/main/node.h>

#define SAVE_VERSION 1

/**
 * Create save game & related directories
 *
 * @param game_session
 * @param game_map
 */
void SaveGame::save(const GameSession *game_session, const GameMap *game_map)
{
	Error err;
	Error errDir;

	DirAccess *dir = DirAccess::open("user://");

	if (!dir->dir_exists("save")) {
		errDir = dir->make_dir("save");
		ERR_FAIL_COND((errDir != OK && errDir != ERR_ALREADY_EXISTS));
	}

	FileAccess *file =
	    FileAccess::open("user://save/" + m_name + ".save", FileAccess::WRITE, &err);
	ERR_FAIL_COND(err != OK || file == NULL);

	Dictionary savedict;
	savedict["version"] = SAVE_VERSION;
	savedict["money"] = game_session->get_money();
	Dictionary map_dict;
	game_map->serialize(map_dict);
	savedict["map"] = map_dict;
	savedict["timestamp"] = time(NULL);
	savedict["game_time"] = game_session->get_game_time();
	savedict["game_speed"] = game_session->get_game_speed();

	// @TODO: save achievement progress too

	file->store_line(savedict.to_json());
	file->close();
}

bool SaveGame::load(GameSession *game_session, GameMap *game_map)
{
	Error err;
	FileAccess *file =
	    FileAccess::open("user://save/" + m_name + ".save", FileAccess::READ, &err);

	if (err != OK || file == NULL) {
		return false;
	}

	Dictionary data;
	err = data.parse_json(file->get_line().c_str());

	if (err != OK || data == NULL) {
		return false;
	}

	if (!(data.has("version") && (int) data["version"] == SAVE_VERSION)) {
		return false;
	}
	if (!(data.has("money") && data.has("map") && data.has("timestamp") &&
	      data.has("game_time") && data.has("game_speed"))) {
		return false;
	}

	game_session->set_money((int64_t) data["money"]);
	game_session->set_game_time((double) data["game_time"]);
	game_session->set_game_speed((uint8_t) data["game_speed"]);

	return game_map->init(game_session, data["map"]);
}
