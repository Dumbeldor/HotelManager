extends Sprite

var PROJECT = HotelManagerProject.new()
var indexSave = 0;

const MAINMENU = {
	MAIN = 0,
	PLAY = 1,
}

var current_menu = MAINMENU.MAIN

func _ready():
	get_node("ProjectInfos/ProjectNameLabel").set_text(PROJECT.get_project_name())
	get_node("ProjectInfos/ProjectNameLabel/ProjectVersion").set_text(PROJECT.get_full_version())

	# Launch scene in full screen mode
	var configFile = ConfigFile.new()
	var err = configFile.load("user://settings.cfg")

	var fullscreen = configFile.get_value("Graphics", "fullscreen", true)

	OS.set_window_fullscreen(fullscreen)
	OS.set_window_title(PROJECT.get_project_name())

	# Tell we accept input key
	set_process_input(true)

func _input(event):
	if (event.type == InputEvent.KEY and event.pressed):
		if (event.is_action("ui_cancel")):
			if (current_menu == MAINMENU.MAIN):
				quit()
			elif (current_menu == MAINMENU.PLAY):
				_on_PlayBackButton_released()

func quit():
	get_tree().quit()

func _on_QuitButton_released():
	get_tree().quit()

func _on_PlayButton_released():
	get_node("MainMenu").hide()
	get_node("PlayMenu").show()
	current_menu = MAINMENU.PLAY
	list_save()
	

func list_save():
	var dir  = Directory.new()
	if dir.open("user://save/") == OK:
		dir.list_dir_begin()
		var file_name = dir.get_next()
		while (file_name != ""):
			if !dir.current_is_dir():
				var name = file_name.split(".")
				get_node("PlayMenu/PlayPanel/SaveList").add_item(name[0])
			file_name = dir.get_next()
	else:
		if dir.open("user://") == OK:
			if dir.make_dir("save") != OK:
				print("Error create dir : user://save")
		else:
			print("An error occured when trying to access the path.")

func _on_PlayBackButton_released():
	get_node("PlayMenu").hide()
	get_node("MainMenu").show()
	get_node("PlayMenu/PlayPanel/SaveList").clear()
	current_menu = MAINMENU.MAIN

func _on_NewGameButton_released():
	get_tree().change_scene("res://scenes/game_scene.tscn")


func _on_OptionsButton_released():
	get_tree().change_scene("res://scenes/options_scene.tscn")


func _on_SaveList_item_activated( index ):
	var saveList = get_node("PlayMenu/PlayPanel/SaveList").get_item_text(index)


func _on_SaveList_item_selected( index ):
	get_node("PlayMenu/PlayPanel/SaveDetailsPanel/LoadGameButton").set_disabled(false)
	get_node("PlayMenu/PlayPanel/SaveDetailsPanel/LoadGameButton/RemoveGameButton").set_disabled(false)
	indexSave = index
	var saveList = get_node("PlayMenu/PlayPanel/SaveList").get_item_text(index)
	var saveGame = File.new()
	if !saveGame.file_exists("user://save/"+saveList+".save"):
		print("Error file don't exist")
		return
	saveGame.open("user://save/"+saveList+".save", File.READ)
	var currentLine = {}
	currentLine.parse_json(saveGame.get_line())
	get_node("PlayMenu/PlayPanel/SaveDetailsPanel/MoneyLabel").show()
	get_node("PlayMenu/PlayPanel/SaveDetailsPanel/MoneyLabel/MoneyValueLabel").set_text(String(currentLine["money"]))
	get_node("PlayMenu/PlayPanel/SaveDetailsPanel/MoneyLabel/DateLabel/DateValueLabel").set_text(String(currentLine["timestamp"]))


func _on_RemoveGameButton_released():
	var saveName = get_node("PlayMenu/PlayPanel/SaveList").get_item_text(indexSave)
	get_node("PlayMenu/PlayPanel/ConfirmationRemoveDialog").set_text("Remove save: " + saveName)
	get_node("PlayMenu/PlayPanel/ConfirmationRemoveDialog").popup()
	pass # replace with function body


func _on_ConfirmationRemoveDialog_confirmed():
	var saveName = get_node("PlayMenu/PlayPanel/SaveList").get_item_text(indexSave)
	var dir = Directory.new()
	if dir.open("user://save") == OK:
		if dir.remove(saveName+".save") == OK:
			get_node("PlayMenu/PlayPanel/SaveDetailsPanel/LoadGameButton").set_disabled(true)
			get_node("PlayMenu/PlayPanel/SaveDetailsPanel/LoadGameButton/RemoveGameButton").set_disabled(true)
			get_node("PlayMenu/PlayPanel/SaveDetailsPanel/MoneyLabel").hide()
			get_node("PlayMenu/PlayPanel/SaveList").remove_item(indexSave)
	pass # replace with function body


func _on_LoadGameButton_released():
	var saveName = get_node("PlayMenu/PlayPanel/SaveList").get_item_text(indexSave)
	var file = File.new()
	file.open("user://save/"+saveName+".save", file.READ)
	
	pass # replace with function body
