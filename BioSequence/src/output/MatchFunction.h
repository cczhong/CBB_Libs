#ifndef __MATCHFUNCTION_H__
#define __MATCHFUNCTION_H__

#include "../paras/Match.h"

int blastFrame(unsigned frame)
{ 
	return frame <= 2 ? (int)frame+1 : 2-(int)frame; 
}

template<typename _val>
void anchoredTransform(LocalMatch<_val> &l, unsigned subject_pos, unsigned query_pos)
{
	l.query_begin_ = query_pos - l.query_begin_;
	l.subject_begin_ = subject_pos - l.subject_begin_;
}

template<typename _val>
void toSourceSpace(LocalMatch<_val> &l, unsigned frame, unsigned source_len)
{
	if(frame == 1) {
		l.query_begin_ = source_len - l.query_begin_ - 1;
		l.query_len_ *= -1;
	}
}

template<>
void toSourceSpace<Protein>(LocalMatch<Protein> &l, unsigned frame, unsigned dna_len)
{
	// if(!query_translated())
	// 	return;
	int query_begin_dna; //, query_end_dna;
	signed f = frame <= 2 ? frame+1 : 2-frame;
	if (f > 0) 
	{
		query_begin_dna = (f-1) + 3 * l.query_begin_;
		//query_end_dna = (f-1) + 3 * (l.query_begin_+l.query_len_-1) + 3;
	} else 
	{
		query_begin_dna = dna_len + f - 3 * l.query_begin_;
		//query_end_dna = dna_len + (f + 1) - 3 * (l.query_begin_+l.query_len_-1) - 2;
		l.query_len_ *= -1;
	}
	l.query_begin_ = query_begin_dna;
	l.query_len_*= 3;
}

template<typename _val>
unsigned queryTranslatedBegin(unsigned query_begin, unsigned frame, unsigned dna_len, bool query_translated)
{
	if(frame == 0)
		return query_begin;
	else
		return dna_len-query_begin-1;
}

template<>
unsigned queryTranslatedBegin<Protein>(unsigned query_begin, unsigned frame, unsigned dna_len, bool query_translated)
{
	if(!query_translated)
		return query_begin;
	int f = frame <= 2 ? frame+1 : 2-frame;
	if (f > 0)
		return (query_begin - (f-1))/3;
	else
		return (dna_len + f - query_begin)/3;
}

#endif // __MATCHFUNCTION_H__