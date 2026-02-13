class_name Level

extends TileMapLayer

var youse : Array
var you : You

var quantum_state : StrangeQuantumState
var qubits : Array

func _ready():
	for child in get_children():
		if child is You:
			child.index = youse.size()
			youse.push_back(child)
		elif child is Qubit:
			child.index = qubits.size()
			qubits.push_back(child)
	
	if youse.size():
		you = youse.front()
	
	quantum_state = get_node("QuantumState")
	quantum_state.initialise(qubits.size())

func _process(_delta):
	if Input.is_action_just_pressed("ui_left"):
		move(Vector2i.LEFT)
	if Input.is_action_just_pressed("ui_right"):
		move(Vector2i.RIGHT)
	if Input.is_action_just_pressed("ui_up"):
		move(Vector2i.UP)
	if Input.is_action_just_pressed("ui_down"):
		move(Vector2i.DOWN)
	if Input.is_action_just_pressed("ui_cycle_youse"):
		if youse.size() > 1:
			you = youse[(you.index + 1) % youse.size()]

func move(direction : Vector2i):
	var adjacent_position : Vector2 = map_to_local(local_to_map(you.position) + direction)
	var adjacent_element = find_adjacent_element(adjacent_position)
	
	if adjacent_element:
		if find_adjacent_element(map_to_local(local_to_map(adjacent_element.position) + direction)):
			return
	
	you.position = adjacent_position
	if adjacent_element:
		adjacent_element.position = map_to_local(local_to_map(adjacent_element.position) + direction)

func find_adjacent_element(position : Vector2):
	for you in youse:
		if you.position == position:
			return you
	for qubit in qubits:
		if qubit.position == position:
			return qubit
	return null
