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

#include <cassert>
#include <iostream>
#include "objectmgr.h"

ObjectMgr *ObjectMgr::s_singleton = nullptr;

ObjectMgr::ObjectMgr()
{
	assert(ObjectMgr::s_singleton == nullptr); // Should be null here
	ObjectMgr::s_singleton = this;
}

ObjectMgr::~ObjectMgr()
{
	ObjectMgr::s_singleton = nullptr;
}

const uint32_t& ObjectMgr::next_id()
{
	// Search next available ID
	m_next_id++;
	while (m_actor_objects.find(m_next_id) != m_actor_objects.end()) {
		assert(m_next_id < UINT32_MAX);
		m_next_id++;
	}
	return m_next_id;
}

void ObjectMgr::register_ao(ActorObject *ao)
{
	const uint32_t &new_id = next_id();
	ao->set_id(new_id);
	assert(m_actor_objects.find(new_id) == m_actor_objects.end());
	m_actor_objects[new_id] = ao;
}

void ObjectMgr::unregister_ao(const uint32_t id)
{
	auto itr = m_actor_objects.find(id);
	if (itr != m_actor_objects.end()) {
		m_actor_objects.erase(itr);
	}
}

void ObjectMgr::step(const double &dtime)
{
	for (auto &obj: m_actor_objects) {
		obj.second->step(dtime);
	}
}
