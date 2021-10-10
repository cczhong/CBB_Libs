#ifndef __SCOREPROFILE_H__
#define __SCOREPROFILE_H__
#include <vector>
#include "../sequence/Sequence.h"
#include "ScoreVector.h"
#include "../paras/Value.h"

using std::vector;

class SequenceStream
{
    public:
	SequenceStream():
		next (buffer_len),
		mask (0)
	{ 

    }
	inline void reset()
	{
		next = buffer_len;
		mask = 0;
	}
	template<typename _val, typename _score>
	inline const __m128i& get(const typename vector<Sequence<const _val> >::const_iterator &begin,
					   const typename vector<Sequence<const _val> >::const_iterator &end,
					   unsigned pos,
					   const _score&)
	{
		if(next == buffer_len)
			fill<_val,_score>(begin, end, pos);
		return data_[next++];
	}
	template<typename _val, typename _score>
	inline void fill(const typename vector<Sequence<const _val> >::const_iterator &begin,
		  	  const typename vector<Sequence<const _val> >::const_iterator &end,
		 	  unsigned pos)
	{
		memset(data_, ValueFeature<_val>::mask_char, buffer_len*16);
		unsigned n = 0;
		typename vector<Sequence<const _val> >::const_iterator it (begin);
		assert(pos < it->length());
		const unsigned read_len (std::min(unsigned(buffer_len), static_cast<unsigned>(it->length())-pos));
		while(it < end) {
			const uint8_t *src (reinterpret_cast<const uint8_t*>(it->data()) + pos);
			_score *dest (reinterpret_cast<_score*>(data_) + n);
			int clip (int(pos) - it->clipping_offset_);
			if((mask & (1 << n)) == 0) {
				if(copyChar(src, dest, mask, n, clip))
				if(read_len > 1 && copyChar(src, dest, mask, n, clip))
				if(read_len > 2 && copyChar(src, dest, mask, n, clip))
				if(read_len > 3) copyChar(src, dest, mask, n, clip);
			}
			++it;
			++n;
		}
		next = 0;
	}
	template<typename _score>
	static inline bool copyChar(const uint8_t*& src, _score*& dest, unsigned &mask, unsigned n, int &clip)
	{
		if(clip++ < 0) {
			dest += 16/sizeof(_score);
			++src;
			return true;
		}
		if(*src == 0xff) {
			mask |= 1 << n;
			return false;
		}
		*dest = *(src++) & 0x7f;
		dest += 16/sizeof(_score);
		return true;
	}
	static const unsigned buffer_len = 4;
	__m128i data_[buffer_len];
	unsigned next;
	unsigned mask;
};

template<typename _score>
class ScoreProfile
{
    public:
	ScoreProfile(){}
	template<typename _val>
	void inline SetValue(const __m128i &seq)
	{
		assert(sizeof(data_)/sizeof(ScoreVector<_score>) >= ValueFeature<_val>::ALPHABET_SIZE);
		for(unsigned j=0;j<ValueFeature<_val>::ALPHABET_SIZE;++j)
			data_[j] = ScoreVector<_score> (j, seq);
	}

	template<typename _val>
	const inline ScoreVector<_score>& get(_val i) const
	{ 
        return data_[(int)i]; 
    }

	ScoreVector<_score> data_[25];

};



#endif // __SCOREPROFILE_H__