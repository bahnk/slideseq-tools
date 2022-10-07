/*
 * nourdinebah@gmail.com
 */

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

#include "record.hpp"
#include "mapping.hpp"
#include "mappings.hpp"
#include "molecule.hpp"

// ============================================================================
// Constructors
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

Molecule::Molecule()
{
}

// ----------------------------------------------------------------------------
// Minimal constructor
// ----------------------------------------------------------------------------

Molecule::Molecule(char* barcode, char* umi, Record record)
{
	this->barcode = std::string(barcode);
	this->umi = std::string(umi);
	this->sequence = this->barcode + this->umi;
	this->records = {record};
}

// ----------------------------------------------------------------------------
// Copy constructor
// ----------------------------------------------------------------------------

Molecule::Molecule(const Molecule& molecule)
{
	this->barcode = molecule.GetBarcode();
	this->umi = molecule.GetUMI();
	this->sequence = molecule.GetSeq();
	this->records = std::set<Record>();
	for (auto& rec : molecule.records) // the access modifiers work on class level, and not on object level!
	{
		this->records.insert(rec);
	}
}

// ============================================================================
// Getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetBarcode()
// ----------------------------------------------------------------------------

std::string Molecule::GetBarcode() const
{
	return barcode;
}

// ----------------------------------------------------------------------------
// GetUMI()
// ----------------------------------------------------------------------------

std::string Molecule::GetUMI() const
{
	return umi;
}

// ----------------------------------------------------------------------------
// GetSeq()
// ----------------------------------------------------------------------------

std::string Molecule::GetSeq() const
{
	return sequence;
}

// ----------------------------------------------------------------------------
// GetStatus()
// ----------------------------------------------------------------------------

std::string Molecule::GetStatus() const
{
	return status;
}

// ----------------------------------------------------------------------------
// GetRecords()
// ----------------------------------------------------------------------------

std::set<Record> Molecule::GetRecords() const
{
	return records;
}

// ----------------------------------------------------------------------------
// GetMappings()
// ----------------------------------------------------------------------------

Mappings Molecule::GetMappings() const
{
	return mappings;
}

// ============================================================================
// Secondary getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetReads()
// ----------------------------------------------------------------------------

std::set<std::string> Molecule::GetReads() const
{
	std::set<std::string> reads;

	for (auto& record : records)
	{
		reads.insert( record.GetRead() );
	}

	return reads;
}

// ----------------------------------------------------------------------------
// GetRecordString()
// ----------------------------------------------------------------------------

std::string Molecule::GetRecordString() const
{
	unsigned long long i = 0;
	std::string str = "";

	for (auto& rec: records)
	{
		str.append( std::to_string(rec.GetIndex()) );

		i++;

		if ( i < records.size() )
		{
			str.append(":");
		}
	}

	return str;
}

// ----------------------------------------------------------------------------
// GetGenes()
// ----------------------------------------------------------------------------

std::set<std::string> Molecule::GetGenes() const
{
	std::set<std::string> genes;
	for (auto& rec : records)
	{
		genes.insert( rec.GetGene() );
	}
	return genes;
}

// ----------------------------------------------------------------------------
// GetGeneString()
// ----------------------------------------------------------------------------

std::string Molecule::GetGeneString() const
{
	unsigned long long i = 0;
	std::string str = "";
	std::set<std::string> genes = GetGenes();

	for (auto& gene: genes)
	{
		str.append(gene);

		i++;

		if ( i < genes.size() )
		{
			str.append(":");
		}
	}

	return str;
}

// ----------------------------------------------------------------------------
// GetCSVString()
// ----------------------------------------------------------------------------

std::string Molecule::GetCSVString(char molecule_delimiter, char record_delimiter) const
{
	std::string str;
	unsigned long long i = 0;

	for (auto& record : records)
	{
		str.append( record.GetCSVString(record_delimiter) );
		i++;
		if ( i < records.size() )
		{
			str.push_back(molecule_delimiter);
		}
	}

	return str;
}

// ============================================================================
// Methods
// ============================================================================

// ----------------------------------------------------------------------------
// Insert()
// ----------------------------------------------------------------------------

void Molecule::Insert(Record record)
{
	records.insert(record);
}

// ============================================================================
// Mappings methods
// ============================================================================

// ----------------------------------------------------------------------------
// ExtractMappings()
// ----------------------------------------------------------------------------

