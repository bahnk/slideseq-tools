/*
 * nourdine.bah@crick.ac.uk
 */

#include <string>
#include <fstream>

#include "gene.hpp"

// ============================================================================
// Constructors
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

Gene::Gene()
{
}

// ----------------------------------------------------------------------------
// Minimal constructor
// ----------------------------------------------------------------------------

Gene::Gene(char* id, char* symbol)
{
	this->id = std::string(id);
	this->symbol = std::string(symbol);
}

// ----------------------------------------------------------------------------
// Alternative constructor
// ----------------------------------------------------------------------------

Gene::Gene(std::string id, std::string symbol)
{
	this->id = id;
	this->symbol = symbol;
}

// ============================================================================
// Getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetID()
// ----------------------------------------------------------------------------

std::string Gene::GetID() const
{
	return id;
}

// ----------------------------------------------------------------------------
// GetSymbol()
// ----------------------------------------------------------------------------

std::string Gene::GetSymbol() const
{
	return symbol;
}

// ============================================================================
// Operators
// ============================================================================

// ----------------------------------------------------------------------------
// ostream
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Gene& gene)
{
	out << "[" << gene.GetID() << ", " << gene.GetSymbol() << "]";
	return out;
}

// ----------------------------------------------------------------------------
// lesser than
// ----------------------------------------------------------------------------

bool operator<(const Gene& gene1, const Gene& gene2)
{
	return gene1.GetID() < gene2.GetID();
}

