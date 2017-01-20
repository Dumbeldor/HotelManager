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
#include "object_type_db.h"
#include "hotelmanagerproject.h"
#include "base_overrided_components.h"
#include "gamesession.h"
#include "character.h"
#include "character/characterceo.h"
#include "character/characterroommaid.h"
#include "gamemap.h"
#include "gui_tabs.h"
#include "objectselectorbutton.h"
#include "modules/hotelmanager/hud/hud.h"
#include "mapcontrol.h"
#include "hud/clock.h"

void register_hotelmanager_types()
{
	ObjectTypeDB::register_type<HotelManagerProject>();
	ObjectTypeDB::register_type<GameSession>();
	ObjectTypeDB::register_type<Character>();
	ObjectTypeDB::register_type<CharacterCEO>();
	ObjectTypeDB::register_type<CharacterRoomMaid>();
	ObjectTypeDB::register_type<Clock>();
	ObjectTypeDB::register_type<GameMap>();
	ObjectTypeDB::register_type<LayerTileMenu>();
	ObjectTypeDB::register_type<ObjectSelectorButton>();
	ObjectTypeDB::register_type<Hud>();
	ObjectTypeDB::register_type<MapControl>();
	ObjectTypeDB::register_type<HMButton>();
}

void unregister_hotelmanager_types()
{

}
