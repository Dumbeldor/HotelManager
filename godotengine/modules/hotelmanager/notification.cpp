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

Notification::Notification() {}

Notification::~Notification() {}

void Notification::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("_on_animation_finished"),
				  &Notification::_on_animation_finished);
}

void Notification::_on_animation_finished()
{
	if (get_node(String("Animation"))->cast_to<AnimationPlayer>()->get_current_animation() ==
	    String("Hide")) {
		get_parent()->cast_to<NotificationMgr>()->reorganize(m_nb);
		queue_delete();
	}
}

void Notification::init(const String &title, const String &desc, const uint16_t nb)
{
	set_title(title);
	set_description(desc);
	init_pos(nb);
	m_expired_time = 20.0f;
}

void Notification::init_pos(const uint16_t nb)
{
	m_nb = nb;
	set_pos(Vector2(get_pos().x, get_pos().y + (80 * (nb - 1))));
}

void Notification::set_title(const String &title)
{
	Label *label = get_node(String("Icon/TitleLabel"))->cast_to<Label>();
	assert(label);
	label->set_text(title);
}

void Notification::set_icon(const String &icon) {}

void Notification::set_description(const String &desc)
{
	Label *label = get_node(String("Icon/DescriptionLabel"))->cast_to<Label>();
	assert(label);
	label->set_text(desc);
}
