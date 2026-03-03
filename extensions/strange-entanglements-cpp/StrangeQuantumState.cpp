#include "StrangeQuantumState.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
// Operations
// -----------------------------------------------------------------------------
const array<array<complex<float>, 2>, 2> StrangeQuantumState::kHadamard =
{
	complex<float>(cos(Math_PI / 4), 0.0),  complex<float>( sin(Math_PI / 4), 0.0),
	complex<float>(sin(Math_PI / 4), 0.0),  complex<float>(-cos(Math_PI / 4), 0.0),
};

// -----------------------------------------------------------------------------
// StrangeQuantumState::_bind_methods:
// -----------------------------------------------------------------------------
void StrangeQuantumState::_bind_methods()
{

}

// -----------------------------------------------------------------------------
// StrangeQuantumState::_ready:
// -----------------------------------------------------------------------------
void StrangeQuantumState::_ready()
{
	Node::_ready();
}