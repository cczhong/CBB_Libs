#ifndef __JOINRANGE_H__
#define __JOINRANGE_H__

#include "../index/SortedList.h"
#include "../paras/statistics.h"
#include "VATSearch.h"
#include "Align.h"
#include "FilterHit.h"


template <typename _val, typename _locr, typename _locq, typename _locl>
void alignRange(_locq q_pos,
				 const typename SortedLists<_locr>::Type::const_iterator &s,
				 Statistics &stats,
				 typename TracePtBuffer<_locr, _locl>::Iterator &out,
				 unsigned sid)
{
	unsigned i = 0, n=0;
	const _val* query = QuerySeqs<_val>::data_->data(q_pos);
	HitFilter<_val, _locr, _locq, _locl> hf(stats, q_pos, out);

	if(s.n <= 20) 
    {
		stats.inc(Statistics::SEED_HITS, s.n);
		while(i < s.n) 
        {
			hitAlign<_val,_locr,_locq,_locl>(q_pos, query, s[i], stats, sid, hf);
			++i;
		}
	} else 
    {
		while(i < s.n && s[i] != 0) 
        {
			assert(position_filter(s[i], filter_treshold(s.n), s.key()));
			hitAlign<_val,_locr,_locq,_locl>(q_pos, query, s[i], stats, sid, hf);
			stats.inc(Statistics::SEED_HITS);
			++i;
			++n;
		}
	}

	hf.Finish();
}

template<typename _val, typename _locr, typename _locq, typename _locl>
void alignRange(const typename SortedLists<_locq>::Type::const_iterator &q,
				 const typename SortedLists<_locr>::Type::const_iterator &s,
				 Statistics &stats,
				 typename TracePtBuffer<_locr,_locl>::Iterator &out,
				 const unsigned sid)
{
	for(unsigned i=0;i<q.n; ++i)
    {
		alignRange<_val,_locr,_locq,_locl>(_locq(q[i]), s, stats, out, sid);
    }
}


/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */

template<typename _val, typename _locr, typename _locq, typename _locl>
void alignPartition(unsigned hp,
		Statistics &stats,
		unsigned sid,
		typename SortedLists<_locr>::Type::const_iterator i,
		typename SortedLists<_locq>::Type::const_iterator j,
		unsigned thread_id)
{
	typename TracePtBuffer<_locr,_locl>::Iterator* out = new typename TracePtBuffer<_locr,_locl>::Iterator (*TracePtBuffer<_locr,_locl>::instance, thread_id);
	while(!i.at_end() && !j.at_end() && !exception_state()) 
    {
		if(i.key() < j.key()) 
        {
			++i;
		} else if(j.key() < i.key()) 
        {
			++j;
		} else 
        {
			alignRange<_val,_locr,_locq,_locl>(j, i, stats, *out, sid);
			++i;
			++j;
		}
	}
	delete out;
}


class AlignPartition
{
	public:
/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
	void alignPartition(unsigned hp,
		Statistics &stats,
		unsigned sid,
		typename SortedLists<uint64_t>::Type::const_iterator i,
		typename SortedLists<uint64_t>::Type::const_iterator j,
		unsigned thread_id)
		{
			typename TracePtBuffer<uint64_t,uint32_t>::Iterator* out = new typename TracePtBuffer<uint64_t,uint32_t>::Iterator (*TracePtBuffer<uint64_t,uint32_t>::instance, thread_id);
			while(!i.at_end() && !j.at_end() && !exception_state()) 
			{
				if(i.key() < j.key()) 
				{
					++i;
				} else if(j.key() < i.key()) 
				{
					++j;
				} else 
				{

					alignSubpartition(j, i, stats, *out, sid);
					++i;
					++j;
				}
			}
			delete out;
		}
/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
	void alignSubpartition(const typename SortedLists<uint64_t>::Type::const_iterator &q,
					const typename SortedLists<uint64_t>::Type::const_iterator &s,
					Statistics &stats,
					typename TracePtBuffer<uint64_t,uint32_t>::Iterator &out,
					const unsigned sid)
	{
		for(unsigned i=0;i<q.n; ++i)
		{
			alignSubpartition(uint64_t(q[i]), s, stats, out, sid);
		}
	}
/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
	void alignSubpartition(uint64_t q_pos,
				 const typename SortedLists<uint64_t>::Type::const_iterator &s,
				 Statistics &stats,
				 typename TracePtBuffer<uint64_t, uint32_t>::Iterator &out,
				 unsigned sid)
	{
		unsigned i = 0, n=0;
		const Protein* query = QuerySeqs<Protein>::data_->data(q_pos);
		HitFilter<Protein, uint64_t, uint64_t, uint32_t> hf(stats, q_pos, out);

		if(s.n <= 20) 
		{
			stats.inc(Statistics::SEED_HITS, s.n);
			while(i < s.n) 
			{
				hitAlign(q_pos, query, s[i], stats, sid, hf);
				++i;
			}
		} else 
		{
			while(i < s.n && s[i] != 0) 
			{
				assert(position_filter(s[i], filter_treshold(s.n), s.key()));
				hitAlign(q_pos, query, s[i], stats, sid, hf);
				stats.inc(Statistics::SEED_HITS);
				++i;
				++n;
			}
		}

		hf.Finish();
	}

/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
    void hitAlign(const uint64_t q_pos,
                                                const Protein *query,
                                                uint64_t s,
                                                Statistics &stats,
                                                const unsigned sid,
												HitFilter<Protein, uint64_t, uint64_t, uint32_t> &hf)
	{
		stats.inc(Statistics::TENTATIVE_MATCHES0);
		const Protein *subject = ReferenceSeq<Protein>::data_->data(s);
		unsigned  length, delta;
		int score = xdropUngapped<Protein, uint64_t, uint64_t>(query, subject, ShapeConfigure::get().getShape(sid).length_, delta, length);
		if (score < 0)
			return;
		stats.inc(Statistics::TENTATIVE_MATCHES2);
		hf.push(s,score);

	}
};



#endif // __JOINRANGE_H__