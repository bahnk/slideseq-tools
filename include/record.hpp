/*
 * nourdinebah@gmail.com
 */

#ifndef __RECORD_HPP
#define __RECORD_HPP

#include <string>

class Record
{
	private:
		unsigned long long index;
		std::string read;
		int32_t reference;
		int32_t position;
		long score;
		std::string gene;
	
	public:
		Record();
		Record(unsigned long long, char*, int32_t, int32_t, long, char*);
		unsigned long long GetIndex() const;
		std::string GetRead() const;
		int32_t GetReference() const;
		int32_t GetPos() const;
		long GetScore() const;
		std::string GetGene() const;
		std::string GetCSVString(char) const;
		friend std::ostream& operator<<(std::ostream&, const Record&);
		friend bool operator<(const Record&, const Record&);
};

#endif

