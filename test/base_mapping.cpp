/*
 * nourdinebah@gmail.com
 */

#include <tuple>
#include <set>
#include <iostream>

#include "base_mapping.hpp"

int main(int argc, char** argv)
{
	////////////////////////////////////////////////////////////////////////////
	
	BaseMapping mapping1 = BaseMapping("AAA", "GA", 1, 50);
	BaseMapping mapping2 = BaseMapping("CAA", "GA", 1, 50);
	BaseMapping mapping3 = BaseMapping("AAA", "CA", 1, 50);
	BaseMapping mapping4 = BaseMapping("TTA", "AA", 2, 400);
	BaseMapping mapping5 = BaseMapping("TTA", "AA", 2, 40);
	BaseMapping mapping6 = BaseMapping("TTA", "AA", 2, 350);
	BaseMapping mapping7 = BaseMapping("CGA", "CC", 3, 490);
	BaseMapping mapping8 = BaseMapping("CGA", "AC", 3, 400);
	BaseMapping mapping9 = BaseMapping("GGA", "AC", 3, 400);
	
	std::cout << std::endl;
	std::cout << "Test of the ostream" << std::endl;
	std::cout << mapping1 << std::endl;
	std::cout << mapping2 << std::endl;
	std::cout << mapping3 << std::endl;
	std::cout << mapping4 << std::endl;
	std::cout << mapping5 << std::endl;
	std::cout << mapping6 << std::endl;
	std::cout << mapping7 << std::endl;
	std::cout << mapping8 << std::endl;
	std::cout << mapping9 << std::endl;

	////////////////////////////////////////////////////////////////////////////
	
	std::cout << std::endl;
	std::cout << "Test of the copy constructor:" << std::endl;
	BaseMapping copied_mapping = BaseMapping(mapping1);
	std::cout << mapping1 << std::endl;
	std::cout << copied_mapping << std::endl;

	////////////////////////////////////////////////////////////////////////////


	std::cout << std::endl;
	std::cout << "Test of the GetReference() method:" << std::endl;
	std::cout << mapping1 << ", " << mapping1.GetReference() << std::endl;

	std::cout << std::endl;
	std::cout << "Test of the GetPosition() method:" << std::endl;
	std::cout << mapping1 << ", " << mapping1.GetPosition() << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test putting mappings in a set:" << std::endl;
	std::set<BaseMapping> mappings;
	mappings.insert(mapping1);
	mappings.insert(mapping2);
	mappings.insert(mapping3);
	mappings.insert(mapping4);
	mappings.insert(mapping5);
	mappings.insert(mapping6);
	mappings.insert(mapping7);
	mappings.insert(mapping8);
	mappings.insert(mapping9);
	for (auto& m : mappings)
	{
		std::cout << m << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	return 0;
}

