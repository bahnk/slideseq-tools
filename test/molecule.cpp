/*
 * nourdinebah@gmail.com
 */

#include <tuple>
#include <iostream>

#include "molecule.hpp"

int main(int argc, char** argv)
{
	////////////////////////////////////////////////////////////////////////////

	Record rec01 = Record(2, "read1", 1, 5, 10, "GeneA");
	Record rec02 = Record(3, "read1", 4, 8, 14, "GeneA");
	Record rec03 = Record(4, "read2", 1, 5, 14, "GeneA");
	Record rec04 = Record(4, "read2", 5, 5, 14, "GeneB");
	Record rec05 = Record(4, "read3", 1, 7, 15, "GeneB");
	Record rec06 = Record(4, "read3", 1, 5, 14, "GeneA");
	Record rec07 = Record(5, "read3", 2, 2, 14, "GeneC");
	Record rec08 = Record(6, "read3", 1, 5, 18, "GeneA");
	Record rec09 = Record(7, "read3", 1, 1, 20, "GeneD");
	Record rec10 = Record(8, "read3", 3, 1, 20, "GeneD");
	Record rec11 = Record(9, "read3", 3, 5, 20, "GeneD");

	////////////////////////////////////////////////////////////////////////////

	Molecule molecule = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec01);
	molecule.Insert(rec02);
	molecule.Insert(rec03);
	molecule.Insert(rec04);
	molecule.Insert(rec05);
	molecule.Insert(rec06);
	molecule.Insert(rec07);
	molecule.Insert(rec08);
	molecule.Insert(rec09);

	std::cout << std::endl;
	std::cout << "Test of the ostream:" << std::endl;
	std::cout << molecule << std::endl;

	std::cout << std::endl;
	std::cout << "Test to display all the records:" << std::endl;
	for (auto& rec : molecule)
	{
		std::cout << rec << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Test for the GetGenes() method:" << std::endl;
	for (auto& gene : molecule.GetGenes())
	{
		std::cout << gene << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Test for the GetGeneString() method:" << std::endl;
	std::cout << molecule.GetGeneString() << std::endl;

	////////////////////////////////////////////////////////////////////////////
	
	std::cout << std::endl;
	std::cout << "Test of the copy constructor:" << std::endl;
	Molecule mol = Molecule(molecule);
	std::cout << mol << std::endl;
	std::cout << molecule << std::endl;

	////////////////////////////////////////////////////////////////////////////
	
	Molecule mol1 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec01);
	Molecule mol2 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec02);
	Molecule mol3 = Molecule("TGCATGCATGCATGCA", "ACGTACGT", rec03);

	std::cout << std::endl;
	std::cout << "Test of the + operator with same barcodes:" << std::endl;
	Molecule m_mol1;
	try {
		m_mol1 = mol1 + mol2;
	}
	catch(std::tuple<Molecule, Molecule> molecules)
	{
		std::cerr << "It should not fail here" << std::endl;
	}
	std::cout << mol1 << ", " << mol2 << ", " << m_mol1 << std::endl;

	std::cout << std::endl;
	std::cout << "Test of the + operator with different barcodes:" << std::endl;
	Molecule m_mol2;
	try
	{
		m_mol2 = mol2 + mol3;
	}
	catch(std::tuple<Molecule, Molecule> molecules)
	{
		std::cerr << "It should fail here" << std::endl;
		std::cerr << "Trying to merge two molecules with different (barcode, UMI) pairs:" << std::endl;
		std::cerr << std::get<0>(molecules) << std::endl;
		std::cerr << std::get<1>(molecules) << std::endl;
	}
	std::cout << mol2 << ", " << mol3 << ", " << m_mol2 << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the ExtractMappings() and GetMappings() methods:" << std::endl;
	Mappings mappings = molecule.GetMappings(true);
	for (auto& m : mappings)
	{
		std::cout << m << ", " << m.GetMaxScore() << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Test of the GetMaxScore() method:" << std::endl;
	std::cout << molecule.GetMaxScore() << std::endl;

	////////////////////////////////////////////////////////////////////////////

	Record r01 = Record(1, "read01", 8, 6, 1, "GeneA");
	Record r02 = Record(2, "read02", 9, 2, 2, "GeneB");
	Record r03 = Record(3, "read03", 2, 6, 3, "GeneC");
	Record r04 = Record(4, "read04", 2, 2, 1, "GeneD");
	Record r05 = Record(5, "read05", 3, 4, 2, "GeneE");
	Record r06 = Record(6, "read06", 2, 2, 3, "GeneD");
	Record r07 = Record(7, "read07", 1, 1, 1, "GeneG");
	Record r08 = Record(8, "read08", 2, 9, 2, "GeneH");
	Record r09 = Record(9, "read09", 1, 6, 3, "GeneI");
	Record r10 = Record(10, "read10", 1, 7, 3, "GeneA");
	Record r11 = Record(11, "read11", 1, 8, 3, "GeneA");
	Record r12 = Record(12, "read11", 1, 8, 2, "GeneC");

	Molecule mm1 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", r01);
	mm1.Insert(r02);
	mm1.Insert(r03);
	mm1.Insert(r04);
	mm1.Insert(r05);

	Molecule mm2 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", r03);
	mm2.Insert(r06);
	mm2.Insert(r08);

	mm1.ExtractMappings();
	mm2.ExtractMappings();

	std::cout << std::endl;
	std::cout << "Test of the IsThereAMaxima() method:" << std::endl;
	std::cout << mm1 << ", this should be true: " << mm1.IsThereAMaxima() << std::endl;
	std::cout << mm2 << ", this should be false :" << mm2.IsThereAMaxima() << std::endl;

	///////////////////////
	std::cout << std::endl;
	std::cout << "Test of the IsThereAMajority() method:" << std::endl;

	mm1.ComputeFrequencies();
	mm2.ComputeFrequencies();

	std::cout
		<< mm1 << ", "
		<< mm1.GetFrequenciesString() << ", "
		<< mm1.IsThereAMajority() << ", "
		<< mm1.GetMaxFrequency()
		<< std::endl;

	std::cout
		<< mm2 << ", "
		<< mm2.GetFrequenciesString() << ", "
		<< mm2.IsThereAMajority() << ", "
		<< mm2.GetMaxFrequency()
		<< std::endl;

	Molecule mm3 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", r01);
	mm3.Insert(r02);
	mm3.Insert(r10);
	mm3.Insert(r11);
	mm3.ExtractMappings();
	mm3.ComputeFrequencies();

	std::cout
		<< mm3 << ", "
		<< mm3.GetFrequenciesString() << ", "
		<< mm3.IsThereAMajority() << ", "
		<< mm3.GetMaxFrequency()
		<< std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the GetRecordTags() method:" << std::endl;

	mm1.Insert(r12);
	for (auto& rec : mm1)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm1.GetRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	for (auto& rec : mm2)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm2.GetRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Molecule mm4 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", r01);
	mm4.ExtractMappings();
	for (auto& rec : mm4)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm4.GetRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Record rr01 = Record(1, "read1", 2, 5, 1, "GeneA");
	Record rr02 = Record(2, "read2", 1, 7, 2, "GeneA");
	Record rr03 = Record(3, "read2", 2, 8, 2, "GeneA");
	Molecule mm5 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rr01);
	mm5.Insert(rr02);
	mm5.Insert(rr03);
	mm5.ExtractMappings();
	for (auto& rec : mm5)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm5.GetRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Molecule mm6 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rr02);
	mm6.Insert(rr03);
	mm6.ExtractMappings();
	for (auto& rec : mm6)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags] : mm6.GetRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the GetFrequencyBasedRecordTags() method:" << std::endl;

	mm1.ComputeFrequencies();
	for (auto& rec : mm1)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm1.GetFrequencyBasedRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	mm2.ComputeFrequencies();
	for (auto& rec : mm2)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags]: mm2.GetFrequencyBasedRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Molecule mm7 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec01);
	mm7.ComputeFrequencies();
	for (auto& rec : mm7)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags] : mm7.GetFrequencyBasedRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Molecule mm8 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec01);
	mm8.Insert(rec03);
	mm8.Insert(rec04);
	mm8.Insert(rec05);
	mm8.Insert(rec06);
	mm8.Insert(rec07);
	mm8.Insert(rec08);
	mm8.Insert(rec09);
	mm8.ExtractMappings();
	mm8.ComputeFrequencies();
	for (auto& rec : mm8)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags] : mm8.GetFrequencyBasedRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	Molecule mm9 = Molecule("ACGTACGTACGTACGT", "ACGTACGT", rec02);
	mm9.Insert(rec03);
	mm9.Insert(rec06);
	mm9.Insert(rec07);
	mm9.ExtractMappings();
	mm9.ComputeFrequencies();
	for (auto& rec : mm9)
	{
		std::cout << rec << ", " << rec.GetScore() << std::endl;
	}
	for (auto& [pos, tags] : mm9.GetFrequencyBasedRecordTags())
	{
		std::cout << pos << ", " << std::get<0>(tags) << ", " << std::get<1>(tags) << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the GetCSVString() method:" << std::endl;
	std::cout << mm7.GetCSVString('|', '/') << std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the ExtractMappings() method:" << std::endl;

	for (auto& rec : mm7.GetRecords())
	{
		std::cout << "Record: " << rec << std::endl;
	}

	mm7.ExtractMappings();

	for (auto& mapping : mm7.GetMappings())
	{
		std::string gene = "";

		try
		{
			gene = mapping.GetGene();
		}
		catch(unsigned long size) {
			gene = "NULL(" + std::to_string(size) + ")";
		}

		std::cout
			<< "Mapping: "
			<< mapping
			<< ", "
			<< mapping.GetMaxScore()
			<< ", "
			<< gene
			<< std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the ComputeFrequencies() method:" << std::endl;

	Molecule mmx = Molecule("ACGTAGGCACGTACGT", "TCCTACGT", rec01);
	mmx.Insert(rec01);
	mmx.Insert(rec02);
	mmx.Insert(rec03);
	mmx.Insert(rec04);
	mmx.Insert(rec05);
	mmx.Insert(rec06);
	mmx.Insert(rec07);
	mmx.Insert(rec08);
	mmx.Insert(rec09);
	mmx.Insert(rec10);
	mmx.Insert(rec11);

	for (auto& rec : mmx.GetRecords())
	{
		std::cout << "Record: " << rec << std::endl;

	}

	mmx.ExtractMappings();
	mmx.ComputeFrequencies();

	std::cout
		<< "Freqs: " << mmx.GetFrequenciesString()
		<< ", Max Freqs: " << mmx.GetMaxFrequency()
		<< std::endl;

	////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
	std::cout << "Test of the IsThereAMajority() method:" << std::endl;
	mm7.ComputeFrequencies();
	std::cout
		<< "Freqs: " << mm7.GetFrequenciesString()
		<< ", majority? " << mm7.IsThereAMajority()
		<< ", max frequency? " << mm7.GetMaxFrequency()
		<< std::endl;
	std::cout
		<< "Freqs: " << mm9.GetFrequenciesString()
		<< ", majority? " << mm9.IsThereAMajority()
		<< ", max frequency? " << mm9.GetMaxFrequency()
		<< std::endl;

	////////////////////////////////////////////////////////////////////////////
	return 0;
}

