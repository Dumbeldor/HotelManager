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
#include <memory>

class ActorObject
{
public:
	ActorObject() {}
	virtual ~ActorObject() {}

	void set_id(const uint32_t id);
private:
	uint32_t m_id = 0;
};

typedef std::shared_ptr<ActorObject> ActorObjectPtr;
