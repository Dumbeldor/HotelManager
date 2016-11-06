
extends GameMap

func _ready():
	set_process_input(true)

var input_mouse_pressed = false

func _input(event):
	if event.type == InputEvent.MOUSE_BUTTON and event.button_index == BUTTON_LEFT:
		if event.is_pressed():
			input_mouse_pressed = true
			handle_event_mouse_click(event.pos)
		else:
			input_mouse_pressed = false
	elif event.type == InputEvent.MOUSE_MOTION and input_mouse_pressed:
		handle_event_mouse_click(event.pos)