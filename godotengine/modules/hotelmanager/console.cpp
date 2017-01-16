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

#include "console.h"
#include "chathandler.h"
#include <scene/gui/rich_text_label.h>
#include "scene/main/viewport.h"
#include "gamesession.h"

static constexpr uint16_t MAX_HISTORY = 100;

Console::Console() : Panel()
{
	m_chat_handler = new ChatHandler(this);
}

Console::~Console()
{
	delete m_chat_handler;
}

void Console::_bind_methods()
{
	ObjectTypeDB::bind_method("send_command", &Console::send_command);
}

void Console::add_text(const std::string &text)
{
	std::cout << "Nombre dans le texte : " << get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->get_total_character_count() << std::endl;
	get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->add_text(String(text.c_str()) + "\n");
	if (get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->get_total_character_count() > MAX_HISTORY) {
		std::cout << "ENLEVEMENT D'UNE LIGNE" << std::endl;
		get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->remove_line(0);
	}
}

void Console::tag_sucess(const std::string &text)
{
	get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->append_bbcode("[color=#33C73A]" + String(text.c_str()) + "[/color]\n");
}

void Console::add_error(const std::string &text)
{
	get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->append_bbcode("[color=red]Error : " + String(text.c_str()) + "[/color]\n");
}

void Console::send_command(const String &command)
{
	get_node(String("RichTextLabel"))->cast_to<RichTextLabel>()->add_text(command + "\n");
	std::string msg = "";
	bool res = m_chat_handler->handle_command(std::string(command.ascii().get_data()), get_tree()->get_root()->get_node(String("Root/GameSession"))->cast_to<GameSession>(), msg);
	(res) ? tag_sucess(msg) : add_error(msg);
}