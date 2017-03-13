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

#include <ctime>
#include <scene/gui/panel.h>

class Notification : public Panel
{
	OBJ_TYPE(Notification, Panel);

public:
	Notification();
	~Notification();
	void init(const String &title, const String &desc, const uint16_t nb, const String &icon);
	void init_pos(const uint16_t nb);
	void set_title(const String &title);
	void set_icon(const String &icon);
	void set_description(const String &desc);
	void _process(const float &time) { m_expired_time -= time; }
	bool is_expired() { return m_expired_time <= 0; }

private:
	float m_expired_time = 20.0f;
	uint8_t m_nb = 0;

protected:
	static void _bind_methods();
	void _on_animation_finished();
	void _on_closebutton_released();
};