void Molecule::ExtractMappings()
{
	Mappings mappings;

	for (auto& rec : records)
	{
		Mapping mapping = Mapping(
			rec.GetReference(),
			rec.GetPos(),
			rec.GetScore(),
			(char*)rec.GetGene().c_str() // I know that's bad, but unlikely to be modified
		);

		mappings.Insert(mapping);
	}

	this->mappings = mappings;
}

// ----------------------------------------------------------------------------
// GetMappings()
// ----------------------------------------------------------------------------

Mappings Molecule::GetMappings(bool extract=false)
{
	if (extract)
	{
		ExtractMappings();
	}

	return mappings;
}

// ----------------------------------------------------------------------------
// GetNumberOfMappings()
// ----------------------------------------------------------------------------

int Molecule::GetNumberOfMappings() const
{
	return mappings.GetSize();
}

// ----------------------------------------------------------------------------
// GetMaxScore()
// ----------------------------------------------------------------------------

long Molecule::GetMaxScore() const
{
	long max = -1;

	for (auto& mapping : mappings)
	{
		if ( mapping.GetMaxScore() > max )
		{
			max = mapping.GetMaxScore();
		}
	}

	return max;
}

// ----------------------------------------------------------------------------
// IsThereAMaxima()
// ----------------------------------------------------------------------------

