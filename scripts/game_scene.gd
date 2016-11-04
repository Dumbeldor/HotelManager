
extends Sprite

var game_session = GameSession.new()

func _ready():
	var day_label = get_node("ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text(str(game_session.get_money()) + " $")

	set_process(true)

func _process(delta):
	var cam = get_node("GameTileMap/Camera2D")
	cam.global_translate(Vector2(delta * 100, 0))