#include "StrangeQuantumState.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
// Operations
// -----------------------------------------------------------------------------
const array<array<Vector2, 2>, 2> StrangeQuantumState::kHadamard =
{
	Vector2(cos(Math_PI / 4), 0.0),  Vector2( sin(Math_PI / 4), 0.0),
	Vector2(sin(Math_PI / 4), 0.0),  Vector2(-cos(Math_PI / 4), 0.0),
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

	// -------------------------------------------------------------------------
	// DEBUG:
	// -------------------------------------------------------------------------
	DoHadamard(0);
	DoHadamard(1);

	for (Vector2 state : mSuperposition)
	{
		UtilityFunctions::print(state);
	}
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoSingleQubitOperation: Apply a single qubit operation
// to a given qubit.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoSingleQubitOperation(array<array<Vector2, 2>, 2> const& operation, int qubit)
{
	vector<Vector2> new_superposition = vector<Vector2>(mSuperposition.size());

	int bit = 1 << qubit;
	for (int state = 0; state < mSuperposition.size(); ++state)
	{
		if (mSuperposition[state].length())
		{
			int row = state & bit;
			for (int i = 0; i < 2; ++i)
			{
				new_superposition[state ^ row + i * bit] += operation[row][i].length() * mSuperposition[state].rotated(operation[row][i].angle());
			}
		}
	}

	mSuperposition = new_superposition;

	DoErrorCorrection();
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoErrorCorrection: Apply error correction after an
// operation.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoErrorCorrection()
{
	for (int state = 0; state < mSuperposition.size(); ++state)
	{
		//mSuperposition[state] = (10000.0 * mSuperposition[state]).round() / 10000.0;
	}
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoControlledNot: Apply a controlled not operation to a
// given qubit.
// -----------------------------------------------------------------------------
void DoControlledNot(int qubit, int control_qubit, int control_state)
{

}

