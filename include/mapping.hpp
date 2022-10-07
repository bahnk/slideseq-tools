/*
 * nourdinebah@gmail.com
 */

#ifndef __MAPPING_HPP
#define __MAPPING_HPP

#include <set>
#include <string>
#include <vector>

class Mapping
{
	private:
		int32_t reference;
		int32_t position;
		std::set<int> scores;
		std::set<std::string> genes;
	
	public:
		Mapping();
		Mapping(int32_t, int32_t, int, char*);
		Mapping(const Mapping&);
		int32_t GetReference() const;
		int32_t GetPosition() const;
		std::set<int> GetScores() const;
		std::set<std::string> GetGenes() const;
		std::string GetScoresString() const;
		std::string GetGenesString() const;
		void InsertScore(int);
		void InsertGene(std::string);
		bool IsGeneUnique() const;
		int GetMaxScore() const;
		std::string GetGene() const;
		friend std::ostream& operator<<(std::ostream&, const Mapping&);
		friend bool operator<(const Mapping&, const Mapping&);
		friend Mapping operator+(const Mapping&, const Mapping&);
};

#endif

