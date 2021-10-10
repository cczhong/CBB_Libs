#ifndef __VECTORSCORE_H__
#define __VECTORSCORE_H__

#ifdef __SSSE3__
#include <tmmintrin.h>
#endif

#include <stdint.h>
#include <emmintrin.h>
#include "../paras/Options.h"
#include "../tool/Utils.h"

template<typename _score>
class ScoreTraits
{
    public:
	static const unsigned channels = 1;
	enum 
    { 
            zero = 0, byte_size = 4 
    };
	typedef bool Mask;
};

template<>
class ScoreTraits<uint8_t>
{
	//static const unsigned channels = 16;
    public:
	enum { channels = 16, zero = 0x00, byte_size = 1 };
	typedef uint16_t Mask;
};

template<typename _score>
class ScoreVector
{ 

};

template<>
class ScoreVector<uint8_t>
{
	public:

	ScoreVector()
	{
		data_ = _mm_set1_epi8(ScoreTraits<uint8_t>::zero);
	}

	explicit ScoreVector(int x):
		data_ (_mm_set(x))
	{ 

    }

	explicit ScoreVector(__m128i data):
		data_ (data)
	{ 

    }

	explicit ScoreVector(unsigned a, const __m128i &seq)
	{
		if(VATOptions::have_ssse3) {
#ifdef __SSSE3__
			set_ssse3(a, seq);
#else
			set_generic(a, seq);
#endif
		} else
			set_generic(a, seq);
	}

	void set_ssse3(unsigned a, const __m128i &seq)
	{
#ifdef __SSSE3__
		const __m128i *row = reinterpret_cast<const __m128i*>(&score_matrix::get().matrix8u()[a << 5]);

		__m128i high_mask = _mm_slli_epi16(_mm_and_si128(seq, _mm_set1_epi8(0x10)), 3);
		__m128i seq_low = _mm_or_si128(seq, high_mask);
		__m128i seq_high = _mm_or_si128(seq, _mm_xor_si128(high_mask, _mm_set1_epi8(0x80)));

		__m128i r1 = _mm_load_si128(row);
		__m128i r2 = _mm_load_si128(row+1);
		__m128i s1 = _mm_shuffle_epi8(r1, seq_low);
		__m128i s2 = _mm_shuffle_epi8(r2, seq_high);
		data_ = _mm_or_si128(s1, s2);
#endif
	}

	void set_generic(unsigned a, const __m128i &seq)
	{
		// //const uint8_t* row (&score_matrix::get().matrix8u()[a<<5]);
		// const uint8_t* seq_ptr (reinterpret_cast<const uint8_t*>(&seq));
		// uint8_t* dest (reinterpret_cast<uint8_t*>(&data_));
		// for(unsigned i=0;i<16;i++)
		// 	*(dest++) = row[*(seq_ptr++)];
	}

	ScoreVector(const uint8_t* s):
		data_ (_mm_loadu_si128(reinterpret_cast<const __m128i*>(s)))
	{ 

    }

	ScoreVector operator+(const ScoreVector &rhs) const
	{
		return ScoreVector (_mm_adds_epu8(data_, rhs.data_));
	}

	ScoreVector operator-(const ScoreVector &rhs) const
	{
		return ScoreVector (_mm_subs_epu8(data_, rhs.data_));
	}

	ScoreVector& operator-=(const ScoreVector &rhs)
	{
		data_ = _mm_subs_epu8(data_, rhs.data_);
		return *this;
	}

	void unbias(const ScoreVector &bias)
	{ 
        this->operator -=(bias); 
    }

	int operator [](unsigned i) const
	{
		return *(((uint8_t*)&data_)+i);
	}

	void set(unsigned i, uint8_t v)
	{
		*(((uint8_t*)&data_)+i) = v;
	}

	ScoreVector& max(const ScoreVector &rhs)
	{
		data_ = _mm_max_epu8(data_, rhs.data_);
		return *this;
	}

	ScoreVector& min(const ScoreVector &rhs)
	{
		data_ = _mm_min_epu8(data_, rhs.data_);
		return *this;
	}

	friend ScoreVector max(const ScoreVector& lhs, const ScoreVector &rhs)
	{
		return ScoreVector (_mm_max_epu8(lhs.data_, rhs.data_));
	}

	friend ScoreVector min(const ScoreVector& lhs, const ScoreVector &rhs)
	{
		return ScoreVector (_mm_min_epu8(lhs.data_, rhs.data_));
	}

	uint16_t cmpeq(const ScoreVector &rhs) const
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi8(data_, rhs.data_));
	}

	__m128i cmpeq2(const ScoreVector &rhs) const
	{
		return _mm_cmpeq_epi8(data_, rhs.data_);
	}

	uint16_t cmpgt(const ScoreVector &rhs) const
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi8(data_, rhs.data_));
	}

	__m128i data_;

};
#endif // __VECTORSCORE_H__