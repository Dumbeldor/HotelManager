
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
const WORLD_LIMIT_X = 180
const WORLD_LIMIT_Y = WORLD_LIMIT_X * 1080 / 1920

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
	var cam = get_cam()
	cam.set_limit(MARGIN_BOTTOM, CAMERA_LIMIT_Y)
	cam.set_limit(MARGIN_TOP, -CAMERA_LIMIT_Y)
	cam.set_limit(MARGIN_LEFT, -CAMERA_LIMIT_X)
	cam.set_limit(MARGIN_RIGHT, CAMERA_LIMIT_X)

	var day_label = get_node("Hud/ControlPane_Top/DayLabel")
	if day_label:
		day_label.set_text("Day: " + str(game_session.get_current_day()))

	var money_label = get_node("Hud/ControlPane_Top/DayLabel/MoneyLabel")
	if money_label:
		money_label.set_text(str(game_session.get_money()) + " $")

	var tilemap = get_node("GameTileMap");
	for i in range(-WORLD_LIMIT_X, WORLD_LIMIT_X):
		tilemap.set_cell(i, WORLD_LIMIT_Y, 0)
		tilemap.set_cell(i, -WORLD_LIMIT_Y, 0)

	for i in range(-WORLD_LIMIT_Y, WORLD_LIMIT_Y):
		tilemap.set_cell(WORLD_LIMIT_X, i, 0)
		tilemap.set_cell(-WORLD_LIMIT_X, i, 0)
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
	var cam = get_cam()
	cam.global_translate(v)

func zoom_camera(multiplier):
	var cam = get_cam()
	var new_zoom = cam.get_zoom() * multiplier
	if new_zoom.x >= ZOOMOUT_LIMIT or new_zoom.y >= ZOOMOUT_LIMIT or new_zoom.x <= ZOOMIN_LIMIT or new_zoom.y <= ZOOMIN_LIMIT:
		return
	cam.set_zoom(new_zoom)

func _on_LeaveGameButton_released():
	get_tree().change_scene("res://scenes/main_menu.tscn")

func _hide_game_menu():
	get_node("Hud/MainMenuLayer/GameMainMenu").hide()
	current_shown_menu = 0

# node getters
func get_cam():
	return get_node("GameTileMap/Camera2D")