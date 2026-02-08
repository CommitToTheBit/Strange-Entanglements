#ifndef STRANGE_QUANTUM_STATE_H
#define STRANGE_QUANTUM_STATE_H

#include <godot_cpp/classes/node.hpp>

using namespace std;
using namespace godot; 

// ----------------------------------------------------------------------------
// StrangeQuantumState: Node to manage quantum state of a puzzle.
// ----------------------------------------------------------------------------
class StrangeQuantumState : public Node
{
	GDCLASS(StrangeQuantumState, Node)

protected:
	// ------------------------------------------------------------------------
	// Godot
	// ------------------------------------------------------------------------
	static void _bind_methods();
};

#endif // STRANGE_QUANTUM_STATE_H