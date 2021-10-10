#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__



#include <vector>
#include "Value.h"
#include "SequenceType.h"
using std::vector;

class Translator
{
    public:

    
    static const Nucleotide reverse_nucleotide[5];



    static const Protein find[5][5][5];
    static const Protein find_reverse[5][5][5];
    static const Protein stop;


    static Nucleotide getReverseComplement(Nucleotide nucleotide)
    { 
        return reverse_nucleotide[(int) nucleotide]; 
    }
    static vector<Nucleotide> reverse(const vector<Nucleotide> &seq)
	{
		vector<Nucleotide> r;
		for(vector<Nucleotide>::const_reverse_iterator i=seq.rbegin(); i!=seq.rend(); ++i)
			r.push_back(getReverseComplement(*i));
		return r;
	}

	static Protein getProtein(vector<Nucleotide> const &dnaSequence, size_t pos)
	{
		return find[(int)dnaSequence[pos]][(int)dnaSequence[pos + 1]][(int)dnaSequence[pos + 2]];
	}

	static Protein getProteinReverse(vector<Nucleotide> const &dnaSequence, size_t pos)
	{
		return find_reverse[(int)dnaSequence[pos + 2]][(int)dnaSequence[pos + 1]][(int)dnaSequence[pos]];
	}


	static size_t translate(vector<Nucleotide> const &dnaSequence, vector<Protein> *proteins)
	{
		size_t length_ = dnaSequence.size(), d, n;
		proteins[0].resize(d = length_ / 3);
		proteins[3].resize(d);
		n = 2 * d;
		proteins[1].resize(d = (length_ - 1) / 3);
		proteins[4].resize(d);
		n += 2 * d;
		proteins[2].resize(d = (length_ - 2) / 3);
		proteins[5].resize(d);
		n += 2 * d;

		size_t r = length_ - 2;
		unsigned pos = 0;
		unsigned i = 0;
		while (r > 2)
		{
			proteins[0][i] = getProtein(dnaSequence, pos++);
			proteins[3][i] = getProteinReverse(dnaSequence, --r);
			proteins[1][i] = getProtein(dnaSequence, pos++);
			proteins[4][i] = getProteinReverse(dnaSequence, --r);
			proteins[2][i] = getProtein(dnaSequence, pos++);
			proteins[5][i] = getProteinReverse(dnaSequence, --r);
			++i;
		}
		if (r)
		{
			proteins[0][i] = getProtein(dnaSequence, pos++);
			proteins[3][i] = getProteinReverse(dnaSequence, --r);
		}
		if (r)
		{
			proteins[1][i] = getProtein(dnaSequence, pos);
			proteins[4][i] = getProteinReverse(dnaSequence, r);
		}
		return n;
	}

	static Protein const *nextChar(Protein const *p, Protein const *end)
	{
		while (*(p) != stop && p < end)
			++p;
		return p;
	}

	static void maskRuns(vector<Protein> &query, unsigned run_len)
	{
		Protein *last = &query[0] - 1, *i = &query[0], *end = &query.back();
		while (i <= end)
		{
			if (*i == stop)
			{
				if (last != 0 && i - last - 1 < run_len)
				{
					for (Protein *j = last + 1; j < i; ++j)
						*j = 23;
				}
				last = i;
			}
			++i;
		}
		if (last != 0 && i - last - 1 < run_len)
		{
			for (Protein *j = last + 1; j < i; ++j)
				*j = 23;
		}
	}

	static unsigned getFrames(vector<Protein> const *queries, unsigned runLen)
	{
		unsigned set = 0;

		for (unsigned i = 0; i < 6; ++i)
		{
			if (queries[i].size() > 0)
			{
				unsigned run = 0;
				Protein const *p = &(queries[i][0]);
				Protein const *q;
				Protein const *end = p + queries[i].size();
				while ((q = nextChar(p, end)) < end)
				{
					run = q - p;
					if (run >= runLen)
						set |= 1 << i;
					p = q + 1;
				}
				run = q - p;
				if (run >= runLen)
					set |= 1 << i;
			}
		}
		return set;
	}

	static void maskRuns(vector<Protein> *queries, unsigned run_len)
	{
		for (unsigned i = 0; i < 6; ++i)
			maskRuns(queries[i], run_len);
	}
};

const Protein Translator::stop(ValueFeature<Protein>::from_char_('*'));
const Nucleotide Translator::reverse_nucleotide[5] = { 3, 2, 1, 0, 4 };

const Protein Translator::find[5][5][5] = {
	{
		{11, 2, 11, 2, 23},
		{16, 16, 16, 16, 16},
		{1, 15, 1, 15, 23},
		{9, 9, 12, 9, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{5, 8, 5, 8, 23},
		{14, 14, 14, 14, 14},
		{1, 1, 1, 1, 1},
		{10, 10, 10, 10, 10},
		{23, 23, 23, 23, 23},
	},
	{
		{6, 3, 6, 3, 23},
		{0, 0, 0, 0, 0},
		{7, 7, 7, 7, 7},
		{19, 19, 19, 19, 19},
		{23, 23, 23, 23, 23},
	},
	{
		{23, 18, 23, 18, 23},
		{15, 15, 15, 15, 15},
		{23, 4, 17, 4, 23},
		{10, 13, 10, 13, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
	}};

    const Protein Translator::find_reverse[5][5][5] = 
    {
	{
		{13, 10, 13, 10, 23},
		{4, 17, 4, 23, 23},
		{15, 15, 15, 15, 15},
		{18, 23, 18, 23, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{19, 19, 19, 19, 19},
		{7, 7, 7, 7, 7},
		{0, 0, 0, 0, 0},
		{3, 6, 3, 6, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{10, 10, 10, 10, 10},
		{1, 1, 1, 1, 1},
		{14, 14, 14, 14, 14},
		{8, 5, 8, 5, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{9, 12, 9, 9, 23},
		{15, 1, 15, 1, 23},
		{16, 16, 16, 16, 16},
		{2, 11, 2, 11, 23},
		{23, 23, 23, 23, 23},
	},
	{
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
		{23, 23, 23, 23, 23},
	}
   
};


#endif // __TRANSLATOR_H__