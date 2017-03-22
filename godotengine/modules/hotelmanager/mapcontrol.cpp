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

#include "mapcontrol.h"
#include "gamemap.h"
#include <scene/gui/label.h>

#define LABEL_COST_NODE "SelectorCost"
#define LABEL_WIDTH_NODE "SelectorWidth"
#define LABEL_HEIGHT_NODE "SelectorHeight"

MapControl::MapControl() {}

/**
 * Init MapControl UI element
 *
 * @param map
 */
void MapControl::init(GameMap *map)
{
	if (!is_connected("draw", map, "_canvas_draw")) {
		connect("draw", map, "_canvas_draw");
	}

	if (!is_connected("mouse_enter", map, "_canvas_mouse_enter")) {
		connect("mouse_enter", map, "_canvas_mouse_enter");
	}

	if (!is_connected("mouse_exit", map, "_canvas_mouse_exit")) {
		connect("mouse_exit", map, "_canvas_mouse_exit");
	}

	m_label_cost = $(LABEL_COST_NODE)->cast_to<Label>();
	m_label_width = $(LABEL_WIDTH_NODE)->cast_to<Label>();
	m_label_height = $(LABEL_HEIGHT_NODE)->cast_to<Label>();
	assert(m_label_cost && m_label_height && m_label_width);
}

/**
 * Draw selection mask
 *
 * @param si
 */
void MapControl::draw_selection(const MapSelectionInfos &si)
{
	assert(si.endpoints);

	// Hovering lines
	draw_borders(si.endpoints, si.border_color);

	// Dimension lines
	if (si.selection_in_progress) {
		if (si.area_height > 1) {
			draw_height_line(si.endpoints[0], si.endpoints[2], si.selection_color);

			m_label_height->set_text(String::num(si.area_height) + " m");
			m_label_height->set_pos(
			    Point2i((int) si.endpoints[1].x + 25,
				    (int) (si.endpoints[1].y + si.endpoints[3].y -
					   m_label_height->get_size().y) /
					2));
			m_label_height->add_color_override("font_color", si.selection_color);
			m_label_height->show();
		} else {
			m_label_height->hide();
		}

		if (si.area_width > 1) {
			draw_width_line(si.endpoints[0], si.endpoints[2], si.selection_color);

			m_label_width->set_text(String::num(si.area_width) + " m");
			m_label_width->set_pos(
			    Point2i((int) (si.endpoints[0].x + si.endpoints[1].x -
					   m_label_width->get_size().x) /
					2,
				    (int) si.endpoints[2].y + 25));
			m_label_width->add_color_override("font_color", si.selection_color);
			m_label_width->show();
		} else {
			m_label_width->hide();
		}

		if (si.cost) {
			m_label_cost->set_text(String::num(si.cost) + " $");
			m_label_cost->set_pos(
			    (si.endpoints[0] + si.endpoints[2] - m_label_cost->get_size()) / 2);
			m_label_cost->add_color_override("font_color", si.selection_color);
			m_label_cost->show();
		}
	} else {
		m_label_cost->hide();
		m_label_height->hide();
		m_label_width->hide();
	}

	// Hovering rectangle
	if (si.tile_hovering) {
		Vector<Vector2> points;
		for (uint8_t i = 0; i < 4; i++) {
			points.push_back(si.endpoints[i]);
		}

		draw_colored_polygon(points, si.selection_color);
	}
}
/**
 * Draw height indicator when selecting an area
 *
 * @param start_point
 * @param end_point
 */
void MapControl::draw_height_line(const Vector2 start_point, const Vector2 end_point,
				  const Color &color)
{
	// Draw height line
	draw_line(Vector2(end_point.x + 15, start_point.y), Vector2(end_point.x + 15, end_point.y),
		  color, 2);
	draw_line(Vector2(end_point.x + 7, start_point.y), Vector2(end_point.x + 23, start_point.y),
		  color, 2);
	draw_line(Vector2(end_point.x + 7, end_point.y), Vector2(end_point.x + 23, end_point.y),
		  color, 2);
}

/**
 *
 * @param start_point
 * @param end_point
 * @param color
 */
void MapControl::draw_width_line(const Vector2 start_point, const Vector2 end_point,
				 const Color &color)
{
	draw_line(Vector2(start_point.x, end_point.y + 15), Vector2(end_point.x, end_point.y + 15),
		  color, 2);
	draw_line(Vector2(start_point.x, end_point.y + 7), Vector2(start_point.x, end_point.y + 23),
		  color, 2);
	draw_line(Vector2(end_point.x, end_point.y + 7), Vector2(end_point.x, end_point.y + 23),
		  color, 2);
}

/**
 *
 * @param endpoints
 * @param color
 */
void MapControl::draw_borders(const Vector2 *endpoints, const Color &color)
{
	for (uint8_t i = 0; i < 4; i++) {
		draw_line(endpoints[i], endpoints[(i + 1) % 4], color, 3);
	}
}
