class_name Qubit

extends Node2D

var packed_qubit_orbit : PackedScene

var qubit_orbits : Array

var index : int
var orbits : int

func _ready():
	packed_qubit_orbit = load("res://scenes/qubit_orbit.tscn")
	
	# --------------------------------------------------------------------------
	# DEBUG
	# --------------------------------------------------------------------------
	for radius in range(8.0, 40.0, 8.0):
		var qubit_orbit = packed_qubit_orbit.instantiate()
		add_child(qubit_orbit)
		
		qubit_orbit.rotation += radius * PI / 16.0
		qubit_orbit.set_radius(radius)
		qubit_orbits.append(qubit_orbit)
