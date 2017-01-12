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
 * 2016-2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include <scene/gui/tab_container.h>
#include "gamesession.h"
#include "gamemap.h"
#include "objectdefmgr.h"
#include "hud.h"
#include "savegame.h"
#include <unistd.h>

#define MONEY_LIMIT 1000000000000

GameSession::~GameSession()
{
	if (m_objdef_mgr) {
		m_objdef_mgr->delete_singleton();
	}
}

void GameSession::_bind_methods()
{
	ObjectTypeDB::bind_method("init", &GameSession::init);
	ObjectTypeDB::bind_method("save", &GameSession::save);
	ObjectTypeDB::bind_method(_MD("_process"),&GameSession::_process);

	// m_money
	ObjectTypeDB::bind_method("get_money",&GameSession::get_money);
	ObjectTypeDB::bind_method("set_money",&GameSession::set_money);
	ObjectTypeDB::bind_method("add_money",&GameSession::add_money);
	ObjectTypeDB::bind_method("remove_money",&GameSession::remove_money);

	// m_game_speed
	ObjectTypeDB::bind_method("get_game_speed",&GameSession::get_game_speed);
	ObjectTypeDB::bind_method("set_game_speed",&GameSession::set_game_speed__api);

	// m_current_day
	ObjectTypeDB::bind_method("get_current_day",&GameSession::get_current_day);
}

/**
 * Initialize session, objectdefmgr, hud & map
 */
void GameSession::init(const String &savename)
{
	// objdef_mgr should be inited first
	assert(!m_objdef_mgr);
	m_objdef_mgr = new ObjectDefMgr();

	// Init hud
	m_hud = get_node(String("GameMap/Hud"))->cast_to<Hud>();
	assert(m_hud);

	m_hud->init();
	m_hud->set_money_label(m_money);
	m_hud->set_day_label(get_current_day());
	m_hud->set_hour_clock_label(m_game_time);

	m_map = get_node(String("GameMap"))->cast_to<GameMap>();
	assert(m_map);

	// Map should be inited quickly
	SaveGame save(savename);
	if (!savename.empty()) {
		// TODO: save.check_if_exists();
		m_map->init(this, &save);
		// TODO: deserialize other args for this session
	}
	else {
		m_map->init(this);
	}
}

/**
 * Main GameSession loop
 * This loop handle:
 * - Game time
 *
 * @param delta
 */
void GameSession::_process(float delta)
{
	m_autosave_timer -= delta;
	if (m_autosave_timer <= 0.0f) {
		//get_node(String("GameMap/Hud/ControlPane/MapControl/SaveSprite"))->cast_to<Sprite>()->show();
		m_autosave_timer = 10.0f;
		char buf[16];
		snprintf(buf, 16, "%lu", time(NULL));
		String date = "autosave_" + (String) buf;
		save(date);
		//get_node(String("GameMap/Hud/ControlPane/MapControl/SaveSprite"))->cast_to<Sprite>()->hide();
	}
	// Delta using game_speed
	float game_delta = delta * m_game_speed;
	{
		uint32_t old_day = get_current_day();

		// Increase game_time
		m_game_time += game_delta;
		// Only update HUD if day changed
		if (get_current_day() != old_day) {
			m_hud->set_day_label(get_current_day());
		}

		m_hud->set_hour_clock_label(m_game_time);
	}

	// Map processing
	m_map->on_process(delta);
}

/**
 *
 * @param money
 */
void GameSession::set_money(int64_t money)
{
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}

	m_money = money;

	m_hud->set_money_label(m_money);
}

/**
 * Save the game
 * @param name of file save
 */
void GameSession::save(const String &name)
{
	SaveGame(name).save(this, m_map);
}

/**
 * Load the game
 * @param name of file save
 */
void GameSession::load(const String &name)
{
	// @TODO: load achievement progress to this session
}

/**
 * Add money to current player money
 * This is limited by MONEY_LIMIT
 *
 * @param money
 */
void GameSession::add_money(int64_t money)
{
	// Limit money to add to limit
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}

	m_money += money;

	// If money overhead is greater than limit, limit it
	if (m_money > MONEY_LIMIT) {
		m_money = MONEY_LIMIT;
	}

	m_hud->set_money_label(m_money);
}

/**
 * Drop some money from current player money
 * Money cannot be negative
 *
 * @param money
 */
void GameSession::remove_money(int64_t money)
{
	// Limit money to add to limit
	if (money > MONEY_LIMIT) {
		money = MONEY_LIMIT;
	}
	else if (money > m_money) {
		money = m_money;
	}

	m_money -= money;

	m_hud->set_money_label(m_money);
}
