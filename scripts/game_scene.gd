
extends Sprite

func _ready():
	var day_label = get_node("ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: 1")
		
	var money_label = get_node("ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text("1000 $")

	set_process(true)

func _process(delta):
	pass