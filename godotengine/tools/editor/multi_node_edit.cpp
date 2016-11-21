/*************************************************************************/
/*  multi_node_edit.cpp                                                  */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
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
#include "multi_node_edit.h"
#include "editor_node.h"

bool MultiNodeEdit::_set(const StringName& p_name, const Variant& p_value){

	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return false;

	String name = p_name;

	if (name=="scripts/script") { // script/script set is intercepted at object level (check Variant Object::get() ) ,so use a different name
		name="script/script";
	}

	UndoRedo *ur=EditorNode::get_singleton()->get_undo_redo();

	ur->create_action(TTR("MultiNode Set")+" "+String(name));
	for (const List<NodePath>::Element *E=nodes.front();E;E=E->next()) {

		if (!es->has_node(E->get()))
			continue;

		Node*n=es->get_node(E->get());
		if (!n)
			continue;

		if (p_value.get_type() == Variant::NODE_PATH) {
			Node *tonode = n->get_node(p_value);
			NodePath p_path = n->get_path_to(tonode);
			ur->add_do_property(n,name,p_path);
		} else {
			ur->add_do_property(n,name,p_value);
		}

		ur->add_undo_property(n,name,n->get(name));


	}
	ur->add_do_method(EditorNode::get_singleton()->get_property_editor(),"refresh");
	ur->add_undo_method(EditorNode::get_singleton()->get_property_editor(),"refresh");

	ur->commit_action();
	return true;
}

bool MultiNodeEdit::_get(const StringName& p_name,Variant &r_ret) const {

	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return false;

	String name=p_name;
	if (name=="scripts/script") { // script/script set is intercepted at object level (check Variant Object::get() ) ,so use a different name
		name="script/script";
	}

	for (const List<NodePath>::Element *E=nodes.front();E;E=E->next()) {

		if (!es->has_node(E->get()))
			continue;

		const Node*n=es->get_node(E->get());
		if (!n)
			continue;

		bool found;
		r_ret=n->get(name,&found);
		if (found)
			return true;

	}

	return false;
}

void MultiNodeEdit::_get_property_list( List<PropertyInfo> *p_list) const{

	HashMap<String,PLData> usage;

	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return;

	int nc=0;

	List<PLData*> datas;

	for (const List<NodePath>::Element *E=nodes.front();E;E=E->next()) {

		if (!es->has_node(E->get()))
			continue;

		Node*n=es->get_node(E->get());
		if (!n)
			continue;

		List<PropertyInfo> plist;
		n->get_property_list(&plist,true);

		for(List<PropertyInfo>::Element *F=plist.front();F;F=F->next()) {

			if (F->get().name=="script/script")
				continue; //added later manually, since this is intercepted before being set (check Variant Object::get() )
			if (!usage.has(F->get().name)) {
				PLData pld;
				pld.uses=0;
				pld.info=F->get();
				usage[F->get().name]=pld;
				datas.push_back(usage.getptr(F->get().name));
			}

			usage[F->get().name].uses++;
		}

		nc++;
	}

	for (List<PLData*>::Element *E=datas.front();E;E=E->next()) {

		if (nc==E->get()->uses) {
			p_list->push_back(E->get()->info);
		}
	}

	p_list->push_back(PropertyInfo(Variant::OBJECT,"scripts/script",PROPERTY_HINT_RESOURCE_TYPE,"Script"));


}

void MultiNodeEdit::clear_nodes() {

	nodes.clear();
}

void MultiNodeEdit::add_node(const NodePath& p_node){

	nodes.push_back(p_node);
}

MultiNodeEdit::MultiNodeEdit()
{
}
