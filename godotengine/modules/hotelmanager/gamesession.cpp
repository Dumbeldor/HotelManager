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
#include "modules/hotelmanager/hud/hud.h"
#include "savegame.h"
#include "log.h"
#include "gameconfig.h"
#include <unistd.h>

#define MONEY_LIMIT 1000000000000

GameSession::~GameSession()
{
	if (ObjectDefMgr::get_singleton()) {
		delete ObjectDefMgr::get_singleton();
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
	assert(!ObjectDefMgr::get_singleton());
	new ObjectDefMgr();

	// Init hud
	m_hud = get_node(String("GameMap/Hud"))->cast_to<Hud>();
	assert(m_hud);

	m_hud->init();
	m_hud->set_money_label(m_money);
	m_hud->set_day_label(get_current_day());
	m_hud->modify_clock(m_game_time);

	m_map = get_node(String("GameMap"))->cast_to<GameMap>();
	assert(m_map);

	// Map should be inited quickly
	SaveGame save(savename);
	if (!savename.empty()) {
		save.load(this, m_map);
		// TODO: deserialize other args for this session
	}
	else {
		Dictionary fake_map;
		m_map->init(this, fake_map);
	}

	// No mission & no mission progress recorded, it's a new game
	if (m_mission_progress.empty()) {
		start_mission(1);
	}

	//Auto save
	m_autosave_timer = GameConfig::get_singleton()->get_interval_save();
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
	// Auto save
	if (GameConfig::get_singleton()->get_auto_save()) {
		m_autosave_timer -= delta;
		if (m_autosave_timer <= 0.0f) {
			m_autosave_timer = GameConfig::get_singleton()->get_interval_save();
			char buf[16];
			snprintf(buf, 16, "%lu", time(NULL));
			String date = "autosave_" + (String) buf;
			save(date);
		}
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

		m_hud->modify_clock(m_game_time);
		m_map->apply_daynight_cycle(m_game_time);
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

/**
 * Start mission_id if exists and not already started
 *
 * @param mission_id
 */
void GameSession::start_mission(const uint32_t mission_id)
{
	const Mission &mission = ObjectDefMgr::get_singleton()->get_mission(mission_id);
	if (mission.id == 0) {
		LOG_CRIT("start_mission: mission id %d not found, not starting.", mission_id);
		return;
	}

	MissionProgressPtr current_progress;
	const auto &mission_progress = m_mission_progress.find(mission_id);
	if (mission_progress == m_mission_progress.end()) {
		m_mission_progress[mission_id] = MissionProgressPtr(new MissionProgress());
		current_progress = m_mission_progress[mission_id];
	}
	else {
		current_progress = mission_progress->second;
	}

	if (current_progress->state != MISSION_STATE_NOT_STARTED) {
		LOG_WARN("start_mission: trying to start already started or finished mission %d, "
			"not starting.",
			mission_id);
		return;
	}

	// Verify if parent missions are finished
	for (const auto &parent: mission.parents) {
		const auto &parent_progress = m_mission_progress.find(parent);
		if (parent_progress == m_mission_progress.end()) {
			LOG_CRIT("start_mission: unable to start mission %d, mission %d requirement "
				"not accomplished", mission.id, parent);
			return;
		}

		if (parent_progress->second->state != MISSION_STATE_DONE) {
			LOG_CRIT("start_mission: unable to start mission %d, mission %d requirement "
				"not finished", mission.id, parent);
			return;
		}
	}

	// Mark mission as in progress & populate objectives
	current_progress->state = MISSION_STATE_IN_PROGRESS;
	for (const auto &mo: mission.objectives) {
		current_progress->objectives_progress[mo->id] = MissionObjectiveProgress();
		current_progress->objectives_progress[mo->id].id = mo->id;
	}

	m_hud->add_mission(mission);
}
