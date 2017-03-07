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

#include "gamedatareader.h"
#include "log.h"
#include <core/os/file_access.h>
#include <iostream>
#include <string>
#include <vector>

#define GAMEDATA_PATH String("res://gamedata/")

GameDataReader::GameDataReader(const String &resourcename, const uint16_t col_number)
{
	m_col_number = col_number;
	Error err;
	m_file =
	    FileAccess::open(GAMEDATA_PATH + resourcename + String(".csv"), FileAccess::READ, &err);
	if (err != OK || m_file == NULL) {
		LOG_CRIT("No %s game data, ignoring.", resourcename.ascii().get_data());
		m_is_good = false;
		return;
	}

	nextr();
}

GameDataReader::~GameDataReader()
{
	if (m_file) {
		m_file->close();
	}
}

void GameDataReader::nextr()
{
	if (!m_is_good) {
		return;
	}

	m_csv_line = m_file->get_csv_line();
	if (m_csv_line.size() < 2) {
		m_is_good = false;
	} else if (m_csv_line.size() != m_col_number) {
		LOG_CRIT("invalid CSV line (found %d cols, expected %d), ignoring.",
			 m_csv_line.size(), m_col_number);
		nextr();
	}
}

void GameDataReader::next()
{
	m_current_col++;
	if (m_current_col == m_col_number) {
		m_current_col = 0;
		nextr();
	}
}

template <> GameDataReader &GameDataReader::operator>>(uint8_t &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = (uint8_t) m_csv_line.get(m_current_col).to_int();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(uint16_t &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = (uint16_t) m_csv_line.get(m_current_col).to_int();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(uint32_t &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = (uint32_t) m_csv_line.get(m_current_col).to_int();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(float &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = (uint32_t) m_csv_line.get(m_current_col).to_float();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(std::vector<uint32_t> &out)
{
	if (!m_is_good) {
		return *this;
	}

	Vector<String> out_str = m_csv_line.get(m_current_col).split(";");
	for (uint32_t i = 0; i < out_str.size(); i++) {
		if (out_str[i].empty()) {
			continue;
		}
		out.push_back((uint32_t) out_str[i].to_int());
	}
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(Vector<uint32_t> &out)
{
	if (!m_is_good) {
		return *this;
	}

	Vector<String> out_str = m_csv_line.get(m_current_col).split(";");
	for (uint32_t i = 0; i < out_str.size(); i++) {
		if (out_str[i].empty()) {
			continue;
		}
		out.push_back((uint32_t) out_str[i].to_int());
	}
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(int32_t &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = m_csv_line.get(m_current_col).to_int();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(std::string &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = m_csv_line.get(m_current_col).utf8();
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(String &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = m_csv_line.get(m_current_col);
	next();
	return *this;
}

template <> GameDataReader &GameDataReader::operator>>(Vector<String> &out)
{
	if (!m_is_good) {
		return *this;
	}
	out = m_csv_line.get(m_current_col).split(";");
	next();
	return *this;
}
