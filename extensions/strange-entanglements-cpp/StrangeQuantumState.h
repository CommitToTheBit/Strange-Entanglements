#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include "StrangeSuperposition.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <cassert>
#include <complex>
#include <memory>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace godot;

// -----------------------------------------------------------------------------
// StrangeQuantumState: Node to manage quantum state of a puzzle.
// -----------------------------------------------------------------------------
class StrangeQuantumState : public Node
{
	GDCLASS(StrangeQuantumState, Node)

public:
	// -------------------------------------------------------------------------
	// State
	// -------------------------------------------------------------------------
	unique_ptr<StrangeSuperposition> mSuperposition = nullptr;
	unique_ptr<shared_ptr<StrangeSuperposition>[]> mEntanglements = nullptr;

private:
	// -------------------------------------------------------------------------
	// Constants
	// -------------------------------------------------------------------------
	// Operations
	// -------------------------------------------------------------------------
	static const array<array<complex<double>, 2>, 2> kHadamard;
	static const array<array<complex<double>, 2>, 2> kNot;

public:
	// -------------------------------------------------------------------------
	// Godot
	// -------------------------------------------------------------------------
	virtual void _ready() override;

	// -------------------------------------------------------------------------
	// Initialisation
	// -------------------------------------------------------------------------
	void Initialise(size_t qubits);

	// -------------------------------------------------------------------------
	// Operations
	// -------------------------------------------------------------------------
	void DoHadamard(int qubit) { DoSingleQubitOperation(kHadamard, qubit); }
	void DoNot(int qubit)      { DoSingleQubitOperation(kNot, qubit); }
	
	void DoCNot(int target_qubit, int control_qubit, int control_bit) { DoControlledOperation(kNot, target_qubit, control_qubit, control_bit); }

	void DoSingleQubitOperation(array<array<complex<double>, 2>, 2> const& operation, int qubit);
	void DoControlledOperation (array<array<complex<double>, 2>, 2> const& operation, int target_qubit, int control_qubit, int control_bit);

	PackedInt32Array GetQubitsEntangledWith(size_t qubit);
	PackedFloat64Array GetOrbitsOf(size_t qubit);
	PackedFloat64Array GetBasisOf(size_t qubit, size_t at);

	// -------------------------------------------------------------------------
	// Accessors
	// -------------------------------------------------------------------------
	size_t GetQubits() const { return mSuperposition->mQubits; }

protected:
	// -------------------------------------------------------------------------
	// Godot
	// -------------------------------------------------------------------------
	static void _bind_methods();

private:
	// -------------------------------------------------------------------------
	// Operations
	// -------------------------------------------------------------------------
	static unique_ptr<StrangeSuperposition> DoSingleQubitOperation(StrangeSuperposition const* superposition, array<array<complex<double>, 2>, 2> const& operation, int qubit);
	static unique_ptr<StrangeSuperposition> DoControlledOperation (StrangeSuperposition const* superposition, array<array<complex<double>, 2>, 2> const& operation, int target_qubit, int control_qubit, int control_bit);

	static unique_ptr<StrangeSuperposition> Collapse(StrangeSuperposition const* superposition, size_t qubits_representation, size_t measurement_representation);
	static unique_ptr<StrangeSuperposition> CollapseAndSimplify(StrangeSuperposition const* superposition, size_t qubits_representation, size_t measurement_representation);

	static unique_ptr<StrangeSuperposition> Normalise(StrangeSuperposition const* superposition);
	static unique_ptr<StrangeSuperposition> ErrorCorrect(StrangeSuperposition const* superposition);

	static unique_ptr<shared_ptr<StrangeSuperposition>[]> Factorise(StrangeSuperposition const* superposition);

	// -------------------------------------------------------------------------
	// Representation
	// -------------------------------------------------------------------------
	static vector<size_t> GetAllMeasurementRepresentations(size_t qubits_representation);
};

#endif // STRANGE_QUANTUM_STATE_H