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

#include "chathandler.h"
#include "console.h"
#include "gamesession.h"
#include <scene/gui/rich_text_label.h>

static const ChatCommand COMMANDHANDLERFINISHER = {nullptr, false, nullptr, nullptr, ""};

/**
 *
 * @return
 */
ChatCommand *ChatHandler::getCommandTable()
{
	static ChatCommand moneyCommandTable[] = {
	    {"add", true, &ChatHandler::handle_command_money_add, nullptr,
	     "Usage: /money add <count>"},
	    {"remove", true, &ChatHandler::handle_command_money_remove, nullptr,
	     "Usage: /money remove <count>"},
	    COMMANDHANDLERFINISHER,
	};

	static ChatCommand timeCommandTable[] = {
	    {"speed", true, &ChatHandler::handle_command_time_speed, nullptr,
	     "Usage: /time speed <multiplier>. multiplier value is between 0 and 100"},
	    COMMANDHANDLERFINISHER,
	};

	static ChatCommand notifCommandTable[] = {
	    {"add", true, &ChatHandler::handle_command_add_notif, nullptr,
	     "Usage: /notif add title description"},
	    {"remove", true, &ChatHandler::handle_command_remove_notif, nullptr,
		 "Use: /notif remove nb"},
	    COMMANDHANDLERFINISHER,
	};

	static ChatCommand globalCommandTable[] = {
	    {"money", true, nullptr, moneyCommandTable, "Available subcommands: add|remove"},
	    {"time", true, nullptr, timeCommandTable, "Available subcommands: speed"},
	    {"list", true, &ChatHandler::handle_command_list, nullptr, ""},
	    {"help", true, &ChatHandler::handle_command_help, nullptr, ""},
	    {"remove_line", true, &ChatHandler::handle_command_remove_line, nullptr,
	     "Usage: /remove_line <number>"},
	    {"notif", true, nullptr, notifCommandTable, "Notification subcommands: add|remove"},
	    COMMANDHANDLERFINISHER,
	};

	return globalCommandTable;
}

/**
 * @brief ChatHandler::handleCommand
 * @param text
 * @param GameSession
 * @return true if command was found and false if not found
 *
 */
bool ChatHandler::handle_command(const std::string &text, GameSession *game_session,
				 std::string &msg)
{
	// No slash or not 2 chars, it's not a command
	if (text.length() < 2 || text[0] != '/') {
		return false;
	}

	ChatCommand *command = nullptr;
	ChatCommand *parentCommand = nullptr;

	const char *ctext = &(text.c_str())[1];

	ChatCommandSearchResult res =
	    find_command(getCommandTable(), ctext, command, &parentCommand);
	switch (res) {
		case CHAT_COMMAND_OK:
			return (this->*(command->Handler))(ctext, game_session, msg);
		case CHAT_COMMAND_UNKNOWN_SUBCOMMAND:
			m_console->add_text(command->help);
			return true;
		case CHAT_COMMAND_UNKNOWN:
			m_console->add_text("Unknown command.");
			return false;
	}
}

/**
 * @brief ChatHandler::findCommand
 * @param table
 * @param text
 * @param command
 * @param parentCommand
 * @return ChatCommandSearchResult
 *
 * Search the command recursively into tables and associated handlers
 */
