
extends Control

func _ready():
	# Tell we accept input key
	set_process_input(true)

func _on_BackButton_released():
	get_tree().change_scene("res://scenes/main_menu.tscn")
