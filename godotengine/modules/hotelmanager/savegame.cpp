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

	FileAccess *file = FileAccess::open("user://save/" + m_name + ".save", FileAccess::WRITE, &err);
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
