#ifndef __REFERENCE_H__
#define __REFERENCE_H__

#include <memory>
#include <string>
#include <numeric>
#include "../tool/BinaryFile.h"
#include "../index/SortedList.h"
#include "../paras/statistics.h"
//#include "../join/SeedCluster.h"
#include "../tool/HashTable.h"
#include "../tool/HashFunction.h"
#include "../paras/PackedLocation.h"
#include "SequenceSet.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "frequency_mask.h"
#include "StringSet.h"
//#include "../workflow/VATObject.h"


using std::auto_ptr;
using boost::ptr_vector;
using namespace std;

class ReferenceHeader
{
	public:
	ReferenceHeader():
		unique_id (0x24af8a415ee186dllu),
		build (ConstParameter::build_version),
		long_addressing (false),
		sequences (0),
		letters (0)
	{

	}
	uint64_t unique_id;
	uint32_t build;
	bool long_addressing;
	unsigned n_blocks;
	size_t sequences, letters;
	double block_size;
} refheader;

//static ReferenceHeader refheader;
class DatabaseFormatException : public exception
{
	virtual const char* what() const throw()
	{ 
		return "Database file is not a VAT database."; 
	}
};


//input database file 
template<typename _val>
class DatabaseController : public InputStreamController
{
	public:
	DatabaseController():
		InputStreamController (VATOptions::database)
	{
		if(this->read(&refheader, 1) != 1)
			throw DatabaseFormatException ();
		if(refheader.unique_id != ReferenceHeader ().unique_id)
			throw DatabaseFormatException ();
		if(refheader.build > ConstParameter::build_version || refheader.build < ConstParameter::build_compatibility)
			throw InvalidDBVersionException();

	}
	void rewind()
	{ 
		this->seek(sizeof(ReferenceHeader)); 
	}
};

template<typename _val>
class ReferenceSeq
{
	public:
	static const MaskSequenceSet<_val>& get()
	{
		 return *(const MaskSequenceSet<_val>*)data_; 
	}
	static MaskSequenceSet<_val>& get_nc()
	{ 
		return *(MaskSequenceSet<_val>*)data_;
	}
	static SequenceSet<_val> *data_;
};

template<typename _val> SequenceSet<_val>* ReferenceSeq<_val>::data_ = 0;

class ReferenceIds
{
	public:
	static const BaseStringSet<char,0>& get()
	{ 
		return *data_;
	}

	static BaseStringSet<char,0> *data_;
};

//StringSet<char,0>* ReferenceIds::data_ = 0;
static RadixCluster ref_hst;
unsigned static current_ref_block;
BaseStringSet<char,0>* ReferenceIds::data_ = 0;

size_t maxIdLen(const BaseStringSet<char, 0> &ids)
{
	size_t max (0);
	for(size_t i=0;i<ids.getLength(); ++i)
		max = std::max(max, find_first_of(ids[i].cStr(), ConstParameter::id_delimiters));
	return max;
}

class ReferenceMap
{
	public:
	ReferenceMap():
		next_ (0)
	{
		//cout << "" << endl;

	}
	void init(unsigned ref_count)
	{

		const unsigned block = current_ref_block;
		if(data_.size() < block+1) 
		{

			data_.resize(block + 1);
			data_[block].insert(data_[block].end(), ref_count, std::numeric_limits<unsigned>::max());

		}


	}
	template<typename _val>
	uint32_t get(unsigned block, unsigned i)
	{
		uint32_t n = data_[block][i];
		if(n != std::numeric_limits<unsigned>::max())
			return n;
		else {
			tthread::lock_guard<tthread::mutex> lock (mtx_);
			n = data_[block][i];
			if(n != std::numeric_limits<uint32_t>::max())
				return n;
			n = next_++;
			data_[block][i] = n;
			len_.push_back(ReferenceSeq<_val>::get().length(i));
			name_.push_back(GetStr(ReferenceIds::get()[i].cStr(), ConstParameter::id_delimiters));
			return n;
		}
	}
private:
	tthread::mutex mtx_;
	vector<vector<uint32_t> > data_;
	vector<uint32_t> len_;
	ptr_vector<string> name_;
	uint32_t next_;
	friend struct VATOutput;
} ref_map;
//static ReferenceMap ref_map;

#endif // __REFERENCE_H__