/*
 * nourdinebah@gmail.com
 */

#include <iostream>

#include "mapping.hpp"
#include "mappings.hpp"

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
	Mapping mapping10 = Mapping(1, 2, 65, "GeneC");

	Mappings mappings;
	mappings.Insert(mapping01);
	mappings.Insert(mapping02);
	mappings.Insert(mapping03);
	mappings.Insert(mapping04);
	mappings.Insert(mapping05);
	mappings.Insert(mapping06);
	mappings.Insert(mapping07);
	mappings.Insert(mapping08);
	mappings.Insert(mapping09);
	mappings.Insert(mapping10);
	
	////////////////////////////////////////////////////////////////////////////
	
	std::cout << std::endl;
	std::cout << "Test to display all the mappings:" << std::endl;
	for (auto& m : mappings)
	{
		std::cout << m << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	return 0;
}

