#ifndef __SEQUENCESET_H__
#define __SEQUENCESET_H__



#include <iostream>
#include <string>
#include "Sequence.h"
#include "StringSet.h"


using std::cout;
using std::endl;
using std::pair;

template<typename _t>
class SequenceSet : public BaseStringSet<_t>
{

    public:
    SequenceSet()
    {

    }

    SequenceSet(InputStreamController &file):BaseStringSet<_t>(file)
    {
        //StringSet<_t> = file;
    }

    void printStats() const
    {
        cout << "sequence = " <<this->getLength()<<endl;
        cout << " letters = " << this->letters() << endl;
    }
    //return min and max pair
    pair<size_t,size_t> lenBounds(size_t min_len) const
    {
        const size_t l = this->getLength();
        size_t max = 0, min = std::numeric_limits<size_t>::max();

        for (size_t i = 0; i < l; i++)
        {
            max = std::max(this->length(i),max);
            min = this->length(i) >= min_len ? std::min(this->length(i),min):min;
        }
        return pair<size_t, size_t>(min,max);
    }
	//put sequence into 256 partitions
	vector<size_t> Partition() const
	{
		vector<size_t> v;
		const size_t l = (this->letters()+ConstParameter::seqp-1) / ConstParameter::seqp;
		v.push_back(0);
		for(unsigned i=0;i<this->getLength();) 
        {
			size_t n = 0;
			while(i<this->getLength() && n < l)
				n += this->length(i++);
			v.push_back(i);
		}
		for(unsigned i=v.size();i<ConstParameter::seqp+1;++i)
        {
			v.push_back(this->getLength());
        }
		return v;
	}
    Sequence<const _t> windowInfix(size_t offset, unsigned &left) const
	{
		const _t* begin (this->data(offset));
		unsigned n (0);

		while(*begin != BaseStringSet<_t>::pad_char && n <= VATOptions::window) 
        {
			--begin;
			++n;
		}

		++begin;
		left = VATOptions::window + 1 - n;
		const _t* end (this->data(offset));
		n = 0;

		while(*end != BaseStringSet<_t>::pad_char && n < VATOptions::window) 
        {
			++end;
			++n;
		}
		return Sequence<const _t> (begin, end - begin);
	}

    Sequence<const _t> fixedWindowInfix(size_t offset) const
	{
		const _t* begin (this->data(offset));
		unsigned n (0);
		while(*begin != BaseStringSet<_t>::pad_char && n <= VATOptions::window) 
        {
			--begin;
			++n;
		}
		++begin;
		const _t* s (this->data(offset - VATOptions::window));
		return Sequence<const _t> (s, 2*VATOptions::window, begin - s);
	}

    // sequence<const _t> window_infix() const
    // {

    // }
	virtual ~SequenceSet()
	{

    }


};



#endif // __SEQUENCESET_H__