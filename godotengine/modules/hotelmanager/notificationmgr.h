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
#include <reference.h>
#include "notification.h"
#include <scene/resources/packed_scene.h>
#include <vector>
#include <scene/gui/panel.h>
#include <functional>

class NotificationMgr : public Panel
{
	OBJ_TYPE(NotificationMgr, Panel);

public:
	NotificationMgr();
	~NotificationMgr();
	void add_notification(const String &title, const String &text, const String &icon,
		NotificationCallback callback = nullptr, const uint32_t &callback_id = 0);
	bool remove_notification(const uint16_t id);
	void _process(const float &delta);
	void reorganize(uint8_t nb);
};
