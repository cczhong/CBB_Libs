#ifndef __SEQUENCETYPE_H__
#define __SEQUENCETYPE_H__


/**
 * Define Protein and Nucleotide
 * Letter_pro for Protein
 * LetterNucletide for Nucleotide
 * 
*/


class LetterProtein
{

};

class LetterNucletide
{

};


/**
 * private member char value
 */
template<typename _t>
class SequenceTypes
{
    public:
    SequenceTypes():value_()
    {

    }

    SequenceTypes(int i)
    {
        value_ = ((char)i);
    }

    SequenceTypes(char str)
    {
        value_ = str;
    }

    SequenceTypes(unsigned i)
    {
        value_ = ((char)i);
    }

    operator char() const
    {
        return value_;
    }

    operator int() const
    {
        return (int)value_;
    }

    operator unsigned() const
    {
        return (unsigned)value_;
    }

    operator long() const
    {
        return (long)value_;
    }

    bool operator ==(const SequenceTypes &vt) const
    {
        return value_ == vt.value_;
    }

    bool operator!=(const SequenceTypes &vt) const
    {
        return value_ != vt.value_;
    }

    private:
        char value_;
};

typedef SequenceTypes<LetterNucletide> Nucleotide;
typedef SequenceTypes<LetterProtein> Protein;

// typedef SequenceTypes Nucleotide;
// typedef SequenceTypes Protein;


#endif // __SEQUENCETYPE_H__