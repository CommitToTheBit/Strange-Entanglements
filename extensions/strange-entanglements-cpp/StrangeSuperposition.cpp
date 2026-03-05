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
bool StrangeSuperposition::operator==(StrangeSuperposition const& other) const
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
bool StrangeSuperposition::IsNonzero() const
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

// ----------------------------------------------------------------------------
// StrangeSuperposition::GetRepresentation: Print representation of a
// superposition.
// ----------------------------------------------------------------------------
string StrangeSuperposition::GetRepresentation() const
{
	string representation = string{ };
	for (size_t dimension = 0; dimension < mDimensions; ++dimension)
	{
		if (mData[dimension] != 0.0)
		{
			string coefficient = string{ };
			if (mData[dimension].imag())
			{
				coefficient = "(" + to_string(mData[dimension].real()).substr(0, 5 + (mData[dimension].real() < 0.0)) + " + " + to_string(mData[dimension].imag()).substr(0, 5 + (mData[dimension].real() < 0.0)) + "i)";
			}
			else if (mData[dimension].real() == -1.0)
			{
				coefficient = "-";
			}
			else if (mData[dimension].real() != 1.0)
			{
				coefficient = to_string(mData[dimension].real()).substr(0, 5 + (mData[dimension].real() < 0.0));
			}

			representation.append(coefficient).append(GetDimensionRepresentation(dimension)).append(" + ");
		}
	}

	return representation.length() ? representation.substr(0, representation.length() - 3) : "0";
}

// ----------------------------------------------------------------------------
// StrangeSuperposition::GetDimensionRepresentation: Print representation of
// a basis vector in a superposition.
// ----------------------------------------------------------------------------
string StrangeSuperposition::GetDimensionRepresentation(size_t dimension) const
{
    if (mQubits)
    {
		string dimension_representation = string{ };
        for (int i = 0; i < mQubits; ++i)
        {
            dimension_representation = std::to_string(dimension & 1) + dimension_representation;
            dimension >>= 1;
        }

		return "<" + dimension_representation + ">";
    }
    else
    {
    	return "<∅>";
    }
}