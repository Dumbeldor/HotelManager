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
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include "base_overrided_components.h"

HMButton::HMButton(const String &p_text) : Button(p_text) {}

void HMButton::pressed()
{
	Button::pressed();
	$("/root/MusicPlayer")->call("button_click");
}
