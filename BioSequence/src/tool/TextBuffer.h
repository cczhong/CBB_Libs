#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include<stdlib.h>
#include<string.h>
#include<string>
#include<limits>
#include<vector>
#include <stdint.h>
#include <stdio.h>
#include "../paras/VATExceptions.h"

class TextBuffers
{
    public:
    TextBuffers():		
        data_ (0),
		ptr_ (data_)
    {

    }
    void Reserve(size_t n)
    {
		const size_t s = ptr_ - data_, new_size = s + n + block_size - ((s+n) & (block_size-1));
		data_ = (char*)realloc(data_, new_size);
		ptr_ = data_ + s;
		if(data_ == 0) throw MemAllocException();
    }
    void operator+=(size_t i)
    {
        ptr_ += i;
    }

    operator char*() const
    {
        return ptr_;
    }

    char* get_begin() const
    {
        return data_;
    }

    void clear()
    {
        ptr_ = data_;
    }

    template<typename _t>
    TextBuffers& write(const _t& data)
    {
        Reserve(sizeof(_t));
        *reinterpret_cast<_t *>(ptr_) = data;
        ptr_ += sizeof(_t);
        return *this;
    }
    void write_c_str(const char* str)
    {
        const size_t l = strlen(str) + 1;
        Reserve(l);
        memcpy(ptr_,str,l);
        ptr_ += l;
    }
    void write_c_str(const char *str,size_t len)
    {
        Reserve(len+1);
        memcpy(ptr_,str,len);
        ptr_ += len;
        *(ptr_++) ='\0';
    }

    TextBuffers& write_pack(unsigned i)
    {
        if (i<=(unsigned)std::numeric_limits<uint8_t>::max())
        {
            write((uint8_t)i);
        }
        else if (i<=(unsigned)std::numeric_limits<uint16_t>::max())
        {
            write((uint16_t)i);
        }
        else
        {
            write(i);
        }
        return *this;
    }

    size_t size() const
    {
        return ptr_ - data_;
    }

	TextBuffers& operator<<(const std::string &s)
	{
		const size_t l = s.length();
		Reserve(l);
		memcpy(ptr_, s.c_str(), l);
		ptr_ += l;
		return *this;
	}

	TextBuffers& operator<<(const char* s)
	{
		const size_t l = strlen(s);
		Reserve(l);
		memcpy(ptr_, s, l);
		ptr_ += l;
		return *this;
	}

	TextBuffers& operator<<(char c)
	{
		Reserve(1);
		*(ptr_++) = c;
		return *this;
	}

	TextBuffers& operator<<(uint32_t x)
	{
		
		Reserve(16);
		ptr_ += sprintf(ptr_, "%u", x);
		return *this;
	}

	TextBuffers& operator<<(int x)
	{
		
		Reserve(16);
		ptr_ += sprintf(ptr_, "%i", x);
		return *this;
	}

	TextBuffers& operator<<(double x)
	{
		Reserve(32);
		ptr_ += sprintf(ptr_, "%.1lf", x);
		return *this;
	}

	TextBuffers& print_e(double x)
	{
		Reserve(32);
		ptr_ += sprintf(ptr_, "%.1le", x);
		return *this;
	}

	template<typename _t>
	TextBuffers& operator<<(const std::vector<_t> &v)
	{
		const size_t l = v.size() * sizeof(_t);
		Reserve(l);
		memcpy(ptr_, v.data(), l);
		ptr_ += l;
		return *this;
	}

    protected:
        enum
        {
            block_size = 65535
        };
        char *data_, *ptr_;
        /* data */
};



#endif // __TEXT_BUFFER_H__