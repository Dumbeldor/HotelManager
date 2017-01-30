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

#pragma once
#include <io/config_file.h>
#include <atomic>

class GameConfig: public ConfigFile
{
	OBJ_TYPE(GameConfig, ConfigFile);
public:
	GameConfig();
	~GameConfig();

	static GameConfig *get_singleton();
	void set_auto_save(const bool auto_save);
	bool get_auto_save() const { return m_auto_save; }
	void set_interval_save(const float interval_save);
	float get_interval_save() const { return m_interval_save; }

protected:
	static void _bind_methods();

private:
	static GameConfig *singleton;
	std::atomic_bool m_auto_save;
	std::atomic<float> m_interval_save;

};

