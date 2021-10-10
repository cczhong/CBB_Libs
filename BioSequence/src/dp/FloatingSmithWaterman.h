#ifndef __FLOATINGSMITHWATERMAN_H__
#define __FLOATINGSMITHWATERMAN_H__

#include "../paras/Match.h"
#include "ScalarDPMatrix.h"
#include "../tool/Direction.h"
#include "ScalarTraceback.h"
#include "../paras/ScoreMatrix.h"


//FloatingSmithWatermantoRight

template <typename _val, typename _dir, typename _score, typename _traceback>
LocalMatch<_val> floatingSWDirection(const _val *query, const _val *subject, 
        int band, _score xdrop, _score gap_open, _score gap_extend, vector<char> &transcript_buf)
{
    using std::max;
	_score max_score = 0, column_max = 0;
	int j = 0, i_max = -1, j_best = -1, i_best = -1;
	ScalarDPMatrix<_score,_traceback> mtx (band);
	//int x1 = 0, y1 = 0;
	//int matrix[][];
	const _val *x = query, *y = subject;
	//cout << "x = " <<Value_Trait<_val>::ALPHABET[*x]<<", y = "<<Value_Trait<_val>::ALPHABET[*y]<<endl;
	NucleotideMatrix nm;
	while (*y != BaseStringSet<_val>::pad_char && max_score - column_max < xdrop)
	{
		typename ScalarDPMatrix<_score, _traceback>::Column_iterator it = mtx.column(j, i_max);
		if(GetDir(x, it.row(), _dir()) == BaseStringSet<_val>::pad_char)
		{
			break;
		}
		_score vgap = ScalarDPMatrix<_score,_traceback>::NEG_MIN;
		//cout << "x = " << Value_Trait<_val>::ALPHABET[get_dir(x, i_max+1, _dir())] << ", y = " << Value_Trait<_val>::ALPHABET[*y] << endl;
		//cout << "x row = " << Value_Trait<_val>::ALPHABET[get_dir(x, it.row(), _dir())] << endl;
		if (GetDir(x, i_max+1, _dir()) == BaseStringSet<_val>::pad_char)
		{
			column_max = std::numeric_limits<_score>::min();
		} 
		else 
		{
			i_max = i_max +1;
			// column_max += nm.LetterScore<_val>(mask_critical(*y), GetDir(x, i_max, _dir()));
			column_max += ScoreMatrix::get().letter_score(mask_critical(*y), GetDir(x, i_max, _dir()));
		}

		for(; it.valid() && GetDir(x, it.row(), _dir()) != BaseStringSet<_val>::pad_char; ++it) 
        {
            //.........
			const _score match_score = ScoreMatrix::get().letter_score(mask_critical(*y), GetDir(x, it.row(), _dir()));
			//match
			// const _score match_score = nm.LetterScore<_val>(mask_critical(*y), GetDir(x, it.row(), _dir()));
			//max diag, vgap and hgap
			const _score s = max(max(it.diag() + match_score, vgap), it.hgap_in());
			//cout << "column_max = " <<column_max<< endl;
			// cout << "s = " <<s<< endl;
			// if(s > column_max) 
			// {
			// 	column_max = s;
			// 	i_max = it.row();
			// 	//cout << "i_max = " <<i_max<< endl;
			// }

			const _score open = s - gap_open;
			vgap = max(vgap - gap_extend, open);
			it.hgap_out() = max(it.hgap_in() - gap_extend, open);
			it.score() = s;
		}

		if(column_max > max_score) 
		{
			max_score = column_max;
			j_best = j;
			i_best = i_max;
		}
		y = IncDir(y, _dir());
		++j;
	}
	//cout << "query = " <<Value_Trait<_val>::ALPHABET[query]<<", subject = "<<Value_Trait<_val>::ALPHABET[subject]<<endl;

	return Traceback<_val,_dir,_score>(query, subject, mtx.score_buffer(), band, gap_open, gap_extend, j_best, i_best, max_score, transcript_buf);

};


template<typename _val, typename _score, typename _traceback>
void floatingSW(const _val *query, LocalMatch<_val> &segment, 
int band, _score xdrop, _score gap_open, _score gap_extend, vector<char> &transcript_buf, 
const _traceback& = ScoreOnly (), const _score& = int())
{
	//cout << "floatingSW start" << endl;
	//cout << "query = " << Value_Trait<_val>::ALPHABET[*query] << endl;
	segment += floatingSWDirection<_val, Right, _score, _traceback>(query, segment.subject_, band, xdrop, gap_open, gap_extend, transcript_buf);
	const LocalMatch<_val> left (floatingSWDirection<_val,Left,_score,_traceback>(query, segment.subject_, band, xdrop, gap_open, gap_extend, transcript_buf));
	NucleotideMatrix nm;
	if (left.query_len_ > 0)
	{
		segment -= left;
		segment.query_begin_--;
		segment.subject_begin_--;
        //....
		const _val q = *query, s = mask_critical(*segment.subject_);
	    //cout <<"FloatingSW q = " <<Value_Trait<_val>::ALPHABET[q]<<", s = "<<Value_Trait<_val>::ALPHABET[s]<<endl;
		segment.score_ -= ScoreMatrix::get().letter_score(q, s);
		// segment.score_ -= nm.LetterScore<_val>(q, s);
		if(q == s)
			segment.identities_--;
		else
			segment.mismatches_--;
		segment.len_--;
		segment.subject_len_--;
		segment.query_len_--;
		//cout << "floatingSW end" << endl;
	}

};



#endif // __FLOATINGSMITHWATERMAN_H__