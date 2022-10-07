/*
 * nourdinebah@gmail.com
 */

#include <set>
#include <iostream>
#include <tuple>

#include "mapping.hpp"

int main(int argc, char** argv)
{
	////////////////////////////////////////////////////////////////////////////
	
	Mapping mapping01 = Mapping(1, 2, 60, "GeneA");
	Mapping mapping02 = Mapping(2, 6, 50, "GeneB");
	Mapping mapping03 = Mapping(6, 1, 60, "GeneA");
	Mapping mapping04 = Mapping(1, 1, 60, "GeneA");
	Mapping mapping05 = Mapping(3, 1, 60, "GeneA");
	Mapping mapping06 = Mapping(1, 1, 60, "GeneA");
	Mapping mapping07 = Mapping(1, 2, 60, "GeneC");
	Mapping mapping08 = Mapping(6, 7, 60, "GeneC");
	Mapping mapping09 = Mapping(1, 2, 65, "GeneC");
	
	std::cout << std::endl;
	std::cout << "Test of the ostream" << std::endl;
	std::cout << mapping01 << std::endl;
	std::cout << mapping02 << std::endl;
	std::cout << mapping03 << std::endl;
	std::cout << mapping04 << std::endl;
	std::cout << mapping05 << std::endl;
	std::cout << mapping06 << std::endl;
	std::cout << mapping07 << std::endl;
	std::cout << mapping08 << std::endl;
	std::cout << mapping09 << std::endl;

	////////////////////////////////////////////////////////////////////////////
	
	std::cout << std::endl;
	std::cout << "Test of the copy constructor:" << std::endl;
	Mapping copied_mapping = Mapping(mapping01);
	std::cout << mapping01 << std::endl;
	std::cout << copied_mapping << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the GetScoresString() method:" << std::endl;
	std::cout << mapping01 << ", " << mapping01.GetScoresString() << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the GetGenesString() method:" << std::endl;
	std::cout << mapping01 << ", " << mapping01.GetGenesString() << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test putting mappings in a set:" << std::endl;
	std::set<Mapping> mappings;
	mappings.insert(mapping01);
	mappings.insert(mapping02);
	mappings.insert(mapping03);
	mappings.insert(mapping04);
	mappings.insert(mapping05);
	mappings.insert(mapping06);
	mappings.insert(mapping07);
	mappings.insert(mapping08);
	mappings.insert(mapping09);
	for (auto& m : mappings)
	{
		std::cout << m << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the + operator:" << std::endl;
	Mapping plus_mapping;
	try {
		plus_mapping = mapping01 + mapping09;
	}
	catch(std::tuple<Mapping, Mapping> mappings_tuple)
	{
		std::cerr << "It should not fail here" << std::endl;
	}
	std::cout
		<< mapping01
		<< ", "
		<< mapping09
		<< ", "
		<< plus_mapping
		<< std::endl;

	return 0;
}

