extends Sprite

var configFile = ConfigFile.new()

const OPTIONMENU = {
	MAIN = 0,
	GRAPHICS = 1,
	SOUND = 2,
	INPUT = 3,
}

var option_config = {
	fullscreen = false,
	music = false,
	sound = false,
}

var current_menu = OPTIONMENU.MAIN

func _ready():
	var err = configFile.load("user://settings.cfg")

	option_config.fullscreen = configFile.get_value("Config", "fullscreen", false)
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			if (current_menu == OPTIONMENU.MAIN):
				goToMainMenu()
			elif (current_menu == OPTIONMENU.GRAPHICS):
				_on_BackGraphicsButton_released()
			elif (current_menu == OPTIONMENU.SOUND):
				_on_BackSoundButton_released()
			elif (current_menu == OPTIONMENU.INPUT):
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

func _on_BackGraphicsButton_released():
	get_node("GraphicsMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_BackSoundButton_released():
	get_node("SoundMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN

func _on_BackInputButton_released():
	get_node("InputMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = OPTIONMENU.MAIN



func _on_CheckBox_toggled( pressed ):
	var is_fullscreen_pressed = get_node("GraphicsMenu/Panel/Container/FullScreenCheck").is_pressed()
	if (is_fullscreen_pressed):
		OS.set_window_fullscreen(true)
		option_config.fullscreen = true
		configFile.set_value("Config", "fullscreen", true)
	else:
		OS.set_window_fullscreen(false)
		option_config.fullscreen = false
		configFile.set_value("Config", "fullscreen", false)

func _on_SaveButton_released():
	configFile.save("user://settings.cfg")
