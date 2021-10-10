#include "Reference.h"

// ReferenceHeader refheader;
// template<typename _val> SequenceSet<_val>* ReferenceSeq<_val>::data_ = 0;
BaseStringSet<char,0>* ReferenceIds::data_ = 0;
//static ReferenceHeader refheader;
// RadixCluster ref_hst;
// unsigned current_ref_block;

size_t maxidLen(const BaseStringSet<char,0> &ids)
{
	size_t max (0);
	for(size_t i=0;i<ids.getLength(); ++i)
		max = std::max(max, find_first_of(ids[i].c_str(), ConstParameter::id_delimiters));
	return max;
}

// ReferenceMap ref_map;



