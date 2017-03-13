extends Control

onready var game_session = get_node("GameSession")

var current_shown_menu = 0
var save_name = null

const GAMEMENU = {
	MAIN = 1,
}

func _input(event):
	if event.type == InputEvent.KEY and event.pressed:
		if event.is_action("ui_cancel"):
			var console = get_node("MainMenuLayer/Console")
			if console.is_visible():
				get_node("MainMenuLayer/Console/ConsoleAnim").play("HideAnim")
			elif current_shown_menu == 0:
				_show_game_menu()
			elif current_shown_menu == GAMEMENU.MAIN:
				_hide_game_menu()
		elif event.is_action("ui_achievement_panel"):
			var ap = get_node("GameSession/GameMap/Hud/AchievementPopup")
			if ap.is_visible():
				ap.hide()
			else:
				ap.show()
		elif event.is_action("ui_console"):
			var console = get_node("MainMenuLayer/Console")
			if console.is_visible():
				get_node("MainMenuLayer/Console/ConsoleAnim").play("HideAnim")
			else:
				console.show()
				get_node("MainMenuLayer/Console/ConsoleAnim").play("ShowAnim")
				get_node("MainMenuLayer/Console/ConsoleLineEdit").grab_focus()

func _ready():
	set_theme(get_node("/root/global").get_theme())
	get_node("MainMenuLayer/GameMainMenu").set_theme(get_node("/root/global").get_theme())
	get_node("MainMenuLayer/SaveMenu").set_theme(get_node("/root/global").get_theme())
    # TODO, get save name and call it from there
	game_session.init(get_node("/root/global").get_save())
	set_process_input(true)
	set_process(true)
	#Activate scroll console
	get_node("MainMenuLayer/Console/RichTextLabel").set_scroll_follow(true)

	get_node("GameSession/GameMap/Hud").initialize()
	
	game_session.add_notification("Good Game", "Welcome in Hotel Manager !", "res://icons/notification.png")

func _process(delta):
	game_session._process(delta)

##
## Button handlers
##

func goto_main_menu():
	get_node("/root/global").set_save("")
	get_tree().change_scene("res://scenes/main_menu.tscn")
	get_tree().set_pause(false)

func _on_LeaveGameButton_released():
	goto_main_menu()

func _show_game_menu():
	get_node("MainMenuLayer/GameMainMenu").show()
	get_tree().set_pause(true)
	current_shown_menu = GAMEMENU.MAIN

func _hide_game_menu():
	get_node("MainMenuLayer/GameMainMenu").hide()
	get_tree().set_pause(false)
	current_shown_menu = 0

func list_save():
	var dir  = Directory.new()
	if dir.open("user://save/") == OK:
		dir.list_dir_begin()
		var file_name = dir.get_next()
		while (file_name != ""):
			if !dir.current_is_dir():
				var name = file_name.split(".")
				get_node("MainMenuLayer/SaveMenu/SaveList").add_item(name[0])
			file_name = dir.get_next()
	else:
		if dir.open("user://") == OK:
			if dir.make_dir("save") != OK:
				print("Error create dir : user://save")
		else:
			print("An error occured when trying to access the path.")
	if save_name != null:
		get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/NewSaveButton/NameSaveEdit").set_text(save_name)

func _on_SaveButton_released():
	get_node("MainMenuLayer/SaveMenu").show()
	list_save()


func _on_SaveLeaveButton_released():
	#get_node("MainMenuLayer/SaveMenu").show()
	#list_save()
	game_session.save("savequit")
	goto_main_menu()


func _on_BackButton_released():
	get_node("MainMenuLayer/SaveMenu").hide()
	get_node("MainMenuLayer/SaveMenu/SaveList").clear()
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel").set_text(String(""))
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel/DateValueLabel").set_text(String(""))
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel").hide()


func _on_SaveList_item_selected( index ):
	var saveName = get_node("MainMenuLayer/SaveMenu/SaveList").get_item_text(index)
	var saveGame = File.new()
	if !saveGame.file_exists("user://save/"+saveName+".save"):
		print("Error file don't exist")
		return
	saveGame.open("user://save/"+saveName+".save", File.READ)
	var currentLine = {}
	currentLine.parse_json(saveGame.get_line())
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel").show()
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel").set_text(String(currentLine["money"]))
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel/DateValueLabel").set_text(String(currentLine["timestamp"]))


func _on_NewSaveButton_released():
	var dir = Directory.new()
	if dir.open("user://save/") != OK:
		if dir.open("user://") == OK:
			dir.make_dir("save")
	if dir.open("user://save/") == OK:
		save_name = get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/NewSaveButton/NameSaveEdit").get_text()
		if dir.file_exists(save_name + ".save"):
			get_node("MainMenuLayer/SaveMenu/OverWriteConfirmDialog").popup()
		else:
			save()

func save():
	game_session.save(save_name)
	_on_BackButton_released()
	_hide_game_menu()

func _on_SaveList_item_activated( index ):
	save_name = get_node("MainMenuLayer/SaveMenu/SaveList").get_item_text(index)
	get_node("MainMenuLayer/SaveMenu/OverWriteConfirmDialog").popup()

func _on_APButton_pressed():
	get_node("GameSession/GameMap/Hud/AchievementPopup").hide()

func _on_ConsoleLineEdit_text_entered( text ):
	send_command(text)

func send_command(text):
	get_node("MainMenuLayer/Console").send_command(text)
	get_node("MainMenuLayer/Console/ConsoleLineEdit").set_text("")

func _on_SendCommandButton_released():
	send_command(get_node("MainMenuLayer/Console/ConsoleLineEdit").get_text())

func _on_ClockSpeed1_pressed():
	get_node("GameSession").set_game_speed(1)

func _on_ClockSpeed2_pressed():
	get_node("GameSession").set_game_speed(2)

func _on_ClockSpeedPause_pressed():
	get_node("GameSession").set_game_speed(0)

func _on_ClockSpeed3_pressed():
	get_node("GameSession").set_game_speed(3)

func _on_ClockSpeed5_pressed():
	get_node("GameSession").set_game_speed(5)


func _on_ConsoleAnim_finished():
	var console = get_node("MainMenuLayer/Console")
	if get_node("MainMenuLayer/Console/ConsoleAnim").get_current_animation() != "ShowAnim":
		if console.is_visible():
			console.hide()

