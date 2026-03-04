#ifndef STRANGE_SUPERPOSITION_H
#define STRANGE_SUPERPOSITION_H

#include <complex>
#include <memory>

using namespace std;

// -----------------------------------------------------------------------------
// StrangeSuperposition: Struct to hold superposition data.
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
	StrangeSuperposition(size_t qubits);

	// -------------------------------------------------------------------------
	// Operators
	// -------------------------------------------------------------------------
	bool operator==(StrangeSuperposition const& other);

	// -------------------------------------------------------------------------
	// Accessors
	// -------------------------------------------------------------------------
	bool IsNonzero();
};

#endif // STRANGE_SUPERPOSITION_H