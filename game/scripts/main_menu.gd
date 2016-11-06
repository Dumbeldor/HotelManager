
extends Sprite

var PROJECT = HotelManagerProject.new()

const MAINMENU = {
	MAIN = 0,
	PLAY = 1,
}

var current_menu = MAINMENU.MAIN

func _ready():
	get_node("ProjectInfos/ProjectNameLabel").set_text(PROJECT.get_project_name())
	get_node("ProjectInfos/ProjectNameLabel/ProjectVersion").set_text(PROJECT.get_full_version())

	# Launch scene in full screen mode
	var configFile = ConfigFile.new()
	var err = configFile.load("user://settings.cfg")

	var fullscreen = configFile.get_value("Config", "fullscreen", true)

	OS.set_window_fullscreen(fullscreen)
	OS.set_window_title(PROJECT.get_project_name())

	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if (event.type == InputEvent.KEY and event.pressed):
		if (event.is_action("ui_cancel")):
			if (current_menu == MAINMENU.MAIN):
				quit()
			elif (current_menu == MAINMENU.PLAY):
				_on_PlayBackButton_released()

func quit():
	get_tree().quit()

func _on_QuitButton_released():
	get_tree().quit()

func _on_PlayButton_released():
	get_node("MainMenu").hide()
	get_node("PlayMenu").show()
	current_menu = MAINMENU.PLAY

func _on_PlayBackButton_released():
	get_node("PlayMenu").hide()
	get_node("MainMenu").show()
	current_menu = MAINMENU.MAIN

func _on_NewGameButton_released():
	get_tree().change_scene("res://scenes/game_scene.tscn")


func _on_OptionsButton_released():
	get_tree().change_scene("res://scenes/options_scene.tscn")
