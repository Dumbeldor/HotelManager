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

#include <iostream>
#include <scene/gui/panel.h>

class RichTextLabel;
class ChatHandler;

class Console : public Panel
{
	OBJ_TYPE(Console, Panel);

public:
	Console();
	~Console();
	void add_text(const std::string &text);
	void tag_sucess(const std::string &text);
	void add_error(const std::string &text);

protected:
	static void _bind_methods();

	void send_command(const String &command);
	ChatHandler *m_chat_handler = nullptr;
};
