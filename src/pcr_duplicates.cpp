// ============================================================================
// Program: pcr_duplicates
// Author: Nourdine Bah <nourdinebah@gmail.com>
// ============================================================================

#include <map>
#include <set>
#include <fstream>
#include <seqan/arg_parse.h>
#include <seqan/seq_io.h>
#include <seqan/sequence.h>

using namespace seqan;

// ============================================================================
// Classes
// ============================================================================

// This struct stores the options from the command line

struct PcrDuplicatesOptions
{
	CharString fastq;
	CharString sample;
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function parseCommandLine()
// ----------------------------------------------------------------------------

seqan::ArgumentParser::ParseResult
parseCommandLine(PcrDuplicatesOptions& options, int argc, char const** argv)
{
	// Initiatlize ArgumentParser
	seqan::ArgumentParser parser("pcr_duplicates");
	setCategory(parser, "Slide-Seq");
	setShortDescription(parser, "This program remove PCR duplicates");
	setVersion(parser, "1.0");
	setDate(parser, "Sept 2022");

	// Add use and description lines
	addUsageLine(parser, "[\\fIOPTIONS\\fP] \\fIFASTQ\\fP ");
	addDescription(parser,
		"This programs takes a FASTQ (containing also barcode UMI) and removes "
		"PCR duplicates (same barcode, UMI and sequence). It will output a new "
		"FASTQ whose name is base on the sample name and a CSV with the counts."
	);

	// Add positional arguments and set their valid file types
	addArgument(parser, ArgParseArgument(ArgParseArgument::INPUT_FILE, "FASTQ"));
	setValidValues(parser, 0, "fq fq.gz fastq fastq.gz");

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
	getArgumentValue(options.fastq, parser, 0);

	// Export options
	getOptionValue(options.sample, parser, "sample");

	return seqan::ArgumentParser::PARSE_OK;
}

// ----------------------------------------------------------------------------
// Function Select()
// ----------------------------------------------------------------------------

std::set<unsigned long long>
Select(
		char *path,
		std::map<std::string, std::set<unsigned long long>>& counts,
		bool test=false)
{
	SeqFileIn fastq(path);

	// Read name, sequence and quality
	CharString meta;
	CharString sequence;
	CharString qual;

	// Sequences
	std::string barcode;
	std::string up_primer;
	std::string umi;

	std::string regex("^[^:]+:([A-Z]+):[^:]+:[^:]+:([A-Z]+):([A-Z]+):.*$");
	std::regex rgx(regex);
	std::smatch m;

	// Position in the file
	unsigned long long position = 0;

	while ( ! atEnd(fastq) )
	{
		// Extract read name, sequence and quality
		readRecord(meta, sequence, qual, fastq);

		// Parse the name and gets the information
		std::string read_id( toCString(meta) );
		if ( std::regex_match(read_id, m, rgx) )
		{
			up_primer = m[1];
			barcode = m[2];
			umi = m[3];
		}
		else
		{
			std::cerr << "Unable to match regex" << std::endl;
			std::cerr << regex << std::endl;
			std::cerr << meta << std::endl;
			exit(EXIT_FAILURE);
		}

		// (barcode, umi, sequence)
		std::string sequence_str( toCString(sequence) );
		std::string seq = barcode + umi + sequence_str;
		counts[seq].insert(position);

		position++;
		if ( (position+1) % (unsigned long long)1000000 == 0 ) {
			std::cerr << "Counting " << (position+1) << "..." << std::endl;
		}
		if (test) { if ( position > 999999 ) { break; } }
	}

	close(fastq);

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

void Filter(
	char *in_path, char *out_path,
	std::set<unsigned long long> positions,
	bool test=false)
{
	SeqFileIn in_fastq(in_path);
	SeqFileOut out_fastq(out_path);

	// Read name, sequence and quality
	CharString meta;
	CharString sequence;
	CharString qual;


	// Position in the file
	unsigned long long position = 0;

	while ( ! atEnd(in_fastq) )
	{
		// Extract read name, sequence and quality
		readRecord(meta, sequence, qual, in_fastq);

		if ( positions.find(position) != positions.end() )
		{
			writeRecord(out_fastq, meta, sequence, qual);
		}

		position++;
		if ( (position+1) % (unsigned long long)1000000 == 0 ) {
			std::cerr << "Filtering " << (position+1) << "..." << std::endl;
		}
		if (test) { if ( position > 999999 ) { break; } }
	}

	close(in_fastq);
	close(out_fastq);
}

// ----------------------------------------------------------------------------
// Function main()
// ----------------------------------------------------------------------------

// Program entry point

int main(int argc, char const** argv)
{
	// Parse command line
	seqan::ArgumentParser parser;
	PcrDuplicatesOptions options;
	seqan::ArgumentParser::ParseResult res = parseCommandLine(options, argc, argv);

	// If there was an error then the programs exits. The return code is 1 if
	// there were errors and 0 if there were none
	if (res != seqan::ArgumentParser::PARSE_OK)
	{
		return res == seqan::ArgumentParser::PARSE_ERROR;
	}

	bool test = false;

	CharString fastq_path(options.sample);
	append(fastq_path, ".remove_dups.fastq.gz");

	// The positions for each (barcode, umi, sequence)
	std::map<std::string, std::set<unsigned long long>> counts;

	// Group the reads based on their sequence
	std::set<unsigned long long> positions = Select(toCString(options.fastq), counts, test);

	// Remove PCR duplicates
	Filter(toCString(options.fastq), toCString(fastq_path), positions, test);

	///////////////////////////////////////////////////////////////////////////
	// Export counts
	
	unsigned long long total = 0;
	for (auto it=counts.begin(); it!=counts.end(); ++it)
	{
		total += it->second.size();
	}

	CharString csv_path(options.sample);
	append(csv_path, ".remove_dups.csv");
	std::ofstream csv( toCString(csv_path) );
	csv << options.sample << ",Duplicates," << total - positions.size() << std::endl;
	csv << options.sample << ",Total," << total << std::endl;
	csv.close();

	return 0;
}

