/*************************************************************************/
/*  world_2d.h                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef WORLD_2D_H
#define WORLD_2D_H

#include "resource.h"
#include "servers/physics_2d_server.h"
#include "globals.h"

class SpatialIndexer2D;
class VisibilityNotifier2D;
class Viewport;

class World2D : public Resource {

	OBJ_TYPE( World2D, Resource );

	RID canvas;
	RID space;
	RID sound_space;

	SpatialIndexer2D* indexer;
protected:

	static void _bind_methods();
friend class Viewport;
friend class VisibilityNotifier2D;

	void _register_viewport(Viewport* p_viewport,const Rect2& p_rect);
	void _update_viewport(Viewport* p_viewport,const Rect2& p_rect);
	void _remove_viewport(Viewport* p_viewport);

	void _register_notifier(VisibilityNotifier2D* p_notifier,const Rect2& p_rect);
	void _update_notifier(VisibilityNotifier2D* p_notifier,const Rect2& p_rect);
	void _remove_notifier(VisibilityNotifier2D* p_notifier);

	void _update();
public:



	RID get_canvas();
	RID get_space();
	RID get_sound_space();

	Physics2DDirectSpaceState *get_direct_space_state();

	World2D();
	~World2D();
};

#endif // WORLD_2D_H
