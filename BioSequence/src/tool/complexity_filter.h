
#ifndef COMPLEXITY_FILTER_H_
#define COMPLEXITY_FILTER_H_

// #include "../algo/blast/core/blast_seg.h"
// #include "../algo/blast/core/blast_filter.h"
// #include "../commons/value.h"
// #include "StringSet.h"
// #include "Sequence.h"
// #include "thread.h"

// template<class _val>
// struct Complexity_filter
// {
// 	unsigned filter(vector<_val> &seq) const
// 	{ 
// 		return 0;
// 	}
// 	static const Complexity_filter& get()
// 	{ 
// 		return instance;
// 	}
// 	void run(StringSet<_val> &seqs) const
// 	{

// 	}
// 	static const Complexity_filter instance;
// };

// template<>
// struct Complexity_filter<AminoAcid>
// {

// 	Complexity_filter()
// 	{ blast_seg_ = SegParametersNewAa(); }

// 	~Complexity_filter()
// 	{ SegParametersFree(blast_seg_); }

// 	unsigned filter(sequence<AminoAcid> seq) const
// 	{
// 		BlastSeqLoc *seg_locs;
// 		SeqBufferSeg ((uint8_t*) seq.data(), seq.length(), 0, blast_seg_, &seg_locs);
// 		unsigned nMasked = 0;

// 		if(seg_locs) {
// 			BlastSeqLoc *l = seg_locs;
// 			do {
// 				for(signed i=l->ssr->left;i<=l->ssr->right;i++) {
// 					nMasked++;
// 					seq[i] = Value_Trait<AminoAcid>::MASK_CHAR;
// 				}
// 			} while((l=l->next) != 0);
// 			BlastSeqLocFree(seg_locs);
// 		}
// 		return nMasked;
// 	}

// 	static const Complexity_filter& get()
// 	{ return instance; }

// 	void run(StringSet<AminoAcid> &seqs) const
// 	{
// 		Filter_context context (seqs, *this);
// 		ThreadPool(context, seqs.get_length(), VATOptions::threads());
// 	}

// private:

// 	struct Filter_context
// 	{
// 		Filter_context(StringSet<AminoAcid> &seqs, const Complexity_filter &filter):
// 			seqs (seqs),
// 			filter (filter)
// 		{ }
// 		void operator()(unsigned thread_id, unsigned i)
// 		{
// 			filter.filter(seqs[i]);
// 		}
// 		StringSet<AminoAcid> &seqs;
// 		const Complexity_filter &filter;
// 	};

// 	SegParameters *blast_seg_;

// 	static const Complexity_filter instance;

// };

// const Complexity_filter<AminoAcid> Complexity_filter<AminoAcid>::instance;
// #ifdef NDEBUG
// template<> const Complexity_filter<Nucleotide> Complexity_filter<Nucleotide>::instance;
// #else
// template<typename _val> const Complexity_filter<_val> Complexity_filter<_val>::instance;
// #endif


#endif /* COMPLEXITY_FILTER_H_ */
