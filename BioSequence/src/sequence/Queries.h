#ifndef QUERIES_H_
#define QUERIES_H_

//#include "../commons/translate.h"
//#include "../tool/complexity_filter.h"
#include "../index/SortedList.h"
#include "../paras/statistics.h"
#include "SequenceSet.h"

static auto_ptr<RadixCluster> query_hst;
unsigned extern current_query_chunk;

class QuerySourceSeqs
{
	public:
	static const SequenceSet<Nucleotide>& get()
	{ return *data_; }
	static SequenceSet<Nucleotide> *data_;
};

SequenceSet<Nucleotide>* QuerySourceSeqs::data_ = 0;

template<typename _val>
class QuerySeqs
{
	public:
	static const SequenceSet<_val>& get()
	{ 
		return *data_;
	}
	static SequenceSet<_val> *data_;
};
template<typename _val>  SequenceSet<_val>* QuerySeqs<_val>::data_ = 0;

class QueryIds
{
	public:
	static const BaseStringSet<char,0>& get()
	{ 
		return *data_; 
	}
	static BaseStringSet<char,0> *data_;
};

BaseStringSet<char,0>* QueryIds::data_ = 0;

#endif /* QUERIES_H_ */

