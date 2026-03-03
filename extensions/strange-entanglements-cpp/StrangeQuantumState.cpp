#include "StrangeQuantumState.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
// Operations
// -----------------------------------------------------------------------------
const array<array<complex<double>, 2>, 2> StrangeQuantumState::kHadamard =
{
	complex<double>(cos(Math_PI / 4)),  complex<double>( sin(Math_PI / 4)),
	complex<double>(sin(Math_PI / 4)),  complex<double>(-cos(Math_PI / 4)),
};

// -----------------------------------------------------------------------------
// StrangeQuantumState::_bind_methods:
// -----------------------------------------------------------------------------
void StrangeQuantumState::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("initialise"), &StrangeQuantumState::Initialise);

	ClassDB::bind_method(D_METHOD("get_qubits"), &StrangeQuantumState::GetQubits);
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::_ready:
// -----------------------------------------------------------------------------
void StrangeQuantumState::_ready()
{
	Node::_ready();
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::Initialise: Initialise quantum state with a fixed number
// of qubits.
// -----------------------------------------------------------------------------
void StrangeQuantumState::Initialise(size_t qubits)
{
	assert(qubits);

	mSuperposition = make_unique<StrangeSuperposition>(qubits);
	mSuperposition->mData[0] = complex<double>(1.0);
}