extends Node

var current_scene = null
var save_name = ""
var wait_frames
var loader
var game_config = null
var main_theme = null
var time_max = 30 # mse
var loading

func set_save(s):
	save_name = s
	
func get_save():
	return save_name
	
func get_theme():
	return main_theme

func _ready():
	var root = get_tree().get_root()
	current_scene = root.get_child(root.get_child_count() - 1)
	game_config = GameConfig.new()
	var err = game_config.load("user://settings.cfg")
	OS.set_window_fullscreen(game_config.get_value("Graphics", "fullscreen", true))

	var f = ResourceLoader.load("res://fonts/teen.fnt")
	main_theme = Theme.new()
	main_theme.set_default_font(f)

func goto_scene(path):
	loader = ResourceLoader.load_interactive(path)
	loading = ResourceLoader.load("res://scenes/loading.tscn");
	if loader == null:
		show_error()
		return
	set_process(true)
	
	current_scene.queue_free()
	
	wait_frames = 1
	set_new_scene(loading)

func _process(time):
	if loader == null:
		set_process(false)
		return
	
	if wait_frames > 0:
		wait_frames -= 1
		return
	
	var t = OS.get_ticks_msec()
	while OS.get_ticks_msec() < t + time_max:
		var err = loader.poll()
		
		if err == ERR_FILE_EOF:
			var resource = loader.get_resource()
			loader = null
			current_scene.queue_free()
			set_new_scene(resource)
			break
		elif err == OK:
			update_progress()
		else:
			show_error()
			loader = null
			break

func update_progress():
	var progress = float(loader.get_stage()) / loader.get_stage_count()
	current_scene.get_node("ProgressBar").set_value(progress)


func set_new_scene(scene_resource):
	current_scene = scene_resource.instance()
	get_node("/root").add_child(current_scene)

func _deferred_goto_scene(path):
	current_scene.free()
	var s = ResourceLoader.load(path)
	current_scene = s.instance()
	get_tree().get_root().add_child(current_scene)
	get_tree().set_current_scene( current_scene )