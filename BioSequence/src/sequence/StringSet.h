#ifndef __STRINGSET_H__
#define __STRINGSET_H__



#include <vector>
#include "../tool/BinaryFile.h"
#include "SequenceSet.h"
#include "Sequence.h"

using std::vector;
using namespace std;

template<typename _t, char _pchar = 0xff, size_t _padding = 1>
class BaseStringSet
{
    public:
	static const unsigned peri_pad = 256;//
	static const _t pad_char;//PADDING CHAR

	BaseStringSet():
		data_ (peri_pad)
	{ 
		limits_.push_back(peri_pad); 
	}

	void finishReserve()
	{
		data_.resize(rawLen() + peri_pad);
		for(unsigned i=0;i<peri_pad;++i) 
		{
			data_[i] = pad_char;
			data_[rawLen()+i] = pad_char;
		}
	}

	void push_back(const vector<_t> &v)
	{
		limits_.push_back(rawLen() + v.size() + _padding);
		data_.insert(data_.end(), v.begin(), v.end());
		data_.insert(data_.end(), _padding, pad_char);
	}

	void fill(size_t n, _t v)
	{
		limits_.push_back(rawLen() + n + _padding);
		data_.insert(data_.end(), n, v);
		data_.insert(data_.end(), _padding, pad_char);
	}

	_t* ptr(size_t i)
	{ return &data_[limits_[i]]; }

	const _t* ptr(size_t i) const
	{ return &data_[limits_[i]]; }

	size_t length(size_t i) const
	{ return limits_[i+1] - limits_[i] - _padding; }

	size_t getLength() const
	{ return limits_.size() - 1; }

	void save(OutputStreamController &file) const
	{
		file.write(limits_);
		file.write(data_);
	}

	BaseStringSet(InputStreamController &file)
	{
		file.read(limits_);
		file.read(data_);
	}

	static void skip(InputStreamController &file)
	{
		file.skip_vector<size_t>();
		file.skip_vector<_t>();
	}

	size_t rawLen() const
	{ return limits_.back(); }

	size_t letters() const
	{ return rawLen() - getLength() - peri_pad; }

	_t* data(ptrdiff_t p = 0)
	{ return &data_[p]; }

	const _t* data(ptrdiff_t p = 0) const
	{ return &data_[p]; }

	size_t position(const _t* p) const
	{ return p - data(); }

	size_t position(size_t i, size_t j) const
	{ return limits_[i] + j; }

	std::pair<size_t,size_t> localPosition(size_t p) const
	{
		size_t i = std::upper_bound(limits_.begin(), limits_.end(), p) - limits_.begin() - 1;
		return std::pair<size_t,size_t> (i, p - limits_[i]);
	}

	Sequence<const _t> operator[](size_t i) const
	{ return Sequence<const _t> (ptr(i), length(i)); }

	Sequence<_t> operator[](size_t i)
	{ return Sequence<_t> (ptr(i), length(i)); }
    
private:
    vector<_t> data_;//store char 
    vector<size_t> limits_;//store pos
};

template<typename _t, char _pchar, size_t _padding> const _t BaseStringSet<_t,_pchar,_padding>::pad_char = _pchar;

#endif // __STRINGSET_H__