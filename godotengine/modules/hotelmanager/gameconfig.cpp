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
 * 2016-2017, Vincent Glize <vincent.glize@live.fr>
 *
 * All rights reserved
 */

#include "gameconfig.h"
#include <cassert>

GameConfig *GameConfig::singleton = nullptr;

GameConfig::GameConfig()
{
	assert(singleton == nullptr);

	singleton = this;
}

GameConfig::~GameConfig() { singleton = nullptr; }

GameConfig *GameConfig::get_singleton() { return singleton; }

void GameConfig::_bind_methods()
{
	ObjectTypeDB::bind_method("set_auto_save", &GameConfig::set_auto_save);
	ObjectTypeDB::bind_method("set_interval_save", &GameConfig::set_interval_save);
}

void GameConfig::set_auto_save(const bool auto_save)
{
	m_auto_save = auto_save;
	set_value("Game", "auto_save", auto_save);
}

void GameConfig::set_interval_save(const float interval_save)
{
	m_interval_save = interval_save;
	set_value("Game", "interval_save", interval_save);
}
