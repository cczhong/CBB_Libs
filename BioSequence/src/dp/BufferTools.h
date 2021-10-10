#ifndef __BUFFERTOOLS_H__
#define __BUFFERTOOLS_H__


#include <vector>

using std::vector;
using std::pair;


/**
 * col_size = total = size + padding
 * init = NEG_MIN
 * padding = band + 1
 * padding_front = 1
 * data_ 
 */
template<typename _t>
class ExtendingBuffer
{
	public:
	//score_->init(band_max_, band_ + 1, 1, NEG_MIN);
	//2*band+1 = band max
	inline void init(size_t size, size_t padding, size_t padding_front, _t init)
	{
		//cout << "size = " <<size<<", padding = "<<padding<< ", padding_front = "<<padding_front<<endl;
		const size_t total = size + padding;
		data_.resize(total);
		col_size_ = total;
		for(size_t i=0;i<total;++i)
			data_[i] = init;
		init_ = init;
		center_.clear();
		center_.push_back(-1);
	}

	inline pair<_t*,_t*> get(int center)
	{
		data_.resize(data_.size() + col_size_);
		_t* ptr = last();
		for(size_t i=0;i<col_size_;++i)
			ptr[i] = init_;
		center_.push_back(center);
		//cout << "center = " <<center<< endl;
		return pair<_t *, _t *>(ptr - col_size_, ptr);
	}
	//last = data.end() - col_size
	inline _t* last()
	{ 
        return &*(data_.end() - col_size_); 
    }

	const _t* column(int col) const
	{ 
        return &data_[col_size_*col]; 
    }

	int center(int col) const
	{ 
        return center_[col]; 
    }

private:
	vector<_t> data_;
	vector<int> center_;
	size_t col_size_;
	_t init_;

};

#endif // __BUFFERTOOLS_H__