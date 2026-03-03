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