
extends Button

func _ready():
	set_process_input(true)
	
func _input_event(event):
	if event.type == InputEvent.MOUSE_BUTTON and event.button_index == BUTTON_LEFT and event.pressed:
		get_tree().quit()