#include "PackedSequence.h"
#include <vector>



bool has_n(const sequence<const Nucleotide> &seq)
{
    for (unsigned i = 0; i < seq.length();i++)
    {
        if(seq[i] == Value_Trait<Nucleotide>::MASK_CHAR)
        {
            return true;
        }
        return false;
    }
} 

