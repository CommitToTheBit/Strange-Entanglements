#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <set>
#include <string>
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
	int mQubits;
	vector<Vector2> mSuperposition;

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
	// Initialisation
	// ------------------------------------------------------------------------
	void Initialise(int qubits);

	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	void DoHadamard(int qubit) { DoSingleQubitOperation(kHadamard, qubit); }

	// ------------------------------------------------------------------------
	// Representation
	// ------------------------------------------------------------------------
	Array GetFactorisation() const;

	// ------------------------------------------------------------------------
	// Accessors
	// ------------------------------------------------------------------------
	PackedVector2Array GetSuperpositionAsArray(int qubits) const;

	int GetQubits() { return mQubits; }

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

	void DoErrorCorrection() { DoErrorCorrection(mSuperposition); }
	void DoErrorCorrection(vector<Vector2>& superposition) const;

	// ------------------------------------------------------------------------
	// Representation
	// ------------------------------------------------------------------------
	vector<Vector2> GetSuperposition(int qubits) const;
	vector<Vector2> GetComplement(int qubits, int measurement) const;

	int GetQubitRepresentation(int qubit) const { return 1 << mQubits - qubit - 1; }
	int GetBitInStateRepresentation(int state_representation, int qubit) const { return (state_representation & 1 << mQubits - qubit - 1) >> mQubits - qubit - 1; }

	static string GetState(int qubits, int state_representation);
 	string GetState(int state_representation) { return GetState(mQubits, state_representation); }
};

#endif // STRANGE_QUANTUM_STATE_H