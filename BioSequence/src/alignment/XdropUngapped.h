#ifndef __XDROPUNGAPPED_H__
#define __XDROPUNGAPPED_H__

#include "../paras/Options.h"
#include "../sequence/SequenceSet.h"
#include "../paras/NuclMatrix.h"

using namespace std;

template <typename _val, typename _locr, typename _locq>
int xdropUngapped(const _val *query, const _val *subject, unsigned seed_len, unsigned &delta, unsigned &len)
{

    int score = 0, st = 0;
    unsigned n = 0;
    delta = 0;
    NucleotideMatrix nm;
    const _val *q = query - 1;
    const _val *s = subject - 1; 
    // to left
    while (score - st < 20 && *q != BaseStringSet<_val>::pad_char && *s != BaseStringSet<_val>::pad_char)
    {
       // cout << "xdrop starting left" << endl;

        st += nm.LetterScore<_val>(*q, mask_critical(*s));
        score = std::max(score,st);
        --q;
        --s;
        ++delta;
    }

    q = query + seed_len;
    s = subject + seed_len;
    st = score;
    // to right
    while (score - st < 20 && *q != BaseStringSet<_val>::pad_char && *s != BaseStringSet<_val>::pad_char)
    {
        st += nm.LetterScore<_val>(*q, mask_critical(*s));
        score = std::max(score,st);
        ++q;
        ++s;
        ++n;
    }

    for(unsigned i=0;i<seed_len;++i)
    {
        score += nm.LetterScore<_val>(query[i],subject[i]);

    }
	len = delta + n + seed_len;
    //cout << "xdrop ending" << endl;

    return score;
};



/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
class XDrop
{
    public:
    int xdropUngapped(const Protein *query, const Protein *subject, unsigned seed_len, unsigned &delta, unsigned &len)
    {

        int score = 0, st = 0;
        unsigned n = 0;
        delta = 0;
        NucleotideMatrix nm;
        const Protein *q = query - 1;
        const Protein *s = subject - 1; 
        // to left
        while (score - st < 20 && *q != BaseStringSet<Protein>::pad_char && *s != BaseStringSet<Protein>::pad_char)
        {

            st += nm.LetterScore<Protein>(*q, mask_critical(*s));
            score = std::max(score,st);
            --q;
            --s;
            ++delta;
        }

        q = query + seed_len;
        s = subject + seed_len;
        st = score;
        // to right
        while (score - st < 20 && *q != BaseStringSet<Protein>::pad_char && *s != BaseStringSet<Protein>::pad_char)
        {
            st += nm.LetterScore<Protein>(*q, mask_critical(*s));
            score = std::max(score,st);
            ++q;
            ++s;
            ++n;
        }

        for(unsigned i=0;i<seed_len;++i)
        {
            score += nm.LetterScore<Protein>(query[i],subject[i]);

        }
        len = delta + n + seed_len;

        return score;
    };

};

#endif // __XDROPUNGAPPED_H__