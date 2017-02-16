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

#pragma once

#include <cstdint>
#include <unordered_map>
#include "actorobject.h"

class ObjectMgr
{
public:
	ObjectMgr();
	~ObjectMgr();

	void register_ao(ActorObject *ao);
	void unregister_ao(const uint32_t id);

	static ObjectMgr *get_singleton() { return s_singleton; }
private:
	const uint32_t &next_id();

	uint32_t m_next_id = 0;
	std::unordered_map<uint32_t, ActorObject *> m_actor_objects = {};

	// Singleton
	static ObjectMgr *s_singleton;
};
