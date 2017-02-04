extends Node

var current_scene = null
var save_name = ""
var configFile = null
var main_theme = null

func set_save(s):
	save_name = s
	
func get_save():
	return save_name
	
func get_theme():
	return main_theme
	
func _ready():
	var root = get_tree().get_root()
	current_scene = root.get_child(root.get_child_count() - 1)
	configFile = ConfigFile.new()
	var err = configFile.load("user://settings.cfg")
	OS.set_window_fullscreen(configFile.get_value("Graphics", "fullscreen", true))
	
	var f = ResourceLoader.load("res://fonts/teen.fnt")
	main_theme = Theme.new()
	main_theme.set_default_font(f)
