#ifndef __SCALARDPMATRIX_H__
#define __SCALARDPMATRIX_H__

#include <vector>
#include <boost/thread.hpp>
#include "../tool/DoubleBuffer.h"
#include "BufferTools.h"

using std::vector;
using std::pair;
using boost::thread_specific_ptr;

class ScoreOnly { };
class TraceBack { };

template<typename _score, typename _traceback>
class ScoreBuffer 
{

};
//score scoreonly
template<typename _score>
class ScoreBuffer<_score,ScoreOnly>
{
	public:
	typedef Double_buffer<_score> Type;
};
//score traceback
template<typename _score>
class ScoreBuffer<_score,TraceBack>
{
	public:
	typedef ExtendingBuffer<_score> Type;
};

template<typename _score, typename _traceback>
class ScalarDPMatrix
{

    public:
	/**
	 * offset = i_ - i + band
	 * i_ = max(i - band, 0) //row
	 * end = score_.second + 2*band + 1
	 * pair score = score_->get(i)
	 * pair hgap = hgap_->get(int ())
	 * delta = i - current_i_
	 * i = std::max(current_i_, i_max + 1)
	 */
	struct Column_iterator
	{

		inline Column_iterator(const pair<_score*,_score*> &score, const pair<_score*,_score*> &hgap, int j, int i, int delta, int band):
			score_ (score),
			hgap_ (hgap),
			end_ (score_.second + 2*band + 1),
			i_ (std::max(i - band, 0))
		{
			// cout << "i - band = " <<i - band<< endl;
			assert(delta >= 0 && j >= 0 && i >= 0 && band >= 0);
			if(j == 0)
				score_.first[band] = 0;
			const int offset = i_ - i + band;
			score_.second += offset;
			hgap_.second += offset;
			hgap_.first += offset + delta;
			score_.first += offset + delta - 1;
		}

		inline int row() const
		{
 
            return i_; 
        }

		inline bool valid() const
		{ 
            return score_.second < end_; 
        }

		inline _score& score()
		{ 
            return *score_.second; 
        }

		inline _score diag() const
		{ 
            return *score_.first; 
        }

		inline _score hgap_in() const
		{ 
            return *hgap_.first; 
        }

		inline _score& hgap_out()
		{ 
            return *hgap_.second; 
        }

		inline void operator++()
		{
			++i_;
			++score_.first;
			++score_.second;
			++hgap_.first;
			++hgap_.second;
		}

	private:
		pair<_score*,_score*> score_, hgap_;
		const _score* const end_;
		int i_;

	};

	inline Column_iterator column(int j, int i_max)
	{
		//cout << "j = " <<j<<", i_max = "<<i_max<< endl;
		int i = std::max(current_i_, i_max + 1), delta = i - current_i_;
		//cout << "i =" <<i<< endl;
		current_i_ = i;
		return Column_iterator (score_->get(i), hgap_->get(int ()), j, i, delta, band_);
	}

	inline ScalarDPMatrix(int band):
		band_ (band),
		band_max_ (2*band+1),
		current_i_ (-1),
		score_ (score_ptr),
		hgap_ (hgap_ptr)
	{
		//cout << "band = " <<band<< endl;
		score_->init(band_max_, band_+1, 1, NEG_MIN);
		hgap_->init(band_max_, band_+1, 1, NEG_MIN);
	}

	const typename ScoreBuffer<_score,_traceback>::Type& score_buffer() const
	{ 
        return *score_; 
    }

	static const _score NEG_MIN = -65536;

private:

	const int band_, band_max_;
	int current_i_;
	Tls<typename ScoreBuffer<_score,_traceback>::Type> score_;
	Tls<Double_buffer<_score> > hgap_;
	static thread_specific_ptr<typename ScoreBuffer<_score,_traceback>::Type> score_ptr;
	static thread_specific_ptr<Double_buffer<_score> > hgap_ptr;

};

template<typename _score, typename _traceback> thread_specific_ptr<typename ScoreBuffer<_score,_traceback>::Type> ScalarDPMatrix<_score,_traceback>::score_ptr;
template<typename _score, typename _traceback> thread_specific_ptr<Double_buffer<_score> > ScalarDPMatrix<_score,_traceback>::hgap_ptr;


#endif // __SCALARDPMATRIX_H__