#ifndef __VALUE_H__
#define __VALUE_H__

#include "SequenceType.h"
#include "ConstParameter.h"
#include "Options.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "VATExceptions.h"
using std::cout;
using std::endl;

typedef enum
{
    amino_acid = 0,
    nucletide = 1
} SeqType;


size_t queryContexts()
{
        return 1;
}

template<typename _t>
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
        }
        while (*mask_chars != 0)
        {
            const char str = *mask_chars;
            data_[(long) str] = mask;
            data_[(long)tolower(str)] = mask;
            ++mask_chars;
        }
    }
    _t operator()(char c) const
    {
        if(data_[(long)c] == invalid)
        {
            cout << "invalid sequence char exception" ;
        }
        return data_[(long)c];
    }

    private:
        static const _t invalid;
        _t data_[256];
};


template<typename _t>
class ValueFeature
{
   
};

template<>
class ValueFeature<Nucleotide>
{
    public:
    /* data */
    enum
    {
        ALPHABET_SIZE = 5
    };
    static const Nucleotide mask_char;//MASK_CHAR
    static const char *alpha;//ALPHABET
    static const SeqRepresentation<Nucleotide> from_char_;
};

template<>
class ValueFeature<Protein>
{
    public:
    
	enum 
    { 
        ALPHABET_SIZE = 25 
    };
	static const Protein				mask_char;
	static const char*					alpha;
	static const SeqRepresentation<Protein>	from_char_;
};

template<>
class ValueFeature<const Nucleotide>: public ValueFeature<Nucleotide>
{

};
template<>
class ValueFeature<const Protein> : public ValueFeature<Protein>
{ 

};

template <> const Nucleotide SeqRepresentation<Nucleotide>::invalid = 0xff;
template <> const Protein SeqRepresentation<Protein>::invalid = 0xff;
//Nucleotide
const Nucleotide ValueFeature<Nucleotide>::mask_char = 4;
const char *ValueFeature<Nucleotide>::alpha = "ACGTN";
const SeqRepresentation<Nucleotide> ValueFeature<Nucleotide>::from_char_(5,ValueFeature<Nucleotide>::alpha,ValueFeature<Nucleotide>::mask_char,"MRWSYKVHDBX");

//Protein
const Protein ValueFeature<Protein>::mask_char = 23;
const char* ValueFeature<Protein>::alpha = "ARNDCQEGHILKMFPSTWYVBJZX*";
const SeqRepresentation<Protein> ValueFeature<Protein>::from_char_(ValueFeature<Protein>::ALPHABET_SIZE, ValueFeature<Protein>::alpha, ValueFeature<Protein>::mask_char, "UO-");


char toChar(Protein protein)
{ 
    return ValueFeature<Protein>::alpha[protein]; 
}

template<>
class ValueFeature<char>
{
    public:
    static char from_char_(char str)
    {
        return str;
    }
};


#endif // __VALUE_H__