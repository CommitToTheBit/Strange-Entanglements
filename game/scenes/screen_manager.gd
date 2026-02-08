
class_name ScreenManager

extends Node

enum Transition
{
	TRANSITION_NONE,
}

var screen_stack : Array
var screen_transition : Transition = Transition.TRANSITION_NONE

var screen : Screen = null
var next_screen : Screen = null

func _ready():
	# play_transition()
	pass

func _on_transition(index : int):
	# --------------------------------------------------------------------------
	# Ignore calls if mid-transition...
	# --------------------------------------------------------------------------
	if next_screen != null:
		return
	
	var next_transition : Transition = index as Transition
	if next_transition != screen_transition:
		screen_stack.append(screen_transition)
		screen_transition = next_transition
	play_transition();

func _on_transition_back():
	# --------------------------------------------------------------------------
	# Ignore calls if mid-transition...
	# --------------------------------------------------------------------------
	if next_screen != null:
		return
	
	if screen_stack.size():
		screen_transition = screen_stack.pop_back()
		play_transition();

func play_transition():
	var tween : Tween = get_tree().create_tween()
	tween.set_parallel()
	
	const delay : float = 0.167
	if next_screen != null:
		await add_child(next_screen)
		
		next_screen.modulate = Color(1.0, 1.0, 1.0, 0.0)
		tween.tween_property(next_screen, "modulate", Color(1.0, 1.0, 1.0, 1.0), delay)
	
	if screen != null:
		tween.tween_property(screen, "modulate", Color(1.0, 1.0, 1.0, 0.0), delay)
	
	if next_screen != null or screen != null:
		tween.tween_callback(Callable(self, "_on_transition_finished")).set_delay(delay)
		tween.play()

func _on_transition_finished():
	if screen != null:
		screen._on_closed()
	if next_screen != null:
		next_screen._on_opened()
		next_screen.connect("transition", Callable(self, "_on_transition"))
		next_screen.connect("transition_back", Callable(self, "_on_transition_back"))
	screen = next_screen
	next_screen = null

func instantiate_screen(name : String):
	var path : String = "res://scenes/screens/" + name + ".tscn"
	var scene : PackedScene = load(path)
	return scene.instantiate()
