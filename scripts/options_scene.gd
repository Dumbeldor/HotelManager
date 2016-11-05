extends Sprite

const MAINMENU = {
	MAIN = 0,
	GRAPHICS = 1,
	SOUND = 2,
	INPUT = 3,
}

var current_menu = MAINMENU.MAIN

func _ready():
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			if (current_menu == MAINMENU.MAIN):
				goToMainMenu()
			elif (current_menu == MAINMENU.GRAPHICS):
				_on_BackGraphicsButton_released()
			elif (current_menu == MAINMENU.SOUND):
				_on_BackSoundButton_released()
			elif (current_menu == MAINMENU.INPUT):
				_on_BackInputButton_released()

func goToMainMenu():
	get_tree().change_scene("res://scenes/main_menu.tscn")

func _on_BackButton_released():
	goToMainMenu()

func _on_GraphicsButton_released():
	get_node("OptionsMenu").hide()
	get_node("GraphicsMenu").show()
	current_menu = MAINMENU.GRAPHICS

func _on_InputButton_released():
	get_node("OptionsMenu").hide()
	get_node("InputMenu").show()
	current_menu = MAINMENU.INPUT

func _on_SoundButton_released():
	get_node("OptionsMenu").hide()
	get_node("SoundMenu").show()
	current_menu = MAINMENU.SOUND

func _on_BackGraphicsButton_released():
	get_node("GraphicsMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = MAINMENU.MAIN

func _on_BackSoundButton_released():
	get_node("SoundMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = MAINMENU.MAIN

func _on_BackInputButton_released():
	get_node("InputMenu").hide()
	get_node("OptionsMenu").show()
	current_menu = MAINMENU.MAIN

