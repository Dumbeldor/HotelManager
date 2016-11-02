
extends Sprite

var PROJECT = HotelManagerProject.new()

func _ready():
	# Launch scene in full screen mode
	OS.set_window_fullscreen(true)
	OS.set_window_title(PROJECT.get_project_name())

	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if (event.type == InputEvent.KEY and event.pressed):
		if (event.is_action("ui_cancel")):
			quit()

func quit():
	get_tree().quit()

func _on_QuitButton_released():
	get_tree().quit()


func _on_ProjectNameLabel_draw():
	get_node("ProjectInfos/ProjectNameLabel").set_text(PROJECT.get_project_name())

func _on_ProjectVersion_draw():
	get_node("ProjectInfos/ProjectNameLabel/ProjectVersion").set_text(PROJECT.get_full_version())


func _on_PlayButton_released():
	get_tree().change_scene("res://scenes/play_menu.tscn")
