/*
 * nourdinebah@gmail.com
 */

#include <string>
#include <fstream>

#include "record.hpp"

// ============================================================================
// Constructors
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

Record::Record()
{
}

// ----------------------------------------------------------------------------
// Minimal constructor
// ----------------------------------------------------------------------------

Record::Record(unsigned long long position, char* read, long score, char* gene)
{
	this->position = position;
	this->read = std::string(read);
	this->score = score;
	this->gene = std::string(gene);
}

// ============================================================================
// Getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetPos()
// ----------------------------------------------------------------------------

unsigned long long Record::GetPos() const
{
	return position;
}

// ----------------------------------------------------------------------------
// GetRead()
// ----------------------------------------------------------------------------

std::string Record::GetRead() const
{
	return read;
}

// ----------------------------------------------------------------------------
// GetScore()
// ----------------------------------------------------------------------------

long Record::GetScore() const
{
	return score;
}

// ----------------------------------------------------------------------------
// GetGene()
// ----------------------------------------------------------------------------

std::string Record::GetGene() const
{
	return gene;
}

// ============================================================================
// Methods
// ============================================================================

// ----------------------------------------------------------------------------
// GetCSVString()
// ----------------------------------------------------------------------------

std::string Record::GetCSVString(char delimiter) const
{
	std::string delim(1, delimiter);
	return GetRead() + delim + GetGene() + delim + std::to_string(GetScore());
}

// ============================================================================
// Operators
// ============================================================================

// ----------------------------------------------------------------------------
// ostream
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Record& record)
{
	out << "(" << record.GetPos() << ", " << record.GetScore() << ", " << record.GetGene() << ")";
	return out;
}

// ----------------------------------------------------------------------------
// lesser than
// ----------------------------------------------------------------------------

bool operator<(const Record& record1, const Record& record2)
{
	return record1.GetPos() < record2.GetPos();
}

