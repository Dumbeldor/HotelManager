
extends Node2D

var game_session = GameSession.new()

var current_shown_menu = 0

const GAMEMENU = {
	MAIN = 1,
}

var WORLD_LIMITS = Vector2(0, 0)

const TILES = {
	EMPTY = 0,
	GRASS = 1,
}

var input_mouse_pressed_on_gamemap = false

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if event.is_action("ui_cancel"):
			if current_shown_menu == 0:
				_show_game_menu()
			elif current_shown_menu == GAMEMENU.MAIN:
				_hide_game_menu()

func _ready():
	init_map()

	var day_label = get_node("GameMap/Hud/ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("GameMap/Hud/ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text(str(game_session.get_money()) + " $")

	set_process_input(true)
	set_process(true)

func _process(delta):
	get_map()._process(delta)
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

##
## CAMERA
##

func get_cam():
	return get_node("GameMap/TileMap/Camera2D")

func move_camera(v):
	var cam = get_cam()
	var zoom = cam.get_zoom()
	v.x *= zoom.x * 0.65
	v.y *= zoom.y * 0.5
	cam.global_translate(v)

##
## MAP
##

func get_map():
	return get_node("GameMap")

func init_map():
	var map = get_map();
	map.init()
	# Load some informations from cpp
	WORLD_LIMITS = map.get_world_limits()

