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

private:
	// -------------------------------------------------------------------------
	// Constants
	// -------------------------------------------------------------------------
	// Operations
	// -------------------------------------------------------------------------
	static const array<array<complex<double>, 2>, 2> kHadamard;

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
	// Accessors
	// -------------------------------------------------------------------------
	size_t GetQubits() const { return mSuperposition->mQubits; }

protected:
	// -------------------------------------------------------------------------
	// Godot
	// -------------------------------------------------------------------------
	static void _bind_methods();
};

#endif // STRANGE_QUANTUM_STATE_H