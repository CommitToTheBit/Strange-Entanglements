#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <complex>
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
	vector<Vector2> mSuperpositions;

private:
	// ------------------------------------------------------------------------
	// Constants
	// ------------------------------------------------------------------------
	// Operations
	// ------------------------------------------------------------------------
	static const array<array<complex<float>, 2>, 2> kHadamard;

public:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	virtual void _ready() override;

protected:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	static void _bind_methods();
};

#endif // STRANGE_QUANTUM_STATE_H