
extends Sprite

func _ready():
	var day_label = get_node("Control/DayLabel")
	if day_label:
		day_label.set_text("Day: 1")
