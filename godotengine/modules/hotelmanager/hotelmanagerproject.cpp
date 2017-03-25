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

#include "hotelmanagerproject.h"

HotelManagerProject::HotelManagerProject()
    : m_version("0.0.6"), m_release("alpha"), m_project_name("HotelManager")
{
}

void HotelManagerProject::_bind_methods()
{
	ObjectTypeDB::bind_method("get_full_version", &HotelManagerProject::get_full_version);
	ObjectTypeDB::bind_method("get_version", &HotelManagerProject::get_version);
	ObjectTypeDB::bind_method("get_release", &HotelManagerProject::get_release);
	ObjectTypeDB::bind_method("get_project_name", &HotelManagerProject::get_project_name);
}

/**
 * Full version of the project
 *
 * @return m_version + " " + m_release
 */
String HotelManagerProject::get_full_version() const { return m_version + " " + m_release; }
