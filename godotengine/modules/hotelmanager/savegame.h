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

#include "gamesession.h"
#include "gamemap.h"


class SaveGame
{
public:
	SaveGame() {};
	~SaveGame() {};

	void save(const String &name, const GameSession *game_session, const GameMap *game_map);

protected:
	void init() {};
	void serialize(Dictionary &result, const GameMap *game_map) const;

private:
};
