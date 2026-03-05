class_name Level

extends TileMapLayer

var yous : Array
var you : You

var quantum_state : StrangeQuantumState
var qubits : Array

func _ready():
	for child in get_children():
		if child is You:
			child.index = yous.size()
			yous.push_back(child)
		elif child is Qubit:
			child.index = qubits.size()
			qubits.push_back(child)
	
	if yous.size():
		you = yous.front()
	
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
	if Input.is_action_just_pressed("ui_cycle_yous"):
		if yous.size() > 1:
			you = yous[(you.index + 1) % yous.size()]

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
	for you in yous:
		if you.position == position:
			return you
	for qubit in qubits:
		if qubit.position == position:
			return qubit
	return null

func _on_state_changed():
	for qubit : Qubit in qubits:
		qubit.dirty = true
	
	for qubit : Qubit  in qubits:
		if qubit.dirty:
			var qubits_entangled : PackedInt32Array = quantum_state.get_qubits_entangled_with(qubit.index)
			
			for qubit_entangled in qubits_entangled:
				if qubit_entangled == qubit.index:
					var orbits : PackedFloat64Array = quantum_state.get_orbits_of(qubit.index);
					qubits[qubit_entangled].set_orbits(orbits.size())
					for i in range(orbits.size()):
						print("qubit ", qubit.index, " has ", i, "th orbit ", orbits[i]);
						qubit.set_orbit(i, orbits[i])
				else:
					var basis : PackedFloat64Array = quantum_state.get_basis_of(qubit.index, qubit_entangled)
					qubits[qubit_entangled].set_orbits(basis.size())
					for i in range(basis.size()):
						print("qubit ", qubit_entangled, " has ", i, "th orbit ", basis[i]);
						qubits[qubit_entangled].set_orbit(i, basis[i])
				qubits[qubit_entangled].dirty = false
	print("")
