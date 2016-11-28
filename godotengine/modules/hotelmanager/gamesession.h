/**
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

class GameMap;
class Hud;
class ObjectDefMgr;

enum GameSpeed
{
	GAMESPEED_NORMAL,
	GAMESPEED_X2,
	GAMESPEED_X3,
	GAMESPEED_X5,
};

class GameSession: public Node
{
	OBJ_TYPE(GameSession, Node);
public:
	GameSession() {}
	~GameSession();

protected:
	static void _bind_methods();

	void init();
	void _process(float delta);

	// m_money
	int64_t get_money() const { return m_money; }
	void set_money(int64_t money);
	void add_money(int64_t money);
	void remove_money(int64_t money);

	// m_game_speed
	GameSpeed get_game_speed() const { return m_game_speed; }
	void set_game_speed(const GameSpeed speed) { m_game_speed = speed; }
	void set_game_speed__api(const uint8_t speed) { m_game_speed = (GameSpeed) speed; }

	// m_current_day
	uint32_t get_current_day() const { return m_current_day; }
	void set_next_day() { m_current_day++; }
private:
	int64_t m_money = 2000;
	uint32_t m_current_day = 1;
	GameSpeed m_game_speed = GAMESPEED_NORMAL;

	GameMap *m_map = nullptr;
	Hud *m_hud = nullptr;
	ObjectDefMgr *m_objdef_mgr = nullptr;
};
