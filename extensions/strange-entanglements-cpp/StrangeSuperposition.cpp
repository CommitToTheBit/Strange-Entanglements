#include "StrangeSuperposition.h"

// -----------------------------------------------------------------------------
// StrangeSuperposition::StrangeSuperposition: Constructor.
// -----------------------------------------------------------------------------
StrangeSuperposition::StrangeSuperposition(size_t qubits)
	: mQubits(qubits)
	, mDimensions(1 << qubits)
{
	mData = make_unique<complex<double>[]>(mDimensions);
	memset(mData.get(), 0.0, mDimensions * sizeof(complex<double>));
}

// -----------------------------------------------------------------------------
// StrangeSuperposition::operator==: Equality operator.
// -----------------------------------------------------------------------------
bool StrangeSuperposition::operator==(StrangeSuperposition const& other)
{
	if (mQubits == other.mQubits)
	{
		for (size_t dimension = 0; dimension < mDimensions; ++dimension)
		{
			if (mData[dimension] != other.mData[dimension])
			{
				return false;
			}
		}

		return true;
	}

	return true;
}

// -----------------------------------------------------------------------------
// StrangeSuperposition::IsNonzero: Returns true if any element is nonzero.
// -----------------------------------------------------------------------------
bool StrangeSuperposition::IsNonzero()
{
	for (size_t dimension = 0; dimension < mDimensions; ++dimension)
	{
		if (abs(mData[dimension]))
		{
			return true;
		}
	}

	return false;
}