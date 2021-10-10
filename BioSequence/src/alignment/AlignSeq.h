#ifndef __ALIGNSEQ_H__
#define __ALIGNSEQ_H__


#include "../dp/FloatingSmithWaterman.h"
#include "../sequence/Queries.h"
#include "../sequence/Reference.h"
#include "../dp/ScalarDPMatrix.h"
#include "../paras/ScoreMatrix.h"

template<typename _val, typename _locr, typename _locl>
void alignSequence(vector<Segment<_val> > &matches,
                            Statistics &stat,
                            vector<LocalMatch<_val> > &local,
                            unsigned *padding,
                            size_t db_letters,
                            unsigned dna_len,
                            typename TracePtBuffer<_locr, _locl>::Vector::iterator &begin,
                            typename TracePtBuffer<_locr, _locl>::Vector::iterator &end,
                            vector<char> &transcript_buf)
{
	//cout << "AlignSequence" << endl;
	std::sort(begin, end, Hit<_locr, _locl>::cmpNormalizedSubject);
	const unsigned q_num (begin->query_);
	//cout << "q_num = " <<q_num<<endl;

	const Sequence<const _val> query (QuerySeqs<_val>::get()[q_num]);
	const unsigned frame = q_num % 1;
	const unsigned query_len = query.length();
	padding[frame] = VATOptions::readPadding<_val>(query_len);
	//padding[frame] = 2;
	//cout << "padding[frame] = " << padding[frame] << endl;
	cout << "query sequence = " << query << endl;

	const SequenceSet<_val> *ref = ReferenceSeq<_val>::data_;
	for(typename TracePtBuffer<_locr,_locl>::Vector::iterator i = begin; i != end; ++i) 
    {
		//cout << "i->seed_offset_ = " <<i->seed_offset_<< endl;
		if (i != begin && (i->global_diagonal() - (i - 1)->global_diagonal()) <= padding[frame])
		{
			
			stat.inc(Statistics::DUPLICATES);
			continue;
		}
		//cout << "VATOptions::gapped_xdrop = " <<VATOptions::gapped_xdrop<<", padding[frame] = "<<padding[frame]<<"ScoreMatrix::get().getRawscore(VATOptions::gapped_xdrop)"<<ScoreMatrix::get().getRawscore(VATOptions::gapped_xdrop)<<endl;
		local.push_back(LocalMatch<_val>(i->seed_offset_, ref->data(i->subject_)));
		// cout << "1111111111111" << endl;
		floatingSW(&query[i->seed_offset_],
				   local.back(),
				   padding[frame],
				   ScoreMatrix::get().getRawscore(VATOptions::gapped_xdrop),
				   VATOptions::gap_open + VATOptions::gap_extend,
				   VATOptions::gap_extend,
				   transcript_buf,
				   TraceBack());

		const int score = local.back().score_;
		//cout << "FloatingSW score = " <<score<<endl;

		std::pair<size_t,size_t> l = ReferenceSeq<_val>::data_->localPosition(i->subject_);
		matches.push_back(Segment<_val> (score, frame, &local.back(), l.first));
		anchoredTransform(local.back(), l.second, i->seed_offset_);
		stat.inc(Statistics::ALIGNED_QLEN, local.back().query_len_);

		//local.back().print(query, ReferenceSeq<_val>::get()[l.first], transcript_buf);

		toSourceSpace(local.back(), frame, dna_len);
		stat.inc(Statistics::SCORE_TOTAL, local.back().score_);
		stat.inc(Statistics::OUT_HITS);
	}

}
#endif // __ALIGNSEQ_H__