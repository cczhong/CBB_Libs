#ifndef __CREATESEARCHCONTEXT_H__
#define __CREATESEARCHCONTEXT_H__

#include "../sequence/Reference.h"
// #include "../out/VATWriter.h"
#include "../alignment/JoinRange.h"
//#include "../join/RadixCluster.h"
#include "../index/SortedList.h"
#include "../paras/statistics.h"
/**
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
template<typename T, typename _locr, typename _locq, typename _locl>
class CreateSearchContext
{
    public:
	CreateSearchContext(unsigned sid, const typename SortedLists<_locr>::Type &ref_idx, const typename SortedLists<_locq>::Type &query_idx):
		sid (sid),
		ref_idx (ref_idx),
		query_idx (query_idx)
	{ 

    }
	void operator()(unsigned thread_id, unsigned seedp) const
	{
		Statistics stat;
		//JoinRange<T, _locr, _locq, _locl> jr;
		alignPartition<T, _locr, _locq, _locl>(seedp,
						  stat,
						  sid,
						  ref_idx.getPartitionConstBegin(seedp),
						  query_idx.getPartitionConstBegin(seedp),
						  thread_id);
		statistics += stat;
	}

	const unsigned sid;
	const typename SortedLists<_locr>::Type &ref_idx;
	const typename SortedLists<_locq>::Type &query_idx;
};


class SearchContext
{
    public:
	SearchContext(unsigned sid, const typename SortedLists<uint64_t>::Type &ref_idx, const typename SortedLists<uint64_t>::Type &query_idx):
		sid (sid),
		ref_idx (ref_idx),
		query_idx (query_idx)
	{ 

    }
	void operator()(unsigned thread_id, unsigned seedp) const
	{
		Statistics stat;
		alignPartition<Protein, uint64_t, uint64_t, uint32_t>(seedp,
						  stat,
						  sid,
						  ref_idx.getPartitionConstBegin(seedp),
						  query_idx.getPartitionConstBegin(seedp),
						  thread_id);
		statistics += stat;
	}

	const unsigned sid;
	const typename SortedLists<uint64_t>::Type &ref_idx;
	const typename SortedLists<uint64_t>::Type &query_idx;
};


#endif // __CREATESEARCHCONTEXT_H__