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
	mSuperposition->mData[2] = 1.0;
	// -------------------------------------------------------------------------
	mSuperposition->mData[3] = polar(1.0, Math_PI / 8);

	mSuperposition = Normalise(mSuperposition.get());
	mSuperposition = ErrorCorrect(mSuperposition.get());
	// -------------------------------------------------------------------------
	size_t qubit = 1;
	size_t qubit_representation = mSuperposition->GetQubitRepresentation(qubit);
	size_t measurement_representation = mSuperposition->GetMeasurementRepresentation(qubit, 0);
	mSuperposition = Collapse(mSuperposition.get(), qubit_representation, measurement_representation);
	// -------------------------------------------------------------------------
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

	return factorised_superposition;

	//for (size_t qubits_representation = 1; qubits < 1 << superposition->mQubits;)


	/* std::set<int> factors = { };

    for (int qubits = 1; qubits < 1 << mQubits; ++qubits)
    {
        bool factored_out = false;
        for (int factor : factors)
        {
            if (factored_out = factor & qubits)
            {
                break;
            }
        }

        bool is_factor = !factored_out;
        if (is_factor)
        {
            int measurements = 1 << __popcnt(qubits);
            vector<Vector2> complement = GetComplement(qubits, 0);
            vector<Vector2> zeroes = vector<Vector2>(complement.size());

            for (int measurement = 1; measurement < measurements; ++measurement)
            {
                if (complement == zeroes)
                {
                    complement = GetComplement(qubits, measurement);
                }
                else
                {
                    vector<Vector2> other = GetComplement(qubits, measurement);
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
            factors.emplace(qubits);
        }
    }

    Array gdFactors;
    for (auto iter = factors.begin(); iter != factors.end(); ++iter)
    {
        Array gdFactor;

        int qubit = 0;
        for (int factor = *iter; factor; factor >>= 1, ++qubit)
        {
            if (factor & 1)
            {
                gdFactor.push_front(qubit);
            }
        }

        gdFactors.push_back(gdFactor);
    }

    return gdFactors; */
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