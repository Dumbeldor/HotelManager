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
#include "gamesession.h"
#include "scene/main/viewport.h"
#include <scene/gui/rich_text_label.h>

static constexpr uint16_t MAX_HISTORY = 100;

Console::Console() : Panel() { m_chat_handler = new ChatHandler(this); }

Console::~Console() { delete m_chat_handler; }

void Console::_bind_methods() { ObjectTypeDB::bind_method("send_command", &Console::send_command); }

/**
 * Add text at the end
 * @param text
 */
void Console::add_text(const std::string &text)
{
	RichTextLabel *rich_text_label =
	    $("RichTextLabel")->cast_to<RichTextLabel>();
	rich_text_label->add_text(String(text.c_str()) + "\n");
	if (rich_text_label->get_line_count() > MAX_HISTORY) {
		uint16_t nb = rich_text_label->get_line_count() - MAX_HISTORY;
		for (uint16_t i = 0; i < nb; i++) {
			rich_text_label->remove_line(0);
		}
	}
}
/**
 * Append green colored sucessful text
 * @param text
 */
void Console::tag_sucess(const std::string &text)
{
	if (text == "")
		return;
	RichTextLabel *rich_text_label = $("RichTextLabel")->cast_to<RichTextLabel>();
	rich_text_label->append_bbcode("[color=#33C73A]" + String(text.c_str()) + "[/color]\n");
}

/**
 * Append red colored error text
 * @param text
 */
void Console::add_error(const std::string &text)
{
	if (text == "")
		return;
	RichTextLabel *rich_text_label = $("RichTextLabel")->cast_to<RichTextLabel>();
	rich_text_label->append_bbcode("[color=red]Error : " + String(text.c_str()) + "[/color]\n");
}

/**
 *
 * @param command
 */
void Console::send_command(const String &command)
{
	add_text(std::string(command.ascii().get_data()));
	std::string msg = "";
	bool res = m_chat_handler->handle_command(
	    std::string(command.ascii().get_data()),
	    get_tree()->get_root()->$("Root/GameSession")->cast_to<GameSession>(), msg);
	(res) ? tag_sucess(msg) : add_error(msg);
}
