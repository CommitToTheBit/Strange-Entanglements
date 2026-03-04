#ifndef STRANGE_SUPERPOSITION_H
#define STRANGE_SUPERPOSITION_H

#include <complex>
#include <memory>
#include <string>

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
	bool operator==(StrangeSuperposition const& other) const;
	bool operator!=(StrangeSuperposition const& other) const { return !(*this == other); }

	// -------------------------------------------------------------------------
	// Accessors
	// -------------------------------------------------------------------------
	bool IsNonzero() const;

	// -------------------------------------------------------------------------
	// Representation
	// -------------------------------------------------------------------------
	size_t GetQubitRepresentation(size_t qubit) const { return 1 << mQubits - qubit - 1; }
	size_t GetMeasurementRepresentation(size_t qubit, size_t measurement) const { return measurement << mQubits - qubit - 1; }

	string GetDimensionRepresentation(size_t dimension) const;
	string GetRepresentation() const;
};

#endif // STRANGE_SUPERPOSITION_H