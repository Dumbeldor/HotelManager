
extends Node2D

onready var game_session = get_node("GameSession")

var current_shown_menu = 0
var index_save = null
var save_name = null

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
		#get_node("MainMenuLayer/SaveMenu/SaveList").select(index_save)
		get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/NameSaveEdit").set_text(save_name)

func _on_SaveButton_released():
	get_node("MainMenuLayer/SaveMenu").show()
	list_save()


func _on_SaveLeaveButton_released():
	#get_node("MainMenuLayer/SaveMenu").show()
	#list_save()
	game_session.save("savequit")
	get_tree().change_scene("res://scenes/main_menu.tscn")
	get_tree().set_pause(false)


func _on_BackButton_released():
	get_node("MainMenuLayer/SaveMenu").hide()
	get_node("MainMenuLayer/SaveMenu/BackButton/OverWriteButton").set_disabled(true)
	get_node("MainMenuLayer/SaveMenu/SaveList").clear()
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel").set_text(String(""))
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel/MoneyValueLabel/DateValueLabel").set_text(String(""))
	get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/MoneyLabel").hide()


func _on_SaveList_item_selected( index ):
	get_node("MainMenuLayer/SaveMenu/BackButton/OverWriteButton").set_disabled(false)
	var saveName = get_node("MainMenuLayer/SaveMenu/SaveList").get_item_text(index)
	index_save = index
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
		if dir.file_exists(get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/NameSaveEdit").get_text() + ".save"):
			get_node("MainMenuLayer/SaveMenu/SaveExistConfirmDialog").popup()
			print("Error save exist")
		else:
			save()

func save():
	save_name = get_node("MainMenuLayer/SaveMenu/SaveDetailPanel/NameSaveEdit").get_text()
	game_session.save(save_name)
	_on_BackButton_released()

func _on_SaveExistConfirmDialog_confirmed():
	save()


func _on_OverWriteButton_released():
	get_node("MainMenuLayer/SaveMenu/OverWriteConfirmDialog").popup()

func _on_SaveList_item_activated( index ):
	index_save = index
	get_node("MainMenuLayer/SaveMenu/OverWriteConfirmDialog").popup()


func _on_OverWriteConfirmDialog_confirmed():
	save_name = get_node("MainMenuLayer/SaveMenu/SaveList").get_item_text(index_save)
	game_session.save(save_name)
	_on_BackButton_released()