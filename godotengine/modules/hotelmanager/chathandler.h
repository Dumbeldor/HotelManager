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

#include <string>
#include <vector>

class GameSession;
class Console;
class ChatHandler;

struct ChatCommand
{
	const char *name;
	bool console;
	bool (ChatHandler::*Handler)(const std::string &args, GameSession *game_session,
				     std::string &msg);
	ChatCommand *childCommand;
	const std::string help;
};

enum ChatCommandSearchResult : uint8_t
{
	CHAT_COMMAND_OK,		 // Found accessible command by command string
	CHAT_COMMAND_UNKNOWN,		 // First level command not found
	CHAT_COMMAND_UNKNOWN_SUBCOMMAND, // Command found but some level subcommand not find in
					 // subcommand list
};

class ChatHandler
{
public:
	ChatHandler(Console *console) : m_console(console){};
	~ChatHandler(){};
	bool handle_command(const std::string &text, GameSession *game_session, std::string &msg);

	// Handlers

	// Money
	bool handle_command_money_add(const std::string &args, GameSession *game_session,
				      std::string &msg);
	bool handle_command_money_remove(const std::string &args, GameSession *game_session,
					 std::string &msg);

	// Time
	bool handle_command_time_speed(const std::string &args, GameSession *game_session,
				       std::string &msg);

	// list
	bool handle_command_list(const std::string &args, GameSession *game_session,
				 std::string &msg);

	// help
	bool handle_command_help(const std::string &args, GameSession *game_session,
				 std::string &msg);

	// Utils for dev
	bool handle_command_remove_line(const std::string &args, GameSession *game_session,
					std::string &msg);
	bool handle_command_add_notif(const std::string &args, GameSession *game_session,
				      std::string &msg);

private:
	ChatCommandSearchResult find_command(ChatCommand *table, const char *&text,
					     ChatCommand *&command,
					     ChatCommand **parentCommand = nullptr);
	ChatCommand *getCommandTable();

	Console *m_console;
};
