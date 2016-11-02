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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "register_types.h"
#include "object_type_db.h"
#include "hotelmanagerproject.h"

void register_hotelmanager_types()
{
	ObjectTypeDB::register_type<HotelManagerProject>();
}

void unregister_hotelmanager_types()
{

}
