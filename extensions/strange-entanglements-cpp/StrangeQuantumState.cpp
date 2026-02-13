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
	ClassDB::bind_method(D_METHOD("do_hadamard"), &StrangeQuantumState::DoHadamard);
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
	{
		for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(state_representation).c_str(), ": ", mSuperposition[state_representation]);
		}
		UtilityFunctions::print("");

		GetFactorisation();
		UtilityFunctions::print("");
	}

	{
		vector<Vector2> complement = GetComplement(1, 0);

		for (int state_representation = 0; state_representation < complement.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(mQubits - 1, state_representation).c_str(), ": ", complement[state_representation]);
		}
		UtilityFunctions::print("");
	}

	DoHadamard(0);

	{
		for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(state_representation).c_str(), ": ", mSuperposition[state_representation]);
		}
		UtilityFunctions::print("");

		GetFactorisation();
		UtilityFunctions::print("");
	}

	{
		vector<Vector2> complement = GetComplement(1, 0);

		for (int state_representation = 0; state_representation < complement.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(mQubits - 1, state_representation).c_str(), ": ", complement[state_representation]);
		}
		UtilityFunctions::print("");
	}

	DoHadamard(1);

	{
		for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(state_representation).c_str(), ": ", mSuperposition[state_representation]);
		}
		UtilityFunctions::print("");

		GetFactorisation();
		UtilityFunctions::print("");
	}

	{
		vector<Vector2> complement = GetComplement(1, 0);

		for (int state_representation = 0; state_representation < complement.size(); ++state_representation)
		{
			UtilityFunctions::print(GetState(mQubits - 1, state_representation).c_str(), ": ", complement[state_representation]);
		}
		UtilityFunctions::print("");
	}
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::DoSingleQubitOperation: Apply a single qubit operation
// to a target qubit.
// -----------------------------------------------------------------------------
void StrangeQuantumState::DoSingleQubitOperation(array<array<Vector2, 2>, 2> const& operation, int qubit)
{
	vector<Vector2> new_superposition = vector<Vector2>(mSuperposition.size());

	int qubit_representation = GetQubitRepresentation(qubit);
	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		if (mSuperposition[state_representation].length())
		{
			int substate_representation = (state_representation | qubit_representation) ^ qubit_representation;
			int row = bool{state_representation & qubit_representation}; 
			for (int i = 0; i < 2; ++i)
			{
				int index = substate_representation + i * qubit_representation;
				new_superposition[index] += operation[row][i].length() * mSuperposition[state_representation].rotated(operation[row][i].angle());
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
void StrangeQuantumState::DoErrorCorrection(vector<Vector2>& superposition) const
{
	for (int state_representation = 0; state_representation < superposition.size(); ++state_representation)
	{
		superposition[state_representation] = (100000.0 * superposition[state_representation]).round() / 100000.0;
	}
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetFactorisation:
// -----------------------------------------------------------------------------
void StrangeQuantumState::GetFactorisation() const
{
	std::set<int> factors = { };

	for (int i = 1; i < 1 << mQubits; ++i)
	{
		bool factored_out = false;
		for (int factor : factors)
		{
			if (factored_out = factor & i)
			{
				break;
			}
		}

		bool is_factor = !factored_out;
		if (is_factor)
		{
			int measurements = 1 << __popcnt(i);
			vector<Vector2> complement = GetComplement(i, 0);
			vector<Vector2> zeroes = vector<Vector2>(complement.size());

			for (int measurement = 1; measurement < measurements; ++measurement)
			{
				if (complement == zeroes)
				{
					complement = GetComplement(i, measurement);
				}
				else
				{
					vector<Vector2> other = GetComplement(i, measurement);
					if (other != complement && other != zeroes)
					{
						is_factor = false;
						break;
					}
				}
			}
		}

		if (is_factor)
		{
			factors.emplace(i);

			// -----------------------------------------------------------------
			// DEBUG:
			// -----------------------------------------------------------------
			UtilityFunctions::print("factor: ", i, "!");
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

	double magnitude = 0.0;
	for (int state_representation = 0; state_representation < mSuperposition.size(); ++state_representation)
	{
		bool skip = false;
		for (int i = 0, q = qubits, m = measurement; q; ++i, q >>= 1)
		{
			if (q & 1)
			{
				skip |= GetBitInStateRepresentation(state_representation, i) != (m & 1);
				m >>= 1;
			}
		}

		if (!skip)
		{
			superposition.push_back(mSuperposition[state_representation]);
			magnitude += mSuperposition[state_representation].length_squared();
		}
	}

	bool first_nonzero_element = true;
	float angle = 0.0;

	if (magnitude = sqrt(magnitude))
	{
		for (int state_representation = 0; state_representation < superposition.size(); ++state_representation)
		{
			

			if (first_nonzero_element && superposition[state_representation] != Vector2(0.0, 0.0))
			{
				angle = superposition[state_representation].angle();
				first_nonzero_element = false;
			}
			
			superposition[state_representation] = superposition[state_representation].rotated(-angle);
			superposition[state_representation] /= magnitude;
		}
	}

	DoErrorCorrection(superposition);

	return superposition;
}

// ----------------------------------------------------------------------------
// StrangeQuantumState::GetState: Read state representation as (binary) state.
// ----------------------------------------------------------------------------
string StrangeQuantumState::GetState(int qubits, int state_representation)
{
	if (qubits)
	{
		string state = string{ };
		for (int i = 0; i < qubits; ++i)
		{
			state = std::to_string(state_representation & 1).append(state);
			state_representation >>= 1;
		}

		return state;
	}
	else
	{
		return "∅";
	}
}