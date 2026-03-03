#ifndef STRANGE_SUPERPOSITION_H
#define STRANGE_SUPERPOSITION_H

#include <complex>
#include <memory>

using namespace std;

// -----------------------------------------------------------------------------
// StrangeSuperposition: 
// -----------------------------------------------------------------------------
struct StrangeSuperposition
{
public:
	// -------------------------------------------------------------------------
	// State
	// -------------------------------------------------------------------------
	unique_ptr<complex<double>[]> mData = nullptr;
	size_t const mDimensions;
	size_t const mQubits;

public:
	// -------------------------------------------------------------------------
	// Constructors/Destructor
	// -------------------------------------------------------------------------
	StrangeSuperposition(int qubits);
};

#endif // STRANGE_SUPERPOSITION_H