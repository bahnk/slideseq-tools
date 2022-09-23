/*
 * nourdinebah@gmail.com
 */

#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <algorithm>

#include "base_mapping.hpp"

// ============================================================================
// Constructors
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

BaseMapping::BaseMapping()
{
}

// ----------------------------------------------------------------------------
// Minimal constructor
// ----------------------------------------------------------------------------

BaseMapping::BaseMapping(
	char *barcode, char *umi,
	int32_t reference, int32_t position
	)
{
	this->barcode = std::string(barcode);
	this->umi = std::string(umi);
	this->reference = reference;
	this->position = position;
}

// ----------------------------------------------------------------------------
// Copy constructor
// ----------------------------------------------------------------------------

BaseMapping::BaseMapping(const BaseMapping& BaseMapping)
{
	this->barcode = BaseMapping.GetBarcode();
	this->umi = BaseMapping.GetUmi();
	this->reference = BaseMapping.GetReference();
	this->position = BaseMapping.GetPosition();
}

// ============================================================================
// Getters
// ============================================================================

// ----------------------------------------------------------------------------
// GetBarcode()
// ----------------------------------------------------------------------------

std::string BaseMapping::GetBarcode() const
{
	return barcode;
}

// ----------------------------------------------------------------------------
// GetUmi()
// ----------------------------------------------------------------------------

std::string BaseMapping::GetUmi() const
{
	return umi;
}

// ----------------------------------------------------------------------------
// GetReference()
// ----------------------------------------------------------------------------

int32_t BaseMapping::GetReference() const
{
	return reference;
}

// ----------------------------------------------------------------------------
// GetPosition()
// ----------------------------------------------------------------------------

int32_t BaseMapping::GetPosition() const
{
	return position;
}

// ============================================================================
// Operators
// ============================================================================

// ----------------------------------------------------------------------------
// ostream
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const BaseMapping& BaseMapping)
{
	out
		<< "("
		<< BaseMapping.GetBarcode()
		<< ", "
		<< BaseMapping.GetUmi()
		<< ", "
		<< BaseMapping.GetReference()
		<< ", "
		<< BaseMapping.GetPosition()
		<< ")";

	return out;
}

// ----------------------------------------------------------------------------
// lesser than
// ----------------------------------------------------------------------------

bool operator<(const BaseMapping& BaseMapping1, const BaseMapping& BaseMapping2)
{
	std::tuple<std::string, std::string, int32_t, int32_t> t1 =
		std::make_tuple(
			BaseMapping1.GetBarcode(),
			BaseMapping1.GetUmi(),
			BaseMapping1.GetReference(),
			BaseMapping1.GetPosition()
		);

	std::tuple<std::string, std::string, int32_t, int32_t> t2 =
		std::make_tuple(
			BaseMapping2.GetBarcode(),
			BaseMapping2.GetUmi(),
			BaseMapping2.GetReference(),
			BaseMapping2.GetPosition()
			);

	return t1 < t2;
}

