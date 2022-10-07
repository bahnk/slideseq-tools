/*
 * nourdinebah@gmail.com
 */

#include <iostream>
#include <set>

#include "record.hpp"

int main(int argc, char** argv)
{
	////////////////////////////////////////////////////////////////////////////
	Record rec1 = Record(2, "read1", 2, 45, 10, "GeneA");
	Record rec2 = Record(3, "read1", 2, 56, 14, "GeneB");
	Record rec3 = Record(4, "read2", 1, 3, 14, "GeneB");
	Record rec4 = Record(4, "read4", 1, 3, 14, "GeneB");
	std::cout << rec1 << std::endl;
	std::cout << rec2 << std::endl;
	std::cout << rec3 << std::endl;
	std::cout << rec4 << std::endl;

	////////////////////////////////////////////////////////////////////////////
	std::set<Record> records;
	records.insert(rec1);
	records.insert(rec1);
	records.insert(rec2);
	records.insert(rec2);
	records.insert(rec3);
	records.insert(rec3);
	records.insert(rec4);
	records.insert(rec4);
	for (auto& rec: records)
	{
		std::cout << "In set, " << rec << std::endl;
	}

	for (auto& rec: records)
	{
		std::cout << rec.GetCSVString('/') << std::endl;
	}

	return 0;
}
