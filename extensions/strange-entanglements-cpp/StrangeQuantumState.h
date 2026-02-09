#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <set>
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

private:
	// ------------------------------------------------------------------------
	// Constants
	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	static const array<array<Vector2, 2>, 2> kHadamard;

public:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	virtual void _ready() override;

	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	void DoHadamard(int qubit) { DoSingleQubitOperation(kHadamard, qubit); }

	// ------------------------------------------------------------------------
	// Representation
	// ------------------------------------------------------------------------
	void GetFactorisation() const;

protected:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	static void _bind_methods();

private:
	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	void DoSingleQubitOperation(array<array<Vector2, 2>, 2> const& operation, int qubit);
	void DoControlledOperation(array<array<Vector2, 2>, 2> const& operation, int target_qubit, int control_qubit, int control_state);
	void DoErrorCorrection();

	// ------------------------------------------------------------------------
	// Representation
	// ------------------------------------------------------------------------
	vector<Vector2> GetComplement(int qubits, int measurement) const;

	// ------------------------------------------------------------------------
	// Accessors
	// ------------------------------------------------------------------------
	int GetQubitRepresentation(int qubit) const { return 1 << qubit; }
	int GetBitInStateRepresentation(int state_representation, int qubit) const { return (state_representation & 1 << qubit) >> qubit; }
};

#endif // STRANGE_QUANTUM_STATE_H