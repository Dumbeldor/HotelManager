
extends Node2D

onready var game_session = get_node("GameSession")

var current_shown_menu = 0

const GAMEMENU = {
	MAIN = 1,
}

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if event.is_action("ui_cancel"):
			if current_shown_menu == 0:
				_show_game_menu()
			elif current_shown_menu == GAMEMENU.MAIN:
				_hide_game_menu()

func _ready():
	game_session.init()

	var day_label = get_node("GameSession/GameMap/Hud/ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("GameSession/GameMap/Hud/ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text(str(game_session.get_money()) + " $")

	set_process_input(true)
	set_process(true)

func _process(delta):
	game_session._process(delta)

##
## Button handlers
##

func _on_LeaveGameButton_released():
	get_tree().change_scene("res://scenes/main_menu.tscn")
	get_tree().set_pause(false)

func _show_game_menu():
	get_node("MainMenuLayer/GameMainMenu").show()
	get_tree().set_pause(true)
	current_shown_menu = GAMEMENU.MAIN

func _hide_game_menu():
	get_node("MainMenuLayer/GameMainMenu").hide()
	get_tree().set_pause(false)
	current_shown_menu = 0
