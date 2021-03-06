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

#include "notification.h"
#include "notificationmgr.h"
#include <math/math_2d.h>
#include <modules/hotelmanager/hud/hud.h>
#include <scene/animation/animation_player.h>
#include <scene/gui/label.h>
#include <scene/2d/sprite.h>
#include <iostream>

Notification::Notification() {}

Notification::~Notification() {}

void Notification::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_on_animation_finished"),
				  &Notification::_on_animation_finished);
	ObjectTypeDB::bind_method(_MD("_on_closebutton_released"),
				  &Notification::_on_closebutton_released);
	ObjectTypeDB::bind_method(_MD("_on_input_event"), &Notification::_on_input_event);
}

void Notification::_on_animation_finished()
{
	if ($("Animation")->cast_to<AnimationPlayer>()->get_current_animation() ==
	    String("Hide")) {
		get_parent()->cast_to<NotificationMgr>()->reorganize(m_nb);
		queue_delete();
	}
}

void Notification::init(const String &title, const String &desc, const uint16_t nb, const String &icon,
		NotificationCallback callback, const uint32_t &callback_id)
{
	set_title(title);
	set_description(desc);
	set_icon(icon);
	init_pos(nb);
	m_callback = callback;
	m_callback_id = callback_id;
	m_expired_time = 20.0f;
}

void Notification::init_pos(const uint16_t nb)
{
	m_nb = nb;
	set_pos(Vector2(get_pos().x, -(95 * (nb - 1))));
}

void Notification::set_title(const String &title)
{
	Label *label = $("Icon/TitleLabel")->cast_to<Label>();
	assert(label);
	label->set_text(title);
}

void Notification::set_icon(const String &icon)
{
	Sprite *sprite = $("Icon")->cast_to<Sprite>();
	assert(sprite);
	Ref<Resource> res = ResourceLoader::load(icon);
	Texture *text = res->cast_to<Texture>();
	sprite->set_texture(text);
}

void Notification::set_description(const String &desc)
{
	Label *label = $("Icon/DescriptionLabel")->cast_to<Label>();
	assert(label);
	label->set_text(desc);
}

void Notification::_on_closebutton_released()
{
	$("Animation")->cast_to<AnimationPlayer>()->play(String("Hide"));
}

void Notification::_on_input_event(const InputEvent &e)
{
	if (e.type == InputEvent::MOUSE_BUTTON) {
		if (e.mouse_button.button_index == BUTTON_LEFT) {
			AnimationPlayer *anim = $("Animation")->cast_to<AnimationPlayer>();
			assert(anim);
			if (m_callback && !anim->is_playing()) {
				m_callback(m_callback_id);
				anim->play(String("Hide"));
			}
		}
	}
}
