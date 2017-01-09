extends Node

var current_scene = null
var save_name = ""

func set_save(s):
	save_name = s
	
func get_save():
	return save_name
	
func _ready():
        var root = get_tree().get_root()
        current_scene = root.get_child(root.get_child_count() - 1)