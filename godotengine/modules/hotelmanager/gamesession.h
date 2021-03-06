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
 * 2016-2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include "achievements.h"
#include "character/character.h"
#include "missions.h"
#include <cmath>
#include <scene/main/node.h>
#include <unordered_set>

class GameMap;
class Hud;
class NotificationMgr;
struct TileDef;

class GameSession : public Node
{
	friend class SaveGame;
	OBJ_TYPE(GameSession, Node);

public:
	GameSession() { m_achievements.clear(); }
	~GameSession();

	/**
	 * @param how
	 * @return true if money >= how
	 */
	bool has_money(int64_t how) const { return m_money >= how; }
	/**
	 * @return m_money value
	 */
	int64_t get_money() const { return m_money; };
	void set_money(int64_t money);
	void remove_money(int64_t money);
	void add_money(int64_t money);

	uint8_t get_game_speed() const { return m_game_speed; };
	void set_game_speed(const uint8_t speed) { m_game_speed = speed; }

	double get_game_time() const { return m_game_time; };
	void set_game_time(const double &game_time) { m_game_time = game_time; }

	void add_notification(const String &title, const String &text, const String &icon);
	bool remove_notification(const uint16_t id);

	Character *hire_character(CharacterRole role);
	bool on_tile_placed(const TileDef &tiledef, uint32_t count);

protected:
	static void _bind_methods();

	void init(const String &savename = "");
	void _process(float dtime);
	void save(const String &name);
	void load(const String &name);

	void set_game_speed__api(const int speed) { m_game_speed = (uint8_t) speed; }

	/**
	 * Calculate current day
	 * 1 day = m_game_time / (60 * 24) = 24 min real time
	 * @return current day from m_game_time
	 */
	uint32_t get_current_day() const
	{
		return (uint32_t) std::floor(m_game_time / 60 / 24) + 1;
	}

private:
	void start_mission(const Mission &mission);
	void start_mission(const uint32_t mission_id);
	void load_mission(const uint32_t mission_id);
	void load_mission_objective_progress(const MissionObjective &modef,
		const uint32_t progress) const;
	const MissionProgressMap &get_mission_progress() const { return m_mission_progress; }
	void set_mission_progress(const MissionProgressMap &misssion_progress)
	{
		m_mission_progress = misssion_progress;
	}

	// No mission & no mission progress recorded, it's a new game
	const bool is_new_game() const { return m_mission_progress.empty(); }

	// events
	void on_hire_character(const CharacterDef &cdef);
	void update_mission_progress(const MissionObjective::Type t, const uint32_t obj_id,
		const uint32_t obj_count = 1);

	int64_t m_money = 2000;
	uint8_t m_game_speed = 1;
	double m_game_time = 0;
	float m_autosave_timer = 300.0f;

	GameMap *m_map = nullptr;
	Hud *m_hud = nullptr;
	AchievementProgressMap m_achievements = {};
	MissionProgressMap m_mission_progress = {};
	NotificationMgr *m_notification_mgr = nullptr;
};
