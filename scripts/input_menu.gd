extends Sprite

func _ready():
	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if (event.is_action("ui_cancel")):
			goToMainMenu()

func goToMainMenu():
	get_tree().change_scene("res://scenes/options_menu.tscn")

func _on_BackButton_pressed():
	goToMainMenu()
