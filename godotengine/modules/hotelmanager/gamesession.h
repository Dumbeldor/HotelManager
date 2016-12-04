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

#include <scene/main/node.h>
#include <cmath>

class GameMap;
class Hud;
class ObjectDefMgr;

enum GameSpeed
{
	GAMESPEED_PAUSE = 0,
	GAMESPEED_NORMAL = 1,
	GAMESPEED_X2 = 2,
	GAMESPEED_X3 = 3,
	GAMESPEED_X5 = 5,
};

class GameSession: public Node
{
	OBJ_TYPE(GameSession, Node);
public:
	GameSession() {}
	~GameSession();

	/**
	 * @param how
	 * @return true if money >= how
	 */
	bool has_money(int64_t how) const { return m_money >= how; }
	void remove_money(int64_t money);

protected:
	static void _bind_methods();

	void init();
	void _process(float delta);

	// m_money
	/**
	 * @return m_money value
	 */
	int64_t get_money() const { return m_money; }
	void set_money(int64_t money);
	void add_money(int64_t money);

	// m_game_speed
	GameSpeed get_game_speed() const { return m_game_speed; }
	void set_game_speed(const GameSpeed speed) { m_game_speed = speed; }
	void set_game_speed__api(const uint8_t speed) { m_game_speed = (GameSpeed) speed; }

	// m_current_day
	/**
	 * Calculate current day
	 * 1 day = m_game_time / (60 * 24) = 24 min real time
	 * @return current day from m_game_time
	 */
	uint32_t get_current_day() const
	{
		return (uint32_t) std::floor(m_game_time / 60 / 24) + 1;
	}
private:
	int64_t m_money = 2000;
	GameSpeed m_game_speed = GAMESPEED_NORMAL;
	double m_game_time = 0;

	GameMap *m_map = nullptr;
	Hud *m_hud = nullptr;
	ObjectDefMgr *m_objdef_mgr = nullptr;
};
