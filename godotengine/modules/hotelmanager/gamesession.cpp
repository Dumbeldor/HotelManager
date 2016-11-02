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

#include "gamesession.h"

#define MONEY_LIMIT 1000000000000

void GameSession::_bind_methods()
{
	ObjectTypeDB::bind_method("get_money",&GameSession::get_money);
	ObjectTypeDB::bind_method("set_money",&GameSession::set_money);
	ObjectTypeDB::bind_method("add_money",&GameSession::add_money);
	ObjectTypeDB::bind_method("remove_money",&GameSession::remove_money);
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
