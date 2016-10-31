
extends Sprite

func _ready():
	# Launch scene in full screen mode
	OS.set_window_fullscreen(true)

	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if (event.type == InputEvent.KEY):
		if (event.is_action("ui_cancel")):
			quit()

func quit():
	get_tree().quit()
