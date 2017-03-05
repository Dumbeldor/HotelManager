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
 * 2017, Vincent Glize <vincent.glize@live.fr>
 *
 * All rights reserved
 */

#pragma once

#include <cstdint>
#include <scene/resources/packed_scene.h>
#include <reference.h>
#include <scene/main/node.h>
#include <vector>

class Notification;

class NotificationMgr: public Node
{
	OBJ_TYPE(NotificationMgr, Node);
public:
	NotificationMgr();
	~NotificationMgr();
	void add_notification(const String &title, const String &text);
	void remove_notification(const uint16_t id);
	void _process(const float &delta);
	void reorganize(uint8_t nb);

};
