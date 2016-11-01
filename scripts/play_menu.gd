
extends Control

func _ready():
	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			goToMainMenu()

func _on_BackButton_released():
	goToMainMenu()

func goToMainMenu():
	get_tree().change_scene("res://scenes/main_menu.tscn")