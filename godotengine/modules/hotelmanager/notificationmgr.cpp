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

#include <io/resource_loader.h>
#include <modules/hotelmanager/hud/hud.h>
#include <scene/animation/animation_player.h>
#include "notificationmgr.h"
#include "notification.h"

NotificationMgr::NotificationMgr()
{
}

NotificationMgr::~NotificationMgr()
{
}

void NotificationMgr::add_notification(const String &title, const String &text)
{
	Ref<Resource> res = ResourceLoader::load("res://scenes/Notification.tscn");
	Ref<PackedScene> ps = res;
	Notification *notif = ps->instance()->cast_to<Notification>();

	notif->init(title, text, get_child_count() + 1);
	add_child(notif);
}

void NotificationMgr::remove_notification(const uint16_t id)
{
	Notification *notif = get_child(id)->cast_to<Notification>();

	assert(notif);
	notif->get_node(String("Animation"))->cast_to<AnimationPlayer>()->play(String("Hide"));
}

void NotificationMgr::_process(const float &delta)
{
	for (int i = 0; i < get_child_count(); i++) {
		Notification *notif = get_child(i)->cast_to<Notification>();
		assert(notif);
		notif->_process(delta);
		if (notif->is_expired() && !notif->get_node(String("Animation"))->cast_to<AnimationPlayer>()->is_playing()) {
			remove_notification(i);
		}
	}
}

void NotificationMgr::reorganize(uint8_t nb)
{
	if (get_child_count() > nb) {
		for (uint8_t i = nb; i < get_child_count(); i++) {
			get_child(i)->cast_to<Notification>()->init_pos(0);
		}
	}
}