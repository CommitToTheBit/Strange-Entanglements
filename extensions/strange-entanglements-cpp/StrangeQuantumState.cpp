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
	cos(Math_PI / 4),   sin(Math_PI / 4),
	sin(Math_PI / 4),  -cos(Math_PI / 4),
};

// -----------------------------------------------------------------------------
// StrangeQuantumState::_bind_methods:
// -----------------------------------------------------------------------------
void StrangeQuantumState::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("initialise"), &StrangeQuantumState::Initialise);

	ClassDB::bind_method(D_METHOD("get_qubits"), &StrangeQuantumState::GetQubits);
	ClassDB::bind_method(D_METHOD("get_qubits_entangled_with"), &StrangeQuantumState::GetQubitsEntangledWith);
	ClassDB::bind_method(D_METHOD("get_orbits_of"), &StrangeQuantumState::GetOrbitsOf);
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
	Initialise(2);

	UtilityFunctions::print("superposition: ", mSuperposition->GetRepresentation().c_str());
	for (size_t qubit = 0; qubit < mSuperposition->mQubits; ++qubit)
	{
		UtilityFunctions::print("* qubit ", qubit, " is entangled in ", GetQubitsEntangledWith(qubit), ": ", mEntanglements[qubit]->GetRepresentation().c_str());

		GetOrbitsOf(qubit);
	}
	// -------------------------------------------------------------------------
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::Initialise: Initialise quantum state with a fixed number
// of qubits.
// -----------------------------------------------------------------------------
void StrangeQuantumState::Initialise(size_t qubits)
{
	assert(qubits);

	mSuperposition = make_unique<StrangeSuperposition>(qubits);
	mSuperposition->mData[0] = 1.0;

	// -------------------------------------------------------------------------
	// DEBUG:
	// -------------------------------------------------------------------------
	mSuperposition->mData[1] = 1.0;
	// mSuperposition->mData[2] = 1.0;
	// -------------------------------------------------------------------------
	// mSuperposition->mData[3] = polar(1.0, Math_PI / 8);

	mSuperposition = Normalise(mSuperposition.get());
	mSuperposition = ErrorCorrect(mSuperposition.get());
	// -------------------------------------------------------------------------
	// size_t qubit = 1;
	// size_t qubit_representation = mSuperposition->GetQubitRepresentation(qubit);
	// size_t measurement_representation = mSuperposition->GetMeasurementRepresentation(qubit, 1);
	// mSuperposition = Collapse(mSuperposition.get(), qubit_representation, measurement_representation);
	// -------------------------------------------------------------------------
	// mSuperposition = CollapseAndSimplify(mSuperposition.get(), qubit_representation, measurement_representation);
	// -------------------------------------------------------------------------

	mEntanglements = Factorise(mSuperposition.get());
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::Collapse: Collapse a superposition to a measurement.
// -----------------------------------------------------------------------------
unique_ptr<StrangeSuperposition> StrangeQuantumState::Collapse(StrangeSuperposition const* superposition, size_t qubits_representation, size_t measurement_representation)
{
	unique_ptr<StrangeSuperposition> collapsed_superposition = make_unique<StrangeSuperposition>(superposition->mQubits);
	for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
	{
		collapsed_superposition->mData[dimension] = (dimension & qubits_representation) == measurement_representation ? superposition->mData[dimension] : 0.0;
	}

	return collapsed_superposition;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::CollapseAndSimplify: Collapse a superposition to a
// measurement, and remove those measured qubits.
// -----------------------------------------------------------------------------
unique_ptr<StrangeSuperposition> StrangeQuantumState::CollapseAndSimplify(StrangeSuperposition const* superposition, size_t qubits_representation, size_t measurement_representation)
{
	unique_ptr<StrangeSuperposition> collapsed_superposition = Collapse(superposition, qubits_representation, measurement_representation);

	size_t qubits_collapsed = __popcnt(qubits_representation);
	unique_ptr<StrangeSuperposition> simplified_superposition = make_unique<StrangeSuperposition>(collapsed_superposition->mQubits - qubits_collapsed);
	
	size_t simplified_dimension = 0;
	for (size_t dimension = 0; dimension < collapsed_superposition->mDimensions; ++dimension)
	{
		if ((dimension & qubits_representation) == measurement_representation)
		{
			simplified_superposition->mData[simplified_dimension++] = collapsed_superposition->mData[dimension];
		}
	}

	return simplified_superposition;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::Normalise: Normalise a superposition.
// -----------------------------------------------------------------------------
// 03-Mar-2025: As book-keeping, we'll make sure the first non-zero term is also
// strictly positive.
// -----------------------------------------------------------------------------
unique_ptr<StrangeSuperposition> StrangeQuantumState::Normalise(StrangeSuperposition const* superposition)
{
	unique_ptr<StrangeSuperposition> normalised_superposition = make_unique<StrangeSuperposition>(superposition->mQubits);

	double magnitude = 0.0;
	double argument = 0.0;

	for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
	{
		if (norm(superposition->mData[dimension]))
		{
			if (!magnitude)
			{
				argument = arg(superposition->mData[dimension]);
			}

			magnitude += norm(superposition->mData[dimension]);
		}
	}

	if (magnitude = sqrt(magnitude))
	{
		complex<double> coefficient = polar(magnitude, argument);
		for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
		{
			normalised_superposition->mData[dimension] = superposition->mData[dimension] / coefficient;
		}
	}

	return normalised_superposition;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::ErrorCorrect: Error-correct a superposition.
// -----------------------------------------------------------------------------
unique_ptr<StrangeSuperposition> StrangeQuantumState::ErrorCorrect(StrangeSuperposition const* superposition)
{
	unique_ptr<StrangeSuperposition> error_corrected_superposition = make_unique<StrangeSuperposition>(superposition->mQubits);

	for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
	{
		double real = round(100000.0 * superposition->mData[dimension].real()) / 100000.0;
		double imag = round(100000.0 * superposition->mData[dimension].imag()) / 100000.0;

		error_corrected_superposition->mData[dimension] = complex<double>(real, imag);
	}

	return error_corrected_superposition;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::Factorise: Factorise a superposition. Returns a fixed-
// size array of each qubit's factors.
// -----------------------------------------------------------------------------
unique_ptr<shared_ptr<StrangeSuperposition>[]> StrangeQuantumState::Factorise(StrangeSuperposition const* superposition)
{
	unique_ptr<shared_ptr<StrangeSuperposition>[]> factorised_superposition = make_unique<shared_ptr<StrangeSuperposition>[]>(superposition->mQubits);
	memset(factorised_superposition.get(), 0, superposition->mQubits * sizeof(shared_ptr<StrangeSuperposition>));

	vector<size_t> factors = { };
	for (size_t qubits_representation = 1; qubits_representation < 1 << superposition->mQubits; ++qubits_representation)
	{
		bool factorisable = true;
		for (size_t factor : factors)
		{
			if (qubits_representation & factor)
			{
				factorisable = false;
				break;
			}
		}

		if (factorisable)
		{
			unique_ptr<StrangeSuperposition> factor_superposition = nullptr;
			
			size_t qubits_complement = qubits_representation ^ (1 << superposition->mQubits) - 1;
			vector<size_t> measurement_complements = GetAllMeasurementRepresentations(qubits_complement);
			for (size_t measurement_complement : measurement_complements)
			{
				if (factor_superposition && factor_superposition->IsNonzero())
				{
					unique_ptr<StrangeSuperposition> collapsed_superposition;
					collapsed_superposition = CollapseAndSimplify(superposition, qubits_complement, measurement_complement);
					collapsed_superposition = Normalise(collapsed_superposition.get());
					collapsed_superposition = ErrorCorrect(collapsed_superposition.get());

					if (collapsed_superposition->IsNonzero() && *collapsed_superposition != *factor_superposition)
					{
						factorisable = false;
						break;
					}
				}
				else
				{
					factor_superposition = CollapseAndSimplify(superposition, qubits_complement, measurement_complement);
					factor_superposition = Normalise(factor_superposition.get());
					factor_superposition = ErrorCorrect(factor_superposition.get());
				}
			}

			if (factorisable)
			{
				shared_ptr<StrangeSuperposition> shared_factor_superposition = make_shared<StrangeSuperposition>(factor_superposition->mQubits);
				for (size_t dimension = 0; dimension < factor_superposition->mDimensions; ++dimension)
				{
					shared_factor_superposition->mData[dimension] = factor_superposition->mData[dimension];
				}

				for (size_t qubit = 0; qubit < superposition->mQubits; ++qubit)
				{
					if (superposition->GetQubitRepresentation(qubit) & qubits_representation)
					{
						factorised_superposition[qubit] = shared_factor_superposition;
					}
				}

				factors.emplace_back(qubits_representation);
			}
		}
	}

	return factorised_superposition;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetAllMeasurementRepresentations: Get all measurements
// on a set of qubits.
// -----------------------------------------------------------------------------
vector<size_t> StrangeQuantumState::GetAllMeasurementRepresentations(size_t qubits_representation)
{
	vector<size_t> measurement_representations = { 0 };
	for (size_t qubit_representation = 1; qubit_representation <= qubits_representation; qubit_representation <<= 1)
	{
		if (qubits_representation & qubit_representation)
		{
			size_t measurements = measurement_representations.size();
			for (int measurement = 0; measurement < measurements; ++measurement)
			{
				measurement_representations.emplace_back(qubit_representation | measurement_representations[measurement]);
			}
		}
	}

	return measurement_representations;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetQubitsEntangledWith: Get a set of entangled qubits,
// including self.
// -----------------------------------------------------------------------------
PackedInt32Array StrangeQuantumState::GetQubitsEntangledWith(size_t qubit)
{
	PackedInt32Array qubits;
	for (size_t i = 0; i < mSuperposition->mQubits; ++i)
	{
		if (mEntanglements[i] == mEntanglements[qubit])
		{
			qubits.append(i);
		}
	}

	return qubits;
}

// -----------------------------------------------------------------------------
// StrangeQuantumState::GetOrbitsOf: Get all orbits of a qubit, ordered by
// indices in an entanglement.
// -----------------------------------------------------------------------------
PackedFloat64Array StrangeQuantumState::GetOrbitsOf(size_t qubit)
{
	PackedInt32Array qubits = GetQubitsEntangledWith(qubit);
	PackedFloat64Array orbits;

	size_t qubits_representation = 0;
	for (size_t i = 0; i < qubits.size(); ++i)
	{
		if (qubits[i] != qubit)
		{
			qubits_representation |= 1 << i;
		}
	}

	vector<size_t> measurement_representations = GetAllMeasurementRepresentations(qubits_representation);
	for (size_t measurement_representation : measurement_representations)
	{
		unique_ptr<StrangeSuperposition> superposition;
		superposition = CollapseAndSimplify(mEntanglements[qubit].get(), qubits_representation, measurement_representation);
		if (superposition->IsNonzero())
		{
			superposition = Normalise(superposition.get());
			superposition = ErrorCorrect(superposition.get());

			double y = superposition->mData[1].real() ? superposition->mData[1].real() / abs(superposition->mData[1].real()) : 0.0;
			double x = superposition->mData[0].real() ? superposition->mData[0].real() / abs(superposition->mData[0].real()) : 0.0;

			double orbit = 2.0 * atan2(y, x) - Math_PI / 2.0;
			orbits.append(orbit);

			UtilityFunctions::print("  * ", superposition->GetRepresentation().c_str(), " has orbit ", 180.0 * orbit / Math_PI);
		}
	}

	return orbits;
}