class_name Screen

extends Control

signal transition(index : int)
signal transition_back()

func _on_opened():
	pass

func _on_closed():
	queue_free()
