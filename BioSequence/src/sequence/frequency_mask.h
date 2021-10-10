
#ifndef FREQUENCY_MASKING_H_
#define FREQUENCY_MASKING_H_

#include <vector>
#include <iostream>
#include <auto_ptr.h>
#include "../tool/Thread.h"

using std::auto_ptr;
using std::cout;
using std::vector;

template <typename _val>
class MaskSequenceSet : public SequenceSet<_val>
{
	public:
	MaskSequenceSet(InputStreamController &file):
			SequenceSet<_val> (file)
	{
		cout << "construction mask" << endl;
	
	}

	template<typename _loc>
	void createMask(unsigned sid, const SeedPartitionRange &range, typename SortedLists<_loc>::Type &idx)
	{
		cout << "Counting low complexity seeds" << endl;
		vector<unsigned> counts (ConstParameter::seedp);
		CountContext<_loc> count_context (idx, counts);
		ThreadPool(count_context, ConstParameter::seedp, VATOptions::threads());
		size_t n = 0;
		for(unsigned i=range.getBegin();i<range.getEnd();++i) 
		{
			n += counts[i];
			const size_t ht_size (std::max(static_cast<size_t>(static_cast<float>(counts[i]) * 1.3), static_cast<size_t>(counts[i] + 1)));
			pos_filters[sid][i] = auto_ptr<filter_table> (new filter_table(ht_size));
		}
		cout << "Hit cap = " << VATOptions::hit_cap << std::endl;
		cout << "Low complexity seeds = " << n << std::endl;

		CreateContext<_loc> build_context(idx, sid, counts, *this);
		ThreadPool(build_context, ConstParameter::seedp, VATOptions::threads());
		cout << "Masked positions = " << std::accumulate(counts.begin(), counts.end(), 0) << std::endl;
	}

	bool getMasking(const _val *pos, unsigned sid) const
	{
		uint64_t seed;
		ShapeConfigure::get().getShape(sid).setSeed(seed, pos);
		const filter_table::entry *e;
		if((e = pos_filters[sid][seedPartition(seed)]->operator [](seedPartitionOffset(seed))) != 0) 
		{
			const size_t offset (pos - this->data(0));
			return !position_filter(offset, e->value, seedPartitionOffset(seed));
		} else
			return false;
	}

	virtual ~MaskSequenceSet()
	{ 

	}

private:

	template<typename _loc>
	class CountContext
	{
		public:
		CountContext(const typename SortedLists<_loc>::Type &idx, vector<unsigned> &counts):
			idx (idx),
			counts (counts)
		{ 

		}
		void operator()(unsigned thread_id, unsigned seedp) const
		{
			unsigned n = 0;
			typename SortedLists<_loc>::Type::const_iterator i = idx.getPartitionConstBegin(seedp);
			while(!i.at_end()) {
				if(i.n > VATOptions::hit_cap)
					++n;
				++i;
			}
			counts[seedp] = n;
		}
		const typename SortedLists<_loc>::Type &idx;
		vector<unsigned> &counts;
	};

	template<typename _loc>
	class CreateContext
	{
		public:
		CreateContext(const typename SortedLists<_loc>::Type &idx, unsigned sid, vector<unsigned> &counts, MaskSequenceSet<_val> &seqs):
			idx (idx),
			sid (sid),
			counts (counts),
			seqs (seqs)
		{ 
			
		}
		void operator()(unsigned thread_id, unsigned seedp)
		{
			unsigned n = 0;
			typename SortedLists<_loc>::Type::iterator i = idx.getPartitionBegin(seedp);
			while(!i.at_end()) {
				if(i.n > VATOptions::hit_cap)
					n += seqs.maskSeedPos<_loc>(i, sid, seedp);
				++i;
			}
			counts[seedp] = n;
		}
		const typename SortedLists<_loc>::Type &idx;
		const unsigned sid;
		vector<unsigned> &counts;
		MaskSequenceSet<_val> &seqs;
	};

	template<typename _loc>
	unsigned maskSeedPos(typename SortedLists<_loc>::Type::iterator &i, unsigned sid, unsigned p)
	{
		const unsigned treshold (filter_treshold(i.n));
		unsigned count (0), k (0);
		for(unsigned j=0;j<i.n;++j)
			if(!position_filter(i[j], treshold, i.key())) 
			{
				maskSeedPos<_loc>(i[j]);
				++count;
			} else
				*(i.get(k++)) = *(i.get(j));
		i.get(k)->value = 0;
		pos_filters[sid][p]->insert(i.key(), treshold);
		return count;
	}

	template<typename _loc>
	void maskSeedPos(_loc pos)
	{
		_val *x = this->data(pos);
		*x = set_critical(*x);
	}

private:

	typedef hash_table<uint32_t, uint8_t, ValueCompare<uint8_t, 0>, murmur_hash> filter_table;
	auto_ptr<filter_table> pos_filters[ConstParameter::max_shapes][ConstParameter::seedp];

};

#endif /* FREQUENCY_MASKING_H_ */
