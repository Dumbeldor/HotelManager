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
 * 2017, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#pragma once

#include <scene/gui/texture_button.h>

class SelectorButton: public TextureButton
{
	OBJ_TYPE(SelectorButton, TextureButton)

public:
	static void init_selector();
	void _change_selected_object();
	void _on_draw();
protected:
	static void _bind_methods();
	static SelectorButton *s_selected;
};
