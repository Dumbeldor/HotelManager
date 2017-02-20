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

#define OBJECTSELECTOR_SIZE 48

class SelectorButton: public TextureButton
{
	OBJ_TYPE(SelectorButton, TextureButton)

public:
	SelectorButton();

	static void init_selector();
protected:
	void _change_selected_object();
	void _on_draw();

	static void _bind_methods();
	static SelectorButton *s_selected;
};

/*
 * Tile Selector
 */
class TileSelectorButton: public SelectorButton
{
	OBJ_TYPE(TileSelectorButton, SelectorButton)
public:
	TileSelectorButton(const uint32_t tile_id);

	uint32_t get_map_tile() const { return m_tile_id; }

	static const uint32_t get_selected_tile_id()
	{
		TileSelectorButton *ts = s_selected->cast_to<TileSelectorButton>();
		return ts ? ts->get_map_tile() : 0;
	}

private:
	TileSelectorButton(): SelectorButton(), m_tile_id(0) {}
	const uint32_t m_tile_id;
};

/*
 * NPC Selector
 */
class NPCSelectorButton: public SelectorButton
{
	OBJ_TYPE(NPCSelectorButton, SelectorButton)
public:
	NPCSelectorButton(const uint32_t obj_id);
private:
	NPCSelectorButton(): SelectorButton(), m_npc_id(0) {}
	const uint32_t m_npc_id;
};
