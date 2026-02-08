#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <vector>

using namespace std;
using namespace godot;

// ----------------------------------------------------------------------------
// StrangeQuantumState: Node to manage quantum state of a puzzle.
// ----------------------------------------------------------------------------
class StrangeQuantumState : public Node
{
	GDCLASS(StrangeQuantumState, Node)

public:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	int mQubits = 2;
	vector<Vector2> mSuperposition =
	{
		Vector2(1.0, 0.0),
		Vector2(0.0, 0.0),
		Vector2(0.0, 0.0),
		Vector2(0.0, 0.0),
	};

public:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	virtual void _ready() override;

	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	void DoSingleQubitOperation(array<array<float, 2>, 2> const& operation, int qubit);


	void DoHadamard(int qubit);
	void DoControlledNot(int qubit, int control_qubit, int control_state);

protected:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	static void _bind_methods();
};

#endif // STRANGE_QUANTUM_STATE_H