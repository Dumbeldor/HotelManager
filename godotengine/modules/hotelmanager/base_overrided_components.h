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

#include <scene/gui/button.h>

class HMButton : public Button
{
	OBJ_TYPE(HMButton, Button)

public:
	HMButton(const String &p_text = String());
	~HMButton() {}
protected:
	virtual void pressed();
};
