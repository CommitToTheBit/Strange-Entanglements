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

// -----------------------------------------------------------------------------
// StrangeQuantumState::Collapse: Collapse a superposition to a measurement.
// -----------------------------------------------------------------------------
unique_ptr<StrangeSuperposition> StrangeQuantumState::Collapse(StrangeSuperposition const* superposition, size_t qubits_representation, size_t measurement_representation)
{
	unique_ptr<StrangeSuperposition> collapsed_superposition = make_unique<StrangeSuperposition>(superposition->mQubits);
	for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
	{
		collapsed_superposition->mData[dimension] = dimension & qubits_representation == measurement_representation ? superposition->mData[dimension] : complex<double>(0.0);
	}

	return collapsed_superposition;
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
		if (abs(superposition->mData[dimension]))
		{
			if (!magnitude)
			{
				argument = arg(superposition->mData[dimension]);
			}

			magnitude += abs(superposition->mData[dimension]);
		}
	}

	if (magnitude)
	{
		complex<double> coefficient = polar(magnitude, argument);
		for (size_t dimension = 0; dimension < superposition->mDimensions; ++dimension)
		{
			superposition->mData[dimension] /= coefficient;
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
		double real = round(10000.0 * superposition->mData[dimension].real()) / 100000.0;
		double imag = round(10000.0 * superposition->mData[dimension].imag()) / 100000.0;

		error_corrected_superposition->mData[dimension] = complex<double>(real, imag);
	}

	return error_corrected_superposition;
}
