#ifndef __BINARY_BUFFER_H__
#define __BINARY_BUFFER_H__
#include <iostream>
#include<vector>
#include<string>
#include<string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class BinaryBuffers : public vector<char>
{
    public:
    struct Iterator
	{
		Iterator(vector<char>::const_iterator begin, vector<char>::const_iterator end):
			ptr_ (begin),
			end_ (end)
		{ }
		Iterator& operator>>(uint32_t &x)
		{ read(x); return *this; }
		Iterator& operator>>(uint8_t &x)
		{ read(x); return *this; }
		template<typename _t>
		void read(_t &x)
		{
			check(sizeof(_t));
			x = *(_t*)(&*ptr_);
			//cout << "sizeof(_t)" <<sizeof(_t)<<endl;
			ptr_ += sizeof(_t);
		}
		template<typename _t>
		void read(vector<_t> &v, size_t count)
		{
			const size_t l = sizeof(_t) * count;
			check(l);
			v.resize(count);
			memcpy(v.data(), &*ptr_, l);
			ptr_ += l;
		}
		void read_packed(uint8_t length, uint32_t &dst)
		{
			switch(length) {
			case 0: uint8_t x; read(x); dst = x; break;
			case 1: uint16_t y; read(y); dst = y; break;
			case 2: read(dst);
			}
		}
		Iterator& operator>>(string &dst)
		{
			dst.clear();
			char c;
			while(read(c), c != '\0')
				dst.push_back(c);
			return *this;
		}
		bool good() const
		{ return ptr_ < end_; }
	private:
		void check(size_t size) const
		{ if(ptr_+size > end_) throw std::runtime_error("Unexpected end of file."); }
		vector<char>::const_iterator ptr_, end_;
	};

	Iterator begin() const
	{ return Iterator (vector<char>::begin(), vector<char>::end()); }


};

#endif // __BINARY_BUFFER_H__