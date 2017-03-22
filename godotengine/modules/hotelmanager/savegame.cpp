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
#include <unitypes.h>

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

	// Save missions
	const MissionProgressMap mission_progress = game_session->get_mission_progress();
	Dictionary missions;
	uint32_t nb_mission = 0;
	for (const auto &mi : mission_progress) {
		Dictionary mission;
		mission["id"] = mi.second->id;
		mission["state"] = (uint8_t) mi.second->state;
		Dictionary objectives_progress;
		uint32_t nb_objectives_progress = 0;
		for (const auto &op : mi.second->objectives_progress) {
			Dictionary ob;
			ob["id"] = op.second.id;
			ob["progress"] = op.second.progress;
			ob["done"] = op.second.done;
			objectives_progress[mi.first] = ob;
			nb_objectives_progress++;
		}
		mission["objectives_progress"] = objectives_progress;
		missions[mi.first] = mission;
		nb_mission++;
	}
	savedict["missions"] = missions;

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

	// Load missions
	MissionProgressMap mission_progress_map;
	Dictionary missions = data["missions"];

	List<Variant> keys;
	missions.get_key_list(&keys);

	for (List<Variant>::Element *E=keys.front(); E; E=E->next()) {
		MissionProgress *mission_progress = new MissionProgress();
		const Dictionary &mission = missions[String(E->get())];
		mission_progress->id = (uint32_t) mission["id"];
		mission_progress->state = (MissionState) (uint8_t) mission["state"];
		MissionObjectiveProgressMap mission_objective_progress_map;
		const Dictionary &objectives_progress = mission["objectives_progress"];
		List<Variant> keys_objectives;
		objectives_progress.get_key_list(&keys_objectives);
		for (List<Variant>::Element *K=keys.front(); K; K=K->next()) {
			MissionObjectiveProgress mission_objective_progress;
			const Dictionary &ob = objectives_progress[String(K->get())];
			mission_objective_progress.id = (uint32_t) ob["id"];
			mission_objective_progress.done = (bool) ob["done"];
			mission_objective_progress.progress = (uint32_t) ob["progress"];
			mission_objective_progress_map[(uint32_t) K->get()] = mission_objective_progress;
		}
		mission_progress->objectives_progress = mission_objective_progress_map;

		MissionProgressPtr mission_progress_ptr(mission_progress);
		mission_progress_map[(uint32_t) E->get()] = mission_progress_ptr;
		game_session->start_mission((uint32_t) E->get());
	}

	game_session->set_mission_progress(mission_progress_map);

	//mission_progress_map.
	return game_map->init(game_session, data["map"]);
}
