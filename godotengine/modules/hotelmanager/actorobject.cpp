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
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "actorobject.h"
#include "objectmgr.h"

ActorObject::ActorObject()
{
	// Necessary condition for editor
	if (ObjectMgr::get_singleton()) {
		ObjectMgr::get_singleton()->register_ao(this);
	}
}

ActorObject::~ActorObject()
{
	// Necessary condition for editor
	if (ObjectMgr::get_singleton()) {
		ObjectMgr::get_singleton()->unregister_ao(m_id);
	}
}

void ActorObject::set_id(const uint32_t id)
{
	assert(m_id == 0);
	m_id = id;
}
