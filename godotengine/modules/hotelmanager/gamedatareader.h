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
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include <cstdint>
#include <ustring.h>

class FileAccess;

class GameDataReader
{
public:
	GameDataReader(const String &resourcename, const uint16_t col_number);
	~GameDataReader();

	const bool is_good() const { return m_is_good; }

	template<typename T> GameDataReader &operator>>(T &out);
	void next();
	void nextr();
private:
	uint16_t m_col_number = 1;
	uint16_t m_current_col = 0;
	FileAccess *m_file = nullptr;
	bool m_is_good = true;

	Vector<String> m_csv_line;
};
