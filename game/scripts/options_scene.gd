extends Control

var configFile

const OPTIONMENU = {
	MAIN = 0,
	GRAPHICS = 1,
	SOUND = 2,
	INPUT = 3,
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
	
	configFile = get_node("/root/global").configFile
	get_node("ProjectInfos/ProjectNameLabel").set_text(PROJECT.get_project_name())
	get_node("ProjectInfos/ProjectNameLabel/ProjectVersion").set_text(PROJECT.get_full_version())

	option_config.fullscreen = configFile.get_value("Graphics", "fullscreen", true)
	option_config.master_sound = configFile.get_value("Sound", "master_sound", 100)
	option_config.music = configFile.get_value("Sound", "music", 100)
	option_config.environment_sound = configFile.get_value("Sound", "environment_sound", 100)
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			if (current_menu == OPTIONMENU.MAIN):
				get_node("/root/MusicPlayer").button_click()
				goToMainMenu()
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

func _on_BackGraphicsButton_released():
	get_node("GraphicsMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_BackSoundButton_released():
	get_node("SoundMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN
	configFile.save("user://settings.cfg")


func _on_BackInputButton_released():
	get_node("InputMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_CheckBox_toggled( pressed ):
	option_config.fullscreen = get_node("GraphicsMenu/Panel/Container/FullScreenCheck").is_pressed()
	OS.set_window_fullscreen(option_config.fullscreen)
	configFile.set_value("Graphics", "fullscreen", option_config.fullscreen)
	configFile.save("user://settings.cfg")

#--------------
#Sound Option
#--------------
func _on_MasterSoundSlider_value_changed( value ):
	option_config.master_sound = value
	configFile.set_value("Sound", "master_sound", option_config.master_sound)

func _on_MusicSlider_value_changed( value ):
	get_node("/root/MusicPlayer").set_volume(value)
	option_config.music = value
	configFile.set_value("Sound", "music", option_config.music)

func _on_EnvironmentSoundSlider_value_changed( value ):
	option_config.environment_sound = value
	configFile.set_value("Sound", "environment_sound", option_config.environment_sound)
