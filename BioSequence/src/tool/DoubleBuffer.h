

#ifndef DOUBLE_BUFFER_H_
#define DOUBLE_BUFFER_H_

#include <vector>

using std::vector;
using std::pair;
/**
 * size = band_max = 2*band + 1
 * padding = band + 1
 * init = NEG_MIN
 */
template<typename _t>
class Double_buffer
{
	public:
	//score_->init(band_max_, band_ + 1, 1, NEG_MIN);
	//2*band+1 = band max
	inline void init(size_t size, size_t padding, size_t padding_front, _t init)
	{
		const size_t total = size + padding + padding_front;
		data_.resize(total*2);
		ptr1 = &data_[padding_front];
		ptr2 = &data_[total+padding_front];
		for(size_t i=0;i<total*2;++i)
			data_[i] = init;
	}

	inline pair<_t*,_t*> get(int)
	{ 
		std::swap(ptr1, ptr2); return pair<_t*,_t*> (ptr2, ptr1);
	}

	inline _t* last()
	{ 
		return ptr1;
	}

private:
	_t *ptr1, *ptr2;
	vector<_t> data_;

};

#endif /* DOUBLE_BUFFER_H_ */
