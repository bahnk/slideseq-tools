/*
 * nourdinebah@gmail.com
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>

#include "mapping.hpp"

// ============================================================================
// Constructors
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

Mapping::Mapping()
{
}

// ----------------------------------------------------------------------------
// Minimal constructor
// ----------------------------------------------------------------------------

Mapping::Mapping(int32_t reference, int32_t position, int score, char* gene)
{
	this->reference = reference;
	this->position = position;
	this->scores = std::set<int>{score};
	this->genes = std::set<std::string>{std::string(gene)};
}

// ----------------------------------------------------------------------------
// Copy constructor
// ----------------------------------------------------------------------------

Mapping::Mapping(const Mapping& mapping)
{
	this->reference = mapping.GetReference();
	this->position = mapping.GetPosition();
	this->scores = mapping.GetScores();
	this->genes = mapping.GetGenes();
}

// ============================================================================
// Getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetReference()
// ----------------------------------------------------------------------------

int32_t Mapping::GetReference() const
{
	return reference;
}

// ----------------------------------------------------------------------------
// GetPosition()
// ----------------------------------------------------------------------------

int32_t Mapping::GetPosition() const
{
	return position;
}

// ----------------------------------------------------------------------------
// GetScores()
// ----------------------------------------------------------------------------

std::set<int> Mapping::GetScores() const
{
	return scores;
}

// ----------------------------------------------------------------------------
// GetGene()
// ----------------------------------------------------------------------------

std::set<std::string> Mapping::GetGenes() const
{
	return genes;
}

// ============================================================================
// Methods
// ============================================================================

// ----------------------------------------------------------------------------
// GetScoresString()
// ----------------------------------------------------------------------------

std::string Mapping::GetScoresString() const
{
	unsigned long long i = 0;
	std::string str = "";

	for (auto& score : scores)
	{
		str.append( std::to_string(score) );
		i++;

		if ( i < scores.size() )
		{
			str.append(", ");
		}
	}

	return "<" + str + ">";
}

// ----------------------------------------------------------------------------
// GetGenesString()
// ----------------------------------------------------------------------------

std::string Mapping::GetGenesString() const
{
	unsigned long long i = 0;
	std::string str = "";

	for (auto& gene : genes)
	{
		str.append(gene);
		i++;

		if ( i < genes.size() )
		{
			str.append(", ");
		}
	}

	return "<" + str + ">";
}

// ----------------------------------------------------------------------------
// InsertScore()
// ----------------------------------------------------------------------------

void Mapping::InsertScore(int score)
{
	scores.insert(score);
}

// ----------------------------------------------------------------------------
// InsertGene()
// ----------------------------------------------------------------------------

void Mapping::InsertGene(std::string gene)
{
	genes.insert(gene);
}

// ----------------------------------------------------------------------------
// IsGeneUnique()
// ----------------------------------------------------------------------------

bool Mapping::IsGeneUnique() const
{
	return genes.size() == 1;
}

// ----------------------------------------------------------------------------
// GetMaxScore()
// ----------------------------------------------------------------------------

int Mapping::GetMaxScore() const
{
	std::set<int>::iterator max_score;
	max_score = std::max_element(scores.begin(), scores.end());
	return *max_score;
}

// ----------------------------------------------------------------------------
// GetGene()
// ----------------------------------------------------------------------------

std::string Mapping::GetGene() const
{
	if ( genes.size() == 1 )
	{
		std::vector<std::string> v(genes.begin(), genes.end());
		return v[0];
	}
	
	else
	{
		throw( genes.size() );
	}
}

// ============================================================================
// Operators
// ============================================================================

// ----------------------------------------------------------------------------
// ostream
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Mapping& mapping)
{
	out
		<< "("
		<< mapping.GetReference() << ", "
		<< mapping.GetPosition() << ", "
		<< mapping.GetScoresString() << ", "
		<< mapping.GetGenesString()
		<< ")";

	return out;
}

// ----------------------------------------------------------------------------
// lesser than
// ----------------------------------------------------------------------------

bool operator<(const Mapping& mapping1, const Mapping& mapping2)
{
	int32_t ref1 = mapping1.GetReference();
	int32_t ref2 = mapping2.GetReference();

	int32_t pos1 = mapping1.GetPosition();
	int32_t pos2 = mapping2.GetPosition();

	return ( ref1 < ref2 ) || ( ref1 == ref2 && pos1 < pos2 );
}

// ----------------------------------------------------------------------------
// plus
// ----------------------------------------------------------------------------

Mapping operator+(const Mapping& mapping1, const Mapping& mapping2)
{
	if (
		mapping1.GetReference() ==  mapping2.GetReference() &&
		mapping1.GetPosition() ==  mapping2.GetPosition()
	)
	{
		Mapping mapping = Mapping(mapping1);

		for (auto& score : mapping2.GetScores())
		{
			mapping.InsertScore(score);
		}

		for (auto& gene : mapping2.GetGenes())
		{
			mapping.InsertGene(gene);
		}

		return mapping;
	}
	else
	{
		throw( std::make_tuple(mapping1, mapping2) );
	}
}

