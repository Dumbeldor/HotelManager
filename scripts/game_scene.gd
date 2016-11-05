
extends Node2D

var game_session = GameSession.new()

var current_shown_menu = 0

const GAMEMENU = {
	MAIN = 1,
}

const ZOOMOUT_LIMIT = 8
const ZOOMIN_LIMIT = 0.55

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if event.is_action("ui_cancel"):
			if current_shown_menu == 0:
				# Show game menu
				get_node("Hud/MainMenuLayer/GameMainMenu").show()
				current_shown_menu = GAMEMENU.MAIN
			elif current_shown_menu == GAMEMENU.MAIN:
				_hide_game_menu()
	elif event.type == InputEvent.MOUSE_BUTTON:
		if event.is_action("ui_zoomin"):
			zoom_camera(0.9)
		elif event.is_action("ui_zoomout"):
			zoom_camera(1.11)

func _ready():
	var day_label = get_node("Hud/ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("Hud/ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text(str(game_session.get_money()) + " $")

	set_process_input(true)
	set_process(true)

func _process(delta):
	var should_move_camera = false
	var camera_movement = Vector2(0, 0)
	if Input.is_action_pressed("ui_up"):
		camera_movement.y -= 10
		should_move_camera = true
	if Input.is_action_pressed("ui_down"):
		camera_movement.y += 10
		should_move_camera = true
	if Input.is_action_pressed("ui_left"):
		camera_movement.x -= 10
		should_move_camera = true
	if Input.is_action_pressed("ui_right"):
		camera_movement.x += 10
		should_move_camera = true

	if should_move_camera:
		move_camera(camera_movement)

func move_camera(v):
	var cam = get_node("GameTileMap/Camera2D")
	cam.global_translate(v)

func zoom_camera(multiplier):
	var cam = get_node("GameTileMap/Camera2D")
	var new_zoom = cam.get_zoom() * multiplier
	if new_zoom.x >= ZOOMOUT_LIMIT or new_zoom.y >= ZOOMOUT_LIMIT or new_zoom.x <= ZOOMIN_LIMIT or new_zoom.y <= ZOOMIN_LIMIT:
		return
	cam.set_zoom(new_zoom)

func _on_LeaveGameButton_released():
	get_tree().change_scene("res://scenes/main_menu.tscn")


func _hide_game_menu():
	get_node("Hud/MainMenuLayer/GameMainMenu").hide()
	current_shown_menu = 0
