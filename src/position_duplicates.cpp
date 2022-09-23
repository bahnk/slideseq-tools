// ============================================================================
// Program: position_duplicates
// Author: Nourdine Bah <nourdinebah@gmail.com>
// ============================================================================

#include <tuple>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <seqan/basic.h>
#include <seqan/arg_parse.h>
#include <seqan/bam_io.h>

#include "utils.hpp"
#include "base_mapping.hpp"

using namespace seqan;

// ============================================================================
// Classes
// ============================================================================

// This struct stores the options from the command line

struct PositionDuplicatesOptions
{
	CharString bam;
	CharString sample;
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function parseCommandLine()
// ----------------------------------------------------------------------------

seqan::ArgumentParser::ParseResult
parseCommandLine(PositionDuplicatesOptions& options, int argc, char const** argv)
{
	// Initiatlize ArgumentParser
	seqan::ArgumentParser parser("position_duplicates");
	setCategory(parser, "Slide-Seq");
	setShortDescription(parser, "This program converts record names to bam tags");
	setVersion(parser, "1.0");
	setDate(parser, "Sept 2022");

	// Add use and description lines
	addUsageLine(parser, "[\\fIOPTIONS\\fP] \\fIBAM\\fP ");
	addDescription(parser,
		"The programs remove the position duplicates. "
		"Position duplicates are alignment records with the same "
		"(barcode, umi, reference, position) tuple. "
		"The program takes a BAM file as input, filters the duplicates and "
		"outputs them in another BAM file. "
		"The programs also outputs a CSV file containing the number of "
		"duplicates. "
		"The output files are name based on the sample option."
	);

	// Add positional arguments and set their valid file types
	addArgument(parser, ArgParseArgument(ArgParseArgument::INPUT_FILE, "BAM"));
	setValidValues(parser, 0, "bam BAM sam SAM");

	// Add section with sample information
	addSection(parser, "Sample");
	addOption(parser, ArgParseOption("s", "sample", "The sample name", ArgParseArgument::STRING));
	setDefaultValue(parser, "s", "Sample");

	// Parse the arguments
	ArgumentParser::ParseResult res = parse(parser, argc, argv);

	// Only extract options. The program continues after parseCommandLine()
	if (res != ArgumentParser::PARSE_OK)
	{
		return res;
	}

	// Export arguments
	getArgumentValue(options.bam, parser, 0);

	// Export options
	getOptionValue(options.sample, parser, "sample");
	return seqan::ArgumentParser::PARSE_OK;
}

// ----------------------------------------------------------------------------
// Function Select()
// ----------------------------------------------------------------------------

std::set<unsigned long long>
Select(
	CharString bam_path,
	std::map<BaseMapping, std::set<unsigned long long>>& counts,
	bool test=false)
{
	// BAM files
	BamFileIn bam(toCString(bam_path));
	BamHeader header;
	readHeader(header, bam);
	BamAlignmentRecord record;

	// BAM tags
	CharString barcode;
	CharString umi;

	unsigned long long position = 0;

	while ( ! atEnd(bam) )
	{
		// Current record
		readRecord(record, bam);

		BamTagsDict dict(record.tags);

		ExtractTag("bc", dict, barcode);
		ExtractTag("mi", dict, umi);

		BaseMapping mapping = BaseMapping(
			toCString(barcode),
			toCString(umi),
			record.rID,
			record.beginPos);

		counts[mapping].insert(position);

		position++;
		if ( (position+1) % (unsigned long long)1000000 == 0 ) {
			std::cerr << "Counting " << (position+1) << "..." << std::endl;
		}
		if (test) { if ( position > 999999 ) { break; } }
	}

	close(bam);

	// Return the positions
	std::set<unsigned long long> positions;
	for (auto it=counts.begin(); it!=counts.end(); ++it)
	{
		positions.insert(*it->second.begin());
	}

	return positions;
}

// ----------------------------------------------------------------------------
// Function Filter()
// ----------------------------------------------------------------------------

void
Filter(
	CharString bam_in_path,
	CharString bam_out_path,
	std::set<unsigned long long> positions,
	bool test=false)
{
	// BAM files
	BamFileIn bam_in(toCString(bam_in_path));
	BamFileOut bam_out(context(bam_in), toCString(bam_out_path));
	BamHeader header;
	readHeader(header, bam_in);
	writeHeader(bam_out, header);
	BamAlignmentRecord record;

	unsigned long long position = 0;

	while ( ! atEnd(bam_in) )
	{
		// Current record
		readRecord(record, bam_in);

		if ( positions.find(position) != positions.end() )
		{
			writeRecord(bam_out, record);
		}

		position++;
		if ( (position+1) % (unsigned long long)1000000 == 0 ) {
			std::cerr << "Filtering " << (position+1) << "..." << std::endl;
		}
		if (test) { if ( position > 999999 ) { break; } }
	}

	close(bam_in);
	close(bam_out);
}

// ----------------------------------------------------------------------------
// Function main()
// ----------------------------------------------------------------------------

// Program entry point

int main(int argc, char const** argv)
{
	// Parse command line
	seqan::ArgumentParser parser;
	PositionDuplicatesOptions options;
	seqan::ArgumentParser::ParseResult res = parseCommandLine(options, argc, argv);

	// If there was an error then the programs exits. The return code is 1 if
	// there were errors and 0 if there were none
	if (res != seqan::ArgumentParser::PARSE_OK)
	{
		return res == seqan::ArgumentParser::PARSE_ERROR;
	}

	bool test = false;

	// The positions for each (barcode, umi, sequence)
	std::map<BaseMapping, std::set<unsigned long long>> counts;

	// Group the reads based on their sequence
	std::set<unsigned long long> positions = Select(options.bam, counts, test);

	// Output BAM file
	CharString bam_path(options.sample);
	append(bam_path, ".pos_dups.bam");

	// Remove position duplicates
	Filter(options.bam, bam_path, positions, test);

	///////////////////////////////////////////////////////////////////////////
	// Export counts

	unsigned long long total = 0;
	for (auto it=counts.begin(); it!=counts.end(); ++it)
	{
		total += it->second.size();
	}

	CharString csv_path(options.sample);
	append(csv_path, ".pos_dups.csv");
	std::ofstream csv( toCString(csv_path) );
	csv << options.sample << ",Duplicates," << total - positions.size() << std::endl;
	csv << options.sample << ",Total," << total << std::endl;
	csv.close();

	return 0;
}

