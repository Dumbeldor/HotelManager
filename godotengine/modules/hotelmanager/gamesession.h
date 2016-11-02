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

#include "reference.h"

class GameSession: public Reference
{
	OBJ_TYPE(GameSession, Reference);
public:
	GameSession() {}

	int64_t get_money() const { return m_money; }
	void set_money(int64_t money);
	void add_money(int64_t money);
	void remove_money(int64_t money);

protected:
	static void _bind_methods();

private:
	int64_t m_money = 0;
};
