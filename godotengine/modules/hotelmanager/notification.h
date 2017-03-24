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
#include <functional>

typedef std::function<void(int)> NotificationCallback;

class Notification : public Panel
{
	OBJ_TYPE(Notification, Panel);

public:
	Notification();
	~Notification();
	void init(const String &title, const String &desc, const uint16_t nb, const String &icon,
		NotificationCallback callback = nullptr, const uint32_t &callback_id = 0);
	void init_pos(const uint16_t nb);
	void set_title(const String &title);
	void set_icon(const String &icon);
	void set_description(const String &desc);
	void _process(const float &time) { m_expired_time -= time; }
	bool is_expired() { return m_expired_time <= 0; }
	void _on_input_event(const InputEvent &e);
	void set_notification_callback_objid(uint32_t callback_id) { m_callback_id = callback_id; }

private:
	float m_expired_time = 20.0f;
	uint8_t m_nb = 0;
	uint32_t m_callback_id = 0;
	NotificationCallback m_callback = nullptr;

protected:
	static void _bind_methods();
	void _on_animation_finished();
	void _on_closebutton_released();
};
