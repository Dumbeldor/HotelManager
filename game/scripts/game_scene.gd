
extends Node2D

var game_session = GameSession.new()

var current_shown_menu = 0

const GAMEMENU = {
	MAIN = 1,
}

const ZOOMOUT_LIMIT = 8
const ZOOMIN_LIMIT = 0.55
const CAMERA_LIMIT_X = 18000
const CAMERA_LIMIT_Y = CAMERA_LIMIT_X * 1080 / 1920

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
	elif event.type == InputEvent.MOUSE_BUTTON:
		if event.is_action("ui_zoomin"):
			zoom_camera(0.9)
		elif event.is_action("ui_zoomout"):
			zoom_camera(1.11)

func _ready():
	var cam = get_cam()
	cam.set_limit(MARGIN_BOTTOM, CAMERA_LIMIT_Y)
	cam.set_limit(MARGIN_TOP, -CAMERA_LIMIT_Y)
	cam.set_limit(MARGIN_LEFT, -CAMERA_LIMIT_X)
	cam.set_limit(MARGIN_RIGHT, CAMERA_LIMIT_X)

	init_map()

	get_node("GameMap/Hud/ControlPane_Bottom/Grounds").init()

	var day_label = get_node("GameMap/Hud/ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("GameMap/Hud/ControlPane_Top/DayLabel/MoneyLabel")
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

func zoom_camera(multiplier):
	var cam = get_cam()
	var new_zoom = cam.get_zoom() * multiplier
	if new_zoom.x >= ZOOMOUT_LIMIT or new_zoom.y >= ZOOMOUT_LIMIT or new_zoom.x <= ZOOMIN_LIMIT or new_zoom.y <= ZOOMIN_LIMIT:
		return
	cam.set_zoom(new_zoom)

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

func _on_TileMap_input_event(event):
	if event.type == InputEvent.MOUSE_BUTTON and event.button_index == BUTTON_LEFT:
		if event.is_pressed():
			input_mouse_pressed_on_gamemap = true
			#get_map().handle_event_mouse_click(event.pos)
		else:
			input_mouse_pressed_on_gamemap = false
	elif event.type == InputEvent.MOUSE_MOTION and input_mouse_pressed_on_gamemap:
		pass
		#get_map().handle_event_mouse_click(event.pos)

