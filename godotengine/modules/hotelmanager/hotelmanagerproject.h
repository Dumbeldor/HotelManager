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

#include "reference.h"

class HotelManagerProject : public Reference
{
	OBJ_TYPE(HotelManagerProject, Reference);

public:
	HotelManagerProject();

	String get_full_version() const;
	/**
	 * @return version number of the project, example 0.4.1
	 */
	String get_version() const { return m_version; }

	/**
	 * @return release mode of the project
	 */
	String get_release() const { return m_release; }

	/**
	 * @return Just the project name
	 */
	String get_project_name() const { return m_project_name; }

protected:
	static void _bind_methods();

private:
	String m_version;
	String m_release;
	String m_project_name;
};
