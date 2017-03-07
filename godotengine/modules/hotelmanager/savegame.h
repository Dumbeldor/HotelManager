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

#pragma once

#include "gamemap.h"
#include "gamesession.h"

class SaveGame
{
public:
	SaveGame(const String &name) : m_name(name){};
	~SaveGame(){};

	void save(const GameSession *game_session, const GameMap *game_map);
	bool load(GameSession *game_session, GameMap *game_map);

private:
	String m_name = "";
};
