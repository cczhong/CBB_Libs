#ifndef __SEQREPRESENTATION_H__
#define __SEQREPRESENTATION_H__
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

template<typename T>
class SeqRepresentation
{
    public:

    SeqRepresentation(unsigned size,const char* chars,char mask, const char* mask_chars)
    {
        //set length of data_<_t> to invalid<_t>
        memset(data_,invalid,sizeof(data_));
        for (unsigned i = 0; i < size; i++)
        {
            //if i of char is not euqal with invalid, then break;
            assert(chars[i] != (char)invalid);
            data_[(long)chars[i]] = i;
            data_[(long)tolower(chars[i])] = i;
            cout << "chars = " <<chars[i]<<", i = "<<i<<" ,tolower = "<<(long)tolower(chars[i])<<", chars = "<<(long)chars[i]<< endl;
        }
        while (*mask_chars != 0)
        {
            const char str = *mask_chars;
            //cout << "mask_chars = " << mask_chars <<", str = "<<str<<endl;
            data_[(long) str] = mask;
            data_[(long)tolower(str)] = mask;
            ++mask_chars;
            cout <<"str = "<<str<< ", mask = " <<data_[(long)str]<<", tolower = "<<(long)tolower(str)<<", str = "<<(long)str<<endl;
        }
    }
    T operator()(char c) const
    {
        if(data_[(long)c] == invalid)
        {
            cout << "invalid sequence char exception" ;
        }
        return data_[(long)c];
    }

    private:
        static const T invalid = 0xff;
        T data_[256];
};


#endif // __SEQREPRESENTATION_H__