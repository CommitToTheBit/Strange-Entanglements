class_name Level

extends TileMapLayer

var quantum_state : StrangeQuantumState
var qubits : Array

func _ready():
	for qubit in get_children():
		if qubit is Qubit:
			qubit.index = qubits.size()
			qubits.push_back(qubit)
	
	quantum_state = get_node("QuantumState")
	quantum_state.initialise(qubits.size())
	
	# --------------------------------------------------------------------------
	# DEBUG
	# --------------------------------------------------------------------------
	print(quantum_state.get_factorisation().size())
	print(quantum_state.get_factorisation()[0])
	print(quantum_state.get_factorisation()[1])
