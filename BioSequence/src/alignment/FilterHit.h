#ifndef __FILTERHIT_H__
#define __FILTERHIT_H__
#include "../sequence/Sequence.h"
#include "../paras/statistics.h"
#include "../sequence/Reference.h"
#include "../index/SortedList.h"
#include "../sequence/Queries.h"
#include "../paras/Match.h" 
#include "VATSearch.h"
#include "../dp/SmithWaterman.h"
/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
   template<typename T, typename _locr, typename _locq, typename _locl>
    class HitFilter
    {
        public:

        HitFilter(Statistics &stats,
			   _locq q_pos,
			    typename TracePtBuffer<_locr,_locl>::Iterator &out):
		q_num_ (std::numeric_limits<unsigned>::max()),
		seed_offset_ (std::numeric_limits<unsigned>::max()),
		stats_ (stats),
		q_pos_ (q_pos),
		out_ (out),
		subjects_ (&s2)
        { 
            subjects_->clear(); 
        }



        void push(_locr subject,int score)
        {

            if(score >= 0)
            {
                insertHit(subject);
            }
            else
            {
                subjects_->push_back(ReferenceSeq<T>::data_->fixedWindowInfix(subject+ConstParameter::seed_anchor));
            }
        }

 
        void insertHit(_locr subject)
        {
            if(q_num_ == std::numeric_limits<unsigned>::max()) 
            {
                std::pair<size_t,size_t> l (QuerySeqs<T>::data_->localPosition(q_pos_));
                q_num_ = l.first;
                seed_offset_ = l.second;
            }
            out_.push(Hit<_locr, _locl>(q_num_, subject, seed_offset_));
            stats_.inc(Statistics::TENTATIVE_MATCHES3);
        }

        void Finish()
        {
            NucleotideMatrix nm;
            if (subjects_->size() == 0)
                return;
            unsigned left = 0;
            Sequence<const T> query(QuerySeqs<T>::data_->windowInfix(q_pos_ + ConstParameter::seed_anchor, left));
            smithWaterman(query,
                           *subjects_,
                           0,
                           left,
                           nm.getGapOpen()+nm.getGapExtend(),
                           nm.getGapExtend(),
                           0,
                           *this,
                           uint8_t(),
                           stats_); 
            return;
        }

        void operator()(int i, const Sequence<const T> &seq, int score)
        { 
            insertHit(ReferenceSeq<T>::data_->position(seq.data()+ 0 - ConstParameter::seed_anchor)); 
            stats_.inc(Statistics::GAPPED_HITS);
        }

        private:
        unsigned q_num_, seed_offset_;
        Statistics  &stats_;
        _locq q_pos_;//query position
        typename TracePtBuffer<_locr,_locl>::Iterator &out_;
        vector<Sequence<const T> > s2;
        vector<Sequence<const T> >* subjects_;
    };



/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
    class HitsFilter
    {
        public:

        HitsFilter(Statistics &stats,
			   uint64_t q_pos,
			   typename TracePtBuffer<uint64_t,uint32_t>::Iterator &out):
		q_num_ (std::numeric_limits<unsigned>::max()),
		seed_offset_ (std::numeric_limits<unsigned>::max()),
		stats_ (stats),
		q_pos_ (q_pos),
		out_ (out),
		subjects_ (&s2)
        { 
            subjects_->clear(); 
        }



        void push(uint64_t subject,int score)
        {

            if(score >= 0)
            {  
                insertHit(subject);
            }else
            {
                subjects_->push_back(ReferenceSeq<Protein>::data_->fixedWindowInfix(subject+ConstParameter::seed_anchor));
            }
        }

 
        void insertHit(uint64_t subject)
        {
            if(q_num_ == std::numeric_limits<unsigned>::max()) 
            {
                std::pair<size_t,size_t> l (QuerySeqs<Protein>::data_->localPosition(q_pos_));
                q_num_ = l.first;
                seed_offset_ = l.second;
            }
            out_.push(Hit<uint64_t, uint32_t>(q_num_, subject, seed_offset_));
            stats_.inc(Statistics::TENTATIVE_MATCHES3);
        }

        void finish()
        {
            NucleotideMatrix nm;
            if (subjects_->size() == 0)
                return;
            unsigned left = 0;
            Sequence<const Protein> query(QuerySeqs<Protein>::data_->windowInfix(q_pos_ + ConstParameter::seed_anchor, left));
            smithWaterman(query,
                           *subjects_,
                           0,
                           left,
                           nm.getGapOpen()+nm.getGapExtend(),
                           nm.getGapExtend(),
                           0,
                           *this,
                           uint8_t(),
                           stats_); 
            return;
        }

        void operator()(int i, const Sequence<const Protein> &seq, int score)
        { 
            insertHit(ReferenceSeq<Protein>::data_->position(seq.data()+ 0 - ConstParameter::seed_anchor)); 
            stats_.inc(Statistics::GAPPED_HITS);
        }

        private:
        unsigned q_num_, seed_offset_;
        Statistics  &stats_;
        uint64_t q_pos_;//query position
        typename TracePtBuffer<uint64_t,uint32_t>::Iterator &out_;
        vector<Sequence<const Protein> > s2;
        vector<Sequence<const Protein> >* subjects_;
    };

#endif // __FILTERHIT_H__