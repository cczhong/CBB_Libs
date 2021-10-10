#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include "../paras/Value.h"
#include "../tool/BinaryBuffer.h"
#include "../tool/TextBuffer.h"

/**
 * _val = char or protein
 */
template<typename _val>
class Sequence
{
    public:
    Sequence()
    {
        len_ = 0;
        clipping_offset_ = 0;
        data_ = 0;
    }

    Sequence(_val *data, size_t l, int clip_offset = 0)
    {
        len_ = l;
        clipping_offset_ = clip_offset;
        data_ = data;
    }

    size_t length() const
    {
        return len_;
    }

    size_t clippedLength() const
    {
        return len_ - clipping_offset_;
    }

    size_t alignedClip(unsigned padding) const
    {
        return clipping_offset_ > padding ? clipping_offset_:0;
    }

    const _val* data() const
    {
        return data_;
    }

    const _val* clippedData() const
    {
        return data_ + clipping_offset_;
    }

    const _val* alignedData(unsigned padding) const
    {
        return data_ + padding;
    }

   	const _val& operator [](size_t i) const
	{
		return data_[i];
	}

	_val& operator [](size_t i)
	{
		return data_[i];
	}


	bool empty() const
	{ 
        return len_ == 0;
    }
	const char* cStr() const
	{ 
        return reinterpret_cast<const char*>(data_);
    }
	size_t print(char *ptr, unsigned begin, unsigned len) const
	{
		for(unsigned i=begin;i<begin+len;++i)
			*(ptr++) = to_char(data_[i]);
		return len;
	}

	friend std::ostream& operator<<(std::ostream &os, const Sequence &s)
	{
		for(unsigned i=0;i<s.len_;++i)
			os << ValueFeature<_val>::alpha[s.data_[i]];
		return os;
	}

	friend TextBuffers& operator<<(TextBuffers &buf, const Sequence &s)
	{
		for(unsigned i=0;i<s.len_;++i)
			buf << ValueFeature<_val>::alpha[s.data_[i]];
		return buf;
	}
     
    size_t len_;
    int clipping_offset_;
    _val *data_;
};

#endif // __SEQUENCE_H__