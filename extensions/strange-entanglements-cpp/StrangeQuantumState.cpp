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

	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		UtilityFunctions::print(mSuperposition[state_representation]);
	}
	UtilityFunctions::print("");

	vector<Vector2> complement = GetComplement(1, 3);
	UtilityFunctions::print("size: ", complement.size());

}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoSingleQubitOperation: Apply a single qubit operation
// to a target qubit.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoSingleQubitOperation(array<array<Vector2, 2>, 2> const& operation, int qubit)
{
	vector<Vector2> new_superposition = vector<Vector2>(mSuperposition.size());

	int bit = 1 << qubit;
	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		if (mSuperposition[state_representation].length())
		{
			int row = state_representation & bit;
			for (int i = 0; i < 2; ++i)
			{
				new_superposition[state_representation ^ row + i * bit] += operation[row][i].length() * mSuperposition[state_representation].rotated(operation[row][i].angle());
			}
		}
	}

	mSuperposition = new_superposition;

	DoErrorCorrection();
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoControlledOperation: Apply a controlled operation to a
// target qubit.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoControlledOperation(array<array<Vector2, 2>, 2> const& operation, int target_qubit, int control_qubit, int control_state)
{
	// -------------------------------------------------------------------------
	// UNIMPLEMENTED:
	// -------------------------------------------------------------------------
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoErrorCorrection: Apply error correction after an
// operation.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoErrorCorrection()
{
	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		mSuperposition[state_representation] = (100000.0 * mSuperposition[state_representation]).round() / 100000.0;
	}
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetFactorisation:
// -----------------------------------------------------------------------------
void StrangeQuantumState::GetFactorisation() const
{
	std::set<int> factors = { };

	for (int i = 1; i <= pow(2.0, mQubits); ++i)
	{
		bool factored_out = false;
		for (int factor : factors)
		{
			if (factored_out = factor & i)
			{
				break;
			}
		}

		if (!factored_out)
		{
			// -----------------------------------------------------------------
			// UNIMPLEMENTED: Need a helper to read off sections...
			// -----------------------------------------------------------------
		}
	}

	// -------------------------------------------------------------------------
	// UNIMPLEMENTED:
	// -------------------------------------------------------------------------
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetComplement:
// -----------------------------------------------------------------------------
// FIXME: Very broken right now!!
// -----------------------------------------------------------------------------
vector<Vector2> StrangeQuantumState::GetComplement(int qubits, int measurement) const
{
	vector<Vector2> superposition;
	superposition.reserve(pow(2.0, mQubits - __popcnt(qubits)));

	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		bool skip = false;
		for (int i = 0, q = qubits, m = measurement; q; ++i, q >>= 1)
		{
			if (q & 1)
			{
				skip |= state_representation % static_cast<int>(pow(2, i + 1)) / pow(2.0, i) != m & 1;
				m >>= 1;
			}
		}

		if (!skip)
		{
			superposition.push_back(mSuperposition[state_representation]);
		}
	}

	// -------------------------------------------------------------------------
	// UNIMPLEMENTED: Normalise!
	// -------------------------------------------------------------------------

	return superposition;
}