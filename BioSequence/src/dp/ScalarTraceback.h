#ifndef __SCALARTRACEBACK_H__
#define __SCALARTRACEBACK_H__

#include <vector>
#include "BufferTools.h"
#include "../paras/Match.h"
#include "../tool/DoubleBuffer.h"
#include "../paras/NuclMatrix.h"
#include "../paras/ScoreMatrix.h"

using std::vector;

template<typename _score>
class ScalarTracebackMatrix
{
    public:

	ScalarTracebackMatrix(const ExtendingBuffer<_score> &data, int band):
		data_ (data),
		band_ (band)
	{
		
	}
	int operator()(int col, int row) const
	{
		return data_.column(col + 1)[row - (data_.center(col + 1) - band_)];
	}
	bool in_band(int col, int row) const
	{ 
		
        return row >= data_.center(col+1)-band_ && row <= data_.center(col+1)+band_ && row >= 0 && col >= 0; 
    }
	void print(int col, int row) const
	{
		for(unsigned j=0;j<=row;++j) {
			for(unsigned i=0;i<=col;++i)
				printf("%4i", in_band(i, j) ? this->operator()(i, j) : 0);
			printf("\n");
		}
	}
private:
	const ExtendingBuffer<_score> &data_;
	const int band_;
};


template <typename _score>
bool haveVgap(const ScalarTracebackMatrix<_score> &dp,
               int i,
               int j,
               int gap_open,
               int gap_extend,
               int &l)
{
	//dp.print(i,j);
	int score = dp(i, j);
	//cout << "HaveVgap score" <<score<<endl;
	l = 1;
	--j;
	while(dp.in_band(i, j)) 
    {
		if(score == dp(i, j) - gap_open - (l-1)*gap_extend)
			return true;
		--j;
		++l;
	}
	return false;
}


template <typename _score>
bool haveHgap(const ScalarTracebackMatrix<_score> &dp,
               int i,
               int j,
               int gap_open,
               int gap_extend,
               int &l)
{
	//dp.print(i,j);
    int score = dp(i, j);
	//cout << "HaveHgap score" <<score<<endl;
	l = 1;
	--i;
	while(dp.in_band(i, j)) 
    {
		if(score == dp(i, j) - gap_open - (l-1)*gap_extend)
			return true;
		--i;
		++l;
	}
	return false;
}


template <typename _val, typename _dir, typename _score>
LocalMatch<_val> Traceback(const _val *query,
                           const _val *subject,
                            const ExtendingBuffer<_score> &scores,
                            int band,
                            int gap_open,
                            int gap_extend,
                            int i,
                            int j,
                            int score,
                            vector<char> &transcript_buf)
{
    if(i == -1)
    {
	    return LocalMatch<_val> (0);
       
    }
//	    return LocalMatch<_val> (0);

	ScalarTracebackMatrix<_score> dp (scores, band);
	//cout << "---------------" << endl;
	dp.print(i, j);
	//cout << "---------------" << endl;
	
	LocalMatch<_val> l;
	l.query_len_ = j + 1;
	l.subject_len_ = i + 1;
	l.query_begin_ = 0;
	l.subject_begin_ = 0;
	l.score_ = score;

	EditTranscript transcript (transcript_buf);
	//cout << "Traceback" << endl;
	int gap_len;
	NucleotideMatrix nm;
    while (i > 0 || j > 0)
    {
		const _val lq = GetDir(query, j, _dir()), ls = mask_critical(GetDir(subject, i, _dir()));
        // const int match_score = nm.LetterScore<_val>(lq, ls);
        const int match_score = ScoreMatrix::get().letter_score(lq, ls);
    	//printf("i=%i j=%i score=%i subject=%c query=%c\n",i,j,dp(i, j),Value_Trait<_val>::ALPHABET[ls],Value_Trait<_val>::ALPHABET[lq]);

		if(dp(i, j) == match_score + dp(i-1, j-1)) 
        {
			//cout << "op_match" << endl;

			if(lq == ls)
				++l.identities_;
			else
				++l.mismatches_;
			--i;
			--j;
			++l.len_;
			transcript_buf.push_back(op_match);
		}
        else if (haveHgap(dp, i, j, gap_open, gap_extend, gap_len)) 
        {
			//cout << "op_deletion" << endl;
			++l.gap_openings_;
			l.len_ += gap_len;
			i -= gap_len;
			transcript_buf.insert(transcript_buf.end(), gap_len ,op_deletion);
		} 
        else if (haveVgap(dp, i, j, gap_open, gap_extend, gap_len)) 
        {
			//cout << "op_insertion" << endl;

			++l.gap_openings_;
			l.len_ += gap_len;
			j -= gap_len;
			transcript_buf.insert(transcript_buf.end(), gap_len ,op_insertion);
		} else 
        {
			throw std::runtime_error("Traceback error.");
		}
	}

	if(GetDir(query, 0, _dir()) == mask_critical(GetDir(subject, 0, _dir())))
		++l.identities_;
	else
		++l.mismatches_;
	++l.len_;
	transcript_buf.push_back(op_match);
	//cout << "subject_begin_ = " <<l.subject_begin_<<", query_begin_ = "<<l.query_begin_<<endl;

	//printf("len=%i\n",l.len_);
	l.transcript_right_ = transcript.setEnd(transcript_buf);
	return l;
}


template<typename _val, typename _dir, typename _score>
LocalMatch<_val> Traceback(const _val *query,
		const _val *subject,
		const Double_buffer<_score> &scores,
		int band,
		int gap_open,
		int gap_extend,
		int i,
		int j,
		int score)
    { 
        return LocalMatch<_val> (score); 
    }





#endif // __SCALARTRACEBACK_H__