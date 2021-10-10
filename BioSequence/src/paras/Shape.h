#ifndef __SHAPE_H__
#define __SHAPE_H__


#include "ConstParameter.h"
#include "Value.h"
#include "Seed.h"
#include "../tool/HashFunction.h"
#include "Reduction.h"
#include "../sequence/SequenceSet.h"
#include "./Reduction.h"

const double background_freq[] = {0.25,0.25,0.25,0.25};

template<typename _val>
bool useSeedFreq()
{ 
	return false; 
}


class Shape
{
	public:
	Shape():
		length_ (0),
		weight_ (0),
		d_ (0),
		mask_ (0),
		rev_mask_ (0),
		id_ (0)
	{ 
        memset(positions_, 0, sizeof(uint32_t)*ConstParameter::max_seed_weight); 
    }

	Shape(const char *code, unsigned id):
		weight_ (0),
		mask_ (0),
		rev_mask_ (0),
		id_ (id)
	{
		//cout << "char code = " <<code<<endl;
		assert(id < ConstParameter::max_shapes);
		assert(strlen(code) <= 32);
		memset(positions_, 0, sizeof(uint32_t)*ConstParameter::max_seed_weight);//init position_ of size to 0
		unsigned i(0);
		for(;i<strlen(code);++i) {
			//cout << "code = " <<code[i]<< endl;
			rev_mask_ <<= 1;
			//cout << "rev mask = " <<rev_mask_<< endl;
			if (code[i] == '1')
			{
				assert(weight_ < ConstParameter::max_seed_weight);
				positions_[weight_] = i;
				++weight_;
				//cout << "positions weight = " <<positions_[weight_]<<endl;
				mask_ |= 1 << i;//  generate mask
				rev_mask_ |= 1;// generate spaced seeds shape 
				//cout << "rev mask = " <<rev_mask_<<", mask = "<<mask_<<", weight = "<<weight_<< endl;
			}
		}
		length_ = i;
		d_ = positions_[weight_/2-1];
		for (size_t i = 0; i < 32; i++)
		{
			//cout << "positions = " <<positions_[i]<<endl;
		}
	}
	
//set seeds by shape and return key(s)
	template<typename _val>
	bool setSeed(uint64_t &s, const _val *seq) const
	{
		s = 0;
		double f = 0;
		for (unsigned i = 0; i < weight_; ++i)
		{
			_val l = seq[positions_[i]];
			if (l == ValueFeature<_val>::mask_char || l == BaseStringSet<_val>::pad_char)
				return false;
			l = mask_critical(l);
			unsigned r ;
			switch (ValueFeature<_val>::alpha[l])
			{
					case 'A':
						r = 0;
						break;
					case 'C':
						r = 1;
						break;
					case 'G':
						r = 2;
						break;
					case 'T':
						r = 3;
						break;
					default:
						break;
			}
			f += background_freq[r];
			s *= 4;
			s += uint64_t(r);
		}					 
		if(useSeedFreq<_val>() && f > VATOptions::max_seed_freq) return false;
#ifdef EXTRA
		s = murmur_hash()(s);
		cout << "murmur hash = " <<s<< endl;
#endif
		return true;
	}
	// template<typename _val>
	// inline unsigned ReduceAlphabet(char a)
	// {
	// 	switch (a)
	// 	{
	// 	case 'A':
	// 		return 0;
	// 		break;
	// 	case 'C':
	// 		return 1;
	// 		break;
	// 	case 'G':
	// 		return 2;
	// 		break;
	// 	case 'T':
	// 		return 3;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	// }

	template<typename _val>
	inline bool	isLowFreq(const _val *seq) const
	{
		double f = 0;
		for(unsigned i=0;i<weight_;++i) {
			_val l = seq[positions_[i]];
			if(l == ValueFeature<_val>::mask_char || l == BaseStringSet<_val>::pad_char)
				return false;
			l = mask_critical(l);
			unsigned r = Reduction<_val>::reduction(l);
			f += background_freq[r];
		}
		return !useSeedFreq<_val>() || f <= VATOptions::max_seed_freq;
	}

	template<typename _val>
	inline bool	isLowFreqRev(const _val *seq) const
	{
		double f = 0;
		for(unsigned i=0;i<weight_;++i) {
			_val l = seq[(int)positions_[i]-(int)length_];
			if(l == ValueFeature<_val>::mask_char || l == BaseStringSet<_val>::pad_char)
				return false;
			l = mask_critical(l);
			unsigned r = Reduction<_val>::reduction(l);
			f += background_freq[r];
		}
		return !useSeedFreq<_val>() || f <= VATOptions::max_seed_freq;
	}
// 1s characters were stored in positions_
	//static Reduction<_val> re;

	uint32_t length_, weight_, positions_[ConstParameter::max_seed_weight], d_, mask_, rev_mask_, id_;

};


#endif // __SHAPE_H__