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

#include <cassert>
#include "gamesession.h"
#include "gamemap.h"

#define MONEY_LIMIT 1000000000000

void GameSession::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GameSession::init);
	ObjectTypeDB::bind_method(_MD("_process"),&GameSession::_process);

	// m_money
	ObjectTypeDB::bind_method("get_money",&GameSession::get_money);
	ObjectTypeDB::bind_method("set_money",&GameSession::set_money);
	ObjectTypeDB::bind_method("add_money",&GameSession::add_money);
	ObjectTypeDB::bind_method("remove_money",&GameSession::remove_money);

	// m_game_speed
	ObjectTypeDB::bind_method("get_game_speed",&GameSession::get_game_speed);
	ObjectTypeDB::bind_method("set_game_speed",&GameSession::set_game_speed__api);

	// m_current_day
	ObjectTypeDB::bind_method("get_current_day",&GameSession::get_current_day);
	ObjectTypeDB::bind_method("set_next_day",&GameSession::set_next_day);
}

void GameSession::init()
{
	m_map = get_node(String("GameMap"))->cast_to<GameMap>();
	assert(m_map);

	// Map should be inited quickly
	m_map->init();
}

void GameSession::_process(float delta)
{
	// Map processing
	m_map->on_process(delta);
}

void GameSession::set_money(int64_t money)
{
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}

	m_money = money;
}

void GameSession::add_money(int64_t money)
{
	// Limit money to add to limit
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}

	m_money += money;

	// If money overhead is greater than limit, limit it
	if (m_money > MONEY_LIMIT) {
		m_money = MONEY_LIMIT;
	}
}

void GameSession::remove_money(int64_t money)
{
	// Limit money to add to limit
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}

	m_money -= money;

	// If money overhead is greater than limit, limit it
	if (m_money < -MONEY_LIMIT) {
		m_money = -MONEY_LIMIT;
	}
}
