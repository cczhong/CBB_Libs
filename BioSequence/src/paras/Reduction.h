#ifndef __REDUCTION_H__
#define __REDUCTION_H__

#include "Value.h"
#include "../tool/Utils.h"

using std::string;
using std::vector;

template<typename _val>
class Reduction
{
	public:
	//Reduction(){}
    Reduction(const char *definition_string)
	{
		//sizeof(map) 0000000
		memset(map_, 0, sizeof(map_));
		memset(map8_, 0, sizeof(map8_));
		const vector<string> tokens (tokenize(definition_string, " "));
		size_ = tokens.size();
        for(unsigned i=0;i<size_;++i)
        	for(unsigned j=0;j<tokens[i].length();++j) 
            {
        		const char ch = tokens[i][j];
				map_[(long)ValueFeature<_val>::from_char_(ch)] = i;
				map8_[(long) ValueFeature<_val>::from_char_(ch)] =  i;
            }
	}
	unsigned size() const
	{ 
        return size_;
    }
	// return an index  of seq character in map set 
	unsigned operator()(_val a) const
	{
		return map_[(long)a];
	}

	const char* map8() const
	{ 
        return map8_;
    }

	static const Reduction reduction;

private:

	unsigned map_[256];
	char map8_[256];
	unsigned size_;

};


//template<>  const Reduction<Nucleotide> Reduction<Nucleotide>::reduction ("A C G T");

//static Reduction<Nucleotide> reductions;

template<>  const Reduction<Nucleotide> Reduction<Nucleotide>::reduction ("A C G T");


#endif // __REDUCTION_H__