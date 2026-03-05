class_name Qubit

extends Node2D

var packed_qubit_orbit : PackedScene

var qubit_orbits : Array

var index : int
var orbits : int

var dirty : bool = false

func _ready():
	packed_qubit_orbit = load("res://scenes/qubit_orbit.tscn")

func set_orbits(value : int):
	for qubit_orbit in qubit_orbits:
		remove_child(qubit_orbit)
	
	orbits = value
	for radius in range(32.0, 0.0, -32.0 / value):
		var qubit_orbit = packed_qubit_orbit.instantiate()
		add_child(qubit_orbit)
		
		qubit_orbit.set_radius(radius)
		qubit_orbits.append(qubit_orbit)

func set_orbit(orbit : int, angle : float):
	qubit_orbits[orbit].rotation = angle;
	print(qubit_orbits[orbit].rotation_degrees)
