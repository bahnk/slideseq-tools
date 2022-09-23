/*
 * nourdinebah@gmail.com
 */

#ifndef __BASE_MAPPING_HPP
#define __BASE_MAPPING_HPP

#include <string>
#include <vector>

class BaseMapping
{
	private:
		std::string barcode;
		std::string umi;
		int32_t reference;
		int32_t position;
	
	public:
		BaseMapping();
		BaseMapping(char *barcode, char* umi, int32_t, int32_t);
		BaseMapping(const BaseMapping&);
		std::string GetBarcode() const;
		std::string GetUmi() const;
		int32_t GetReference() const;
		int32_t GetPosition() const;
		friend std::ostream& operator<<(std::ostream&, const BaseMapping&);
		friend bool operator<(const BaseMapping&, const BaseMapping&);
};

#endif

