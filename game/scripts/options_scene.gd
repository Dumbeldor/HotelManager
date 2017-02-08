extends Control

var game_config

const OPTIONMENU = {
	MAIN = 0,
	GAME = 1,
	GRAPHICS = 2,
	SOUND = 3,
	INPUT = 4,
}

var option_config = {
	fullscreen = false,
	master_sound = 100,
	music = 100,
	environment_sound = 100,
}

var PROJECT = HotelManagerProject.new()

var current_menu = OPTIONMENU.MAIN

func _ready():
	set_theme(get_node("/root/global").get_theme())

	game_config = get_node("/root/global").game_config
	get_node("ProjectInfos/ProjectNameLabel").set_text(PROJECT.get_project_name())
	get_node("ProjectInfos/ProjectNameLabel/ProjectVersion").set_text(PROJECT.get_full_version())

	option_config.interval_save = game_config.get_value("Game", "interval_save", 300.0)
	option_config.auto_save = game_config.get_value("Game", "auto_save", true)
	option_config.fullscreen = game_config.get_value("Graphics", "fullscreen", true)
	option_config.master_sound = game_config.get_value("Sound", "master_sound", 100)
	option_config.music = game_config.get_value("Sound", "music", 100)
	option_config.environment_sound = game_config.get_value("Sound", "environment_sound", 100)
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			if (current_menu == OPTIONMENU.MAIN):
				get_node("/root/MusicPlayer").button_click()
				goToMainMenu()
			elif (current_menu == OPTIONMENU.GAME):
				get_node("/root/MusicPlayer").button_click()
				_on_BackGameButton_released()
			elif (current_menu == OPTIONMENU.GRAPHICS):
				get_node("/root/MusicPlayer").button_click()
				_on_BackGraphicsButton_released()
			elif (current_menu == OPTIONMENU.SOUND):
				get_node("/root/MusicPlayer").button_click()
				_on_BackSoundButton_released()
			elif (current_menu == OPTIONMENU.INPUT):
				get_node("/root/MusicPlayer").button_click()
				_on_BackInputButton_released()

func goToMainMenu():
	get_tree().change_scene("res://scenes/main_menu.tscn")

func _on_BackButton_released():
	goToMainMenu()

func _on_GameButton_released():
	get_node("OptionsMenu").hide()
	get_node("GameMenu").show()
	current_menu = OPTIONMENU.GAME
	get_node("GameMenu/Panel/Container/IntervalHSlider").set_value(option_config.interval_save / 60)
	get_node("GameMenu/Panel/Container/AutoSaveLabel/AutoSaveCheckBox").set_pressed(option_config.auto_save)

func _on_GraphicsButton_released():
	get_node("OptionsMenu").hide()
	get_node("GraphicsMenu").show()
	current_menu = OPTIONMENU.GRAPHICS
	get_node("GraphicsMenu/Panel/Container/FullScreenCheck").set_pressed(option_config.fullscreen)

func _on_InputButton_released():
	get_node("OptionsMenu").hide()
	get_node("InputMenu").show()
	current_menu = OPTIONMENU.INPUT

func _on_SoundButton_released():
	get_node("OptionsMenu").hide()
	get_node("SoundMenu").show()
	current_menu = OPTIONMENU.SOUND
	get_node("SoundMenu/Panel/Container/MasterSoundSlider").set_value(option_config.master_sound)
	get_node("SoundMenu/Panel/Container/MusicSlider").set_value(option_config.music)
	get_node("SoundMenu/Panel/Container/EnvironmentSoundSlider").set_value(option_config.environment_sound)

func _on_BackGameButton_released():
	game_config.set_auto_save(option_config.auto_save)
	game_config.set_interval_save(option_config.interval_save)
	game_config.save("user://settings.cfg")
	get_node("GameMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_BackGraphicsButton_released():
	get_node("GraphicsMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_BackSoundButton_released():
	get_node("SoundMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN
	game_config.save("user://settings.cfg")

func _on_BackInputButton_released():
	get_node("InputMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_CheckBox_toggled( pressed ):
	option_config.fullscreen = get_node("GraphicsMenu/Panel/Container/FullScreenCheck").is_pressed()
	OS.set_window_fullscreen(option_config.fullscreen)
	game_config.set_value("Graphics", "fullscreen", option_config.fullscreen)
	game_config.save("user://settings.cfg")

#--------------
#Sound Option
#--------------
func _on_MasterSoundSlider_value_changed( value ):
	option_config.master_sound = value
	game_config.set_value("Sound", "master_sound", option_config.master_sound)

func _on_MusicSlider_value_changed( value ):
	get_node("/root/MusicPlayer").set_volume(value)
	option_config.music = value
	game_config.set_value("Sound", "music", option_config.music)

func _on_EnvironmentSoundSlider_value_changed( value ):
	option_config.environment_sound = value
	game_config.set_value("Sound", "environment_sound", option_config.environment_sound)


func _on_HSlider_value_changed( value ):
	get_node("GameMenu/Panel/Container/IntervalHSlider/IntervalNbLabel").set_text(str(value, "mn"))
	option_config.interval_save = value * 60


func _on_AutoSaveCheckBox_toggled( pressed ):
	option_config.auto_save = get_node("GameMenu/Panel/Container/AutoSaveLabel/AutoSaveCheckBox").is_pressed()
	game_config.set_auto_save(option_config.auto_save)
	game_config.set_interval_save(option_config.interval_save)
	game_config.save("user://settings.cfg")
