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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "register_types.h"
#include "base_overrided_components.h"
#include "character/characterceo.h"
#include "character/characterroommaid.h"
#include "console.h"
#include "gameconfig.h"
#include "gamemap.h"
#include "gamesession.h"
#include "hotelmanagerproject.h"
#include "hud/clock.h"
#include "hud/hud.h"
#include "hud/selectorbutton.h"
#include "hud/selectormenu.h"
#include "mapcontrol.h"
#include "notification.h"
#include "notificationmgr.h"
#include "hud/missionhudcontainer.h"
#include "object_type_db.h"

void register_hotelmanager_types()
{
	ObjectTypeDB::register_type<HotelManagerProject>();
	ObjectTypeDB::register_type<GameSession>();
	ObjectTypeDB::register_type<CharacterCEO>();
	ObjectTypeDB::register_type<CharacterRoomMaid>();
	ObjectTypeDB::register_type<Clock>();
	ObjectTypeDB::register_type<GameMap>();
	ObjectTypeDB::register_type<NPCSelectorButton>();
	ObjectTypeDB::register_type<NPCSelectorMenu>();
	ObjectTypeDB::register_type<TileSelectorMenu>();
	ObjectTypeDB::register_type<TileSelectorButton>();
	ObjectTypeDB::register_type<Hud>();
	ObjectTypeDB::register_type<MapControl>();
	ObjectTypeDB::register_type<Console>();
	ObjectTypeDB::register_type<HMButton>();
	ObjectTypeDB::register_type<GameConfig>();
	ObjectTypeDB::register_type<Notification>();
	ObjectTypeDB::register_type<NotificationMgr>();
	ObjectTypeDB::register_type<MissionHudContainer>();
}

void unregister_hotelmanager_types() {}