bool Molecule::IsThereAMaxima() const
{
	int max = GetMaxScore();
	long n = 0;

	for (auto& mapping : mappings)
	{
		if ( mapping.GetMaxScore() == max )
		{
			n++;
		}
	}

	if ( n == 0 )
	{
		std::cerr
			<< "Error: the maximal score value can't be found in the "
			<< *this
			<< " molecule"
			<< std::endl;

		exit(EXIT_FAILURE);
	}
	else if ( n == 1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ----------------------------------------------------------------------------
// GetRecordTags()
// ----------------------------------------------------------------------------

std::map< unsigned long long , std::tuple<std::string, std::string> >
Molecule::GetRecordTags() const
{
	std::map< unsigned long long , std::tuple<std::string, std::string> > map;

	if ( GetGenes().size() == 1 && records.size() == 1 )
	{
		for (auto& rec : records)
		{
			map[rec.GetIndex()] = std::make_tuple("UNIQUE", "SAME_GENE");
		}

		return map;
	}

	if ( IsThereAMaxima() )
	{
		bool found = false;
		long max = GetMaxScore();

		// the possible selected gene
		std::string gene = "";
		for (auto& rec : records)
		{
			if ( rec.GetScore() == max )
			{
				gene = rec.GetGene();
			}
		}

		for (auto& rec : records)
		{
			if ( rec.GetScore() == max )
			{
				if ( ! found )
				{
					map[rec.GetIndex()] = std::make_tuple("INCLUDED", "SAME_GENE");
					found = true;
				}
				else
				{
					// if it's max score then it has to be the same gene
					map[rec.GetIndex()] = std::make_tuple("EXCLUDED", "SAME_GENE");
				}
			}

			else
			{
				if ( rec.GetGene() == gene )
				{
					map[rec.GetIndex()] = std::make_tuple("EXCLUDED", "SAME_GENE");
				}
				else
				{
					map[rec.GetIndex()] = std::make_tuple("EXCLUDED", "DIFFERENT_GENE");
				}
			}
		}
	}

	else
	{
		for (auto& rec : records)
		{
			map[rec.GetIndex()] = std::make_tuple("UNRESOLVED", "UNRESOLVED");
		}
	}

	return map;
}

// ----------------------------------------------------------------------------
// ComputeFrequencies()
// ----------------------------------------------------------------------------

void Molecule::ComputeFrequencies()
{
	std::map<std::string, long long> frequencies;

	for (auto& mapping : mappings)
	{
		try
		{
			frequencies[ mapping.GetGene() ]++;
		}
		catch(unsigned long size)
		{
			std::cerr
				<< "Error: the size of the Mapping "
				<< mapping
				<< " is "
				<< std::to_string(size)
				<< std::endl;
		}
	}

	this->frequencies = frequencies;
}

// ----------------------------------------------------------------------------
// GetFrequenciesString()
// ----------------------------------------------------------------------------

std::string Molecule::GetFrequenciesString() const
{
	unsigned long long i = 0;
	std::string str = "";

	for (auto& [gene, freq]: frequencies)
	{
		str.append( gene + ":" + std::to_string(freq) );

		if ( i < frequencies.size() )
		{
			str.append(", ");
		}
	}

	return "<" + str + ">";
}

// ----------------------------------------------------------------------------
// GetMaxFrequency()
// ----------------------------------------------------------------------------

long long Molecule::GetMaxFrequency() const
{
	long long max = -1;

	for (auto& [gene, frequency] : frequencies)
	{
		if ( frequency > max )
		{
			max = frequency;
		}
	}

	return max;
}

// ----------------------------------------------------------------------------
// IsThereAMajority()
// ----------------------------------------------------------------------------

bool Molecule::IsThereAMajority() const
{
	long long max = GetMaxFrequency();
	long n = 0;

	for (auto& [gene, frequency] : frequencies)
	{
		if ( frequency == max )
		{
			n++;
		}
	}

	if ( n == 0 )
	{
		std::cerr
			<< "Error: the maximal frequency can't be found in the "
			<< *this
			<< " molecule"
			<< std::endl;

		exit(EXIT_FAILURE);
	}
	else if ( n == 1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ----------------------------------------------------------------------------
// GetFrequencyBasedRecordTags()
// ----------------------------------------------------------------------------

std::map< unsigned long long , std::tuple<std::string, std::string> >
Molecule::GetFrequencyBasedRecordTags()
{
	std::map< unsigned long long , std::tuple<std::string, std::string> > map;

	std::string status;

	if ( GetGenes().size() == 1 && records.size() == 1 )
	{
		status = "UNIQUE";

		for (auto& rec : records)
		{
			map[rec.GetIndex()] = std::make_tuple("UNIQUE", "SAME_GENE");
		}

		return map;
	}

	if ( IsThereAMajority() )
	{
		status = "RESOLVED";

		long long max = GetMaxFrequency();
		std::string selected_gene;
		for (auto& [gene, frequency] : frequencies)
		{
			if ( frequency == max )
			{
				selected_gene = gene;
			}
		}

		bool found = false;
		for (auto& rec : records)
		{
			if ( rec.GetGene() == selected_gene )
			{
				if ( ! found )
				{
					map[rec.GetIndex()] = std::make_tuple("INCLUDED", "SAME_GENE");
					found = true;
				}
				else
				{
					map[rec.GetIndex()] = std::make_tuple("EXCLUDED", "SAME_GENE");
				}
			}
			else
			{
				map[rec.GetIndex()] = std::make_tuple("EXCLUDED", "DIFFERENT_GENE");
			}
		}
	}

	else
	{
		status = "UNRESOLVED";

		for (auto& rec : records)
		{
			map[rec.GetIndex()] = std::make_tuple("UNRESOLVED", "UNRESOLVED");
		}
	}

	this->status = status;

	return map;
}

// ============================================================================
// Operators
// ============================================================================

// ----------------------------------------------------------------------------
// ostream
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Molecule& molecule)
{
	out
		<< "("
		<< molecule.GetBarcode() << ", "
		<< molecule.GetUMI() << ", "
		<< molecule.GetRecordString()
		<< ")";

	return out;
}

// ----------------------------------------------------------------------------
// lesser than
// ----------------------------------------------------------------------------

bool operator<(const Molecule& molecule1, const Molecule& molecule2)
{
	return molecule1.GetSeq() < molecule2.GetSeq();
}

// ----------------------------------------------------------------------------
// plus
// ----------------------------------------------------------------------------

Molecule operator+(Molecule& molecule1, Molecule& molecule2)
{
	if ( molecule1.GetSeq() ==  molecule2.GetSeq() )
	{
		Molecule molecule = Molecule(molecule1);

		for (auto& rec : molecule2)
		{
			molecule.Insert(rec);
		}

		return molecule;
	}
	else
	{
		throw( std::make_tuple(molecule1, molecule2) );
	}
}

// ============================================================================
// Iterators
// ============================================================================

// ----------------------------------------------------------------------------
// begin()
// ----------------------------------------------------------------------------

std::set<Record>::iterator Molecule::begin()
{
	return records.begin();
}

// ----------------------------------------------------------------------------
// end()
// ----------------------------------------------------------------------------

std::set<Record>::iterator Molecule::end()
{
	return records.end();
}