ChatCommandSearchResult ChatHandler::find_command(ChatCommand *table, const char *&text,
						  ChatCommand *&command,
						  ChatCommand **parentCommand)
{
	std::string cmd = "";
	// Skip whitespaces
	while (*text != ' ' && *text != '\0') {
		cmd += *text;
		++text;
	}

	while (*text == ' ') {
		++text;
	}

	// Search first level command in table
	for (int32_t i = 0; table[i].name != nullptr; ++i) {
		// If it's not the searched command, skip
		size_t len = strlen(table[i].name);
		if (strncmp(table[i].name, cmd.c_str(), len + 1) != 0) {
			continue;
		}

		// If there is a child command
		if (table[i].childCommand != nullptr) {
			const char *stext = text;
			ChatCommand *parentSubCommand = nullptr;
			ChatCommandSearchResult res =
			    find_command(table[i].childCommand, text, command, &parentSubCommand);

			// Lookup child command
			switch (res) {
				case CHAT_COMMAND_OK:
					// if subcommand success search not return parent command,
					// then
					// this parent command is owner of child commands
					if (parentCommand) {
						*parentCommand =
						    parentSubCommand ? parentSubCommand : &table[i];
					}

					// name == "" is special case: restore original command text
					// for next level "" (where parentSubcommand==nullptr)
					if (strlen(command->name) == 0 && !parentSubCommand) {
						text = stext;
					}
					return CHAT_COMMAND_OK;
				case CHAT_COMMAND_UNKNOWN:
					// Command not found directly in child command list, return
					// child command list owner
					command = &table[i];
					if (parentCommand)
						*parentCommand = nullptr; // We don't know parent of
									  // table list at this
									  // point

					text = stext; // Restore text to stated just after parse
						      // found parent command
					return CHAT_COMMAND_UNKNOWN_SUBCOMMAND;
				case CHAT_COMMAND_UNKNOWN_SUBCOMMAND:
				default:
					// Some deep subcommand not found, if this second level
					// subcommand then parentCommand can ben nullptr, use know
					// value for it
					if (parentCommand)
						*parentCommand =
						    parentSubCommand ? parentSubCommand : &table[i];
					return res;
			}
		}

		// Unhandled command, skip
		if (!table[i].Handler) {
			continue;
		}

		// Command handler found
		command = &table[i];

		if (parentCommand) {
			*parentCommand = nullptr;
		}

		return CHAT_COMMAND_OK;
	}

	// Command not found in table directly
	command = nullptr;

	// unknoow table owner at this point
	if (parentCommand)
		*parentCommand = nullptr;

	return CHAT_COMMAND_UNKNOWN;
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_list(const std::string &args, GameSession *game_session,
				      std::string &msg)
{
	ChatCommand *cmds = getCommandTable();
	msg += "Command list : \n";
	for (int32_t i = 0; cmds[i].name != nullptr; i++) {
		msg += std::string(cmds[i].name) + "\n";
	}
	return true;
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_help(const std::string &args, GameSession *game_session,
				      std::string &msg)
{
	if (args.empty()) {
		msg = "/help <command> to get the help of the command\n";
		return handle_command_list(args, game_session, msg);
	}

	ChatCommand *command = nullptr;
	ChatCommand *parentCommand = nullptr;
	const char *ctext = args.c_str();
	msg = args;

	ChatCommandSearchResult res =
	    find_command(getCommandTable(), ctext, command, &parentCommand);
	switch (res) {
		case CHAT_COMMAND_OK:
			msg = command->help;
			return true;
		case CHAT_COMMAND_UNKNOWN_SUBCOMMAND: {
			msg = command->help + "\n";
			ChatCommand *child = command->childCommand;
			for (uint16_t i = 0; child[i].name != nullptr; ++i) {
				msg += (std::string) child[i].name + "\n";
				msg += "	" + child[i].help + "\n";
			}
			return true;
		}
		case CHAT_COMMAND_UNKNOWN:
			msg = "Command not found";
			return false;
	}
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_money_add(const std::string &args, GameSession *game_session,
					   std::string &msg)
{
	int nb = std::atoi(args.c_str());
	if (nb == 0) {
		msg = "Argument should be a number.";
		return false;
	}
	game_session->add_money(nb);
	msg = "Adding " + args + " money. Your new money is now " +
	      std::to_string(game_session->get_money());
	return true;
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_money_remove(const std::string &args, GameSession *game_session,
					      std::string &msg)
{
	int nb = std::atoi(args.c_str());
	if (nb == 0) {
		msg = "Argument should be a number.";
		return false;
	}
	game_session->remove_money(nb);
	msg = "Remove " + args + " money. Your new solde is " +
	      std::to_string(game_session->get_money());
	return true;
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_time_speed(const std::string &args, GameSession *game_session,
					    std::string &msg)
{
	if (args.empty()) {
		msg = "Time speed : " + std::to_string(game_session->get_game_speed()) + ".\n";
		msg += "/time speed <nb> for change time speed.";
		return true;
	}

	uint8_t speed = (uint8_t) std::atoi(args.c_str());
	if (speed == 0) {
		msg = "Pause mode.";
		return true;
	} else if (speed < 0 || speed > 100) {
		msg = "Argument should be a number between 0 and 100.";
		return false;
	}
	game_session->set_game_speed(speed);
	msg = "Time speed changed";
	return true;
}

/**
 *
 * @param args
 * @param game_session
 * @param msg
 * @return
 */
bool ChatHandler::handle_command_remove_line(const std::string &args, GameSession *game_session,
					     std::string &msg)
{
	if (args.empty()) {
		msg = "/remove_line <nb>.";
		return true;
	}

	bool res = m_console->get_node(String("RichTextLabel"))
		       ->cast_to<RichTextLabel>()
		       ->remove_line(std::atoi(args.c_str()));
	(res) ? msg = "Line " + std::to_string(std::atoi(args.c_str())) + " has been deleted"
	      : msg = "The line number does not exist.";
	return res;
}

bool ChatHandler::handle_command_add_notif(const std::string &args, GameSession *game_session,
					   std::string &msg)
{
	if (args.empty()) {
		msg = "/notif add 'Title' 'Description'";
		return false;
	}

	std::vector<std::string> args_notif;
	std::string arg = args;
	size_t pos = 0;
	uint8_t i = 0;
	pos = arg.find(" ");
	while (pos != std::string::npos) {
		args_notif.push_back(arg.substr(0, pos));
		arg.erase(0, pos + 1);
		i++;
		if (i >= 2) {
			msg = "/notif add 'Title' 'Description'";
			return false;
		}
		pos = arg.find(" ");
	}

	game_session->add_notification(String(args_notif[0].c_str()), String(arg.c_str()));
	msg = "The notification has been added";
	return true;
}

bool ChatHandler::handle_command_remove_notif(const std::string &args, GameSession *game_session,
						std::string &msg)
{
    if (args.empty()) {
	msg = "/notif remove nb";
	return false;
    }

    uint8_t nb = (uint8_t) std::atoi(args.c_str());

    bool res = game_session->remove_notification(nb);
	(res) ? msg = "The notification has been removed" : msg = "The notification not existe";
    return res;
}


