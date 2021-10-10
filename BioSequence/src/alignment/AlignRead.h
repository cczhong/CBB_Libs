#ifndef __ALIGNREAD_H__
#define __ALIGNREAD_H__


#include <vector>
#include <assert.h>
#include "../tool/AsyncBuffer.h"
#include "../paras/Match.h"
#include "../paras/statistics.h"
#include "AlignSeq.h"
#include "../tool/TextBuffer.h"
#include "../output/VATOutBuffer.h"
#include "ConnectSegment.h"
#include "../alignment/XdropUngapped.h"

template<typename _val, typename _locr, typename _locl>
void alignReads(OutputBuffers<_val> &buffer,
                    Statistics &stat,
                    typename TracePtBuffer<_locr, _locl>::Vector::iterator &begin,
                    typename TracePtBuffer<_locr, _locl>::Vector::iterator &end)
{
    static thread_specific_ptr<vector<LocalMatch<_val> > > local_ptr;
	static thread_specific_ptr<vector<Segment<_val> > > matches_ptr;
	static thread_specific_ptr<vector<char> > transcript_ptr;
	//cout << "AlignReads...." << endl;

	Tls<vector<Segment<_val> > > matches (matches_ptr);
	Tls<vector<LocalMatch<_val> > > local (local_ptr);
	Tls<vector<char> > transcript_buf (transcript_ptr);
	local->clear();
	matches->clear();
	transcript_buf->clear();

	assert(end > begin);
	const size_t hit_count = end - begin;
	cout << "hit_count = " <<hit_count<<endl;

	local->reserve(hit_count);
	const unsigned contexts = 1;
	const unsigned query = begin->query_;
	const size_t query_len (QuerySeqs<_val>::data_->length(query*contexts));
	cout << "query_len = " <<query_len<<endl;

	//const size_t source_query_len = query_translated() ? QuerySeqs<_val>::data_->reverse_translated_len(query*contexts) : query_len;
	const size_t source_query_len =  query_len;
	const size_t db_letters = refheader.letters;
	unsigned padding[6];

	typedef Map<typename vector<Hit<_locr,_locl> >::iterator,typename Hit<_locr,_locl>::template QueryId<1> > Map_t;
	Map_t hits (begin, end);
	typename Map_t::Iterator i = hits.begin();
	while(i.valid()) 
    {
		//cout << "AlignReads" << endl;
		//AlignSeq<_val, _locr, _locl> as;
		alignSequence<_val, _locr, _locl>(*matches, stat, *local, padding, db_letters, source_query_len, i.begin(), i.end(), *transcript_buf);
		++i;
	}

	if(matches->size() == 0)
		return;

	connectSegment(*matches);

	std::sort(matches->begin(), matches->end());
	unsigned n_hsp = 0, n_target_seq = 0;
	typename vector<Segment<_val> >::iterator it = matches->begin();
	const int min_raw_score = 0;
	const int top_score = matches->operator[](0).score_;

	while(it < matches->end()) 
    {
		const bool same_subject = it != matches->begin() && (it-1)->subject_id_ == it->subject_id_;
		if(!same_subject && it->score_ < min_raw_score)
			break;
		if(!same_subject && !VATOptions::outputRange(n_target_seq, it->score_, top_score))
			break;
		if(same_subject && (it-1)->score_ == it->score_) {
			++it;
			continue;
		}
		if(static_cast<double>(it->traceback_->identities_)*100/it->traceback_->len_ < VATOptions::min_id) {
			++it;
			continue;
		}
		if(same_subject && VATOptions::single_domain) {
			++it;
			continue;
		}

		// if(n_hsp == 0)
		// 	buffer.write_query_record(query);
		buffer.print_match(*it, source_query_len, QuerySeqs<_val>::get()[query*contexts + it->frame_], query, *transcript_buf);

		++n_hsp;
		if(!same_subject)
			++n_target_seq;
		if(VATOptions::alignment_traceback && it->traceback_->gap_openings_ > 0)
			stat.inc(Statistics::GAPPED);
		++it;
	}

	// if(n_hsp > 0)
	// 	buffer.finish_query_record();

	stat.inc(Statistics::OUT_MATCHES, matches->size());
	if(refheader.n_blocks == 1) 
    {
		stat.inc(Statistics::MATCHES, n_hsp);
		if(n_hsp > 0)
			stat.inc(Statistics::ALIGNED);
	}

}
#endif // __ALIGNREAD_H__