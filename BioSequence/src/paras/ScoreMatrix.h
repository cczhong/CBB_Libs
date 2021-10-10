#ifndef __SCOREMATRIX_H__
#define __SCOREMATRIX_H__
#include <iostream>
#include <memory>
#include "BlastScore.h"


using namespace std;
using std::auto_ptr;
class ScoreMatrix
{
    public:
	template<typename _val>
	ScoreMatrix(const string &matrix, int gap_open, int gap_extend, int reward, int penalty, const _val&):
		sb_ (matrix, gap_open, gap_extend, reward, penalty, _val ()),
		bias_ ((char)(-sb_.getLowScore())),
		name_ (matrix),
		matrix8_ (_val(), sb_),
		matrix8u_ (_val(), sb_, bias_),
		matrix16_ (_val(), sb_)
	{ 

    }

	template<typename _val>
	void print() const
	{
		cout << "Scoring matrix = " << name_ << endl;
		cout << "Lambda = " << sb_.getLambda() << endl;
		cout << "K = " << sb_.getK() << endl;
		const unsigned n = ValueFeature<_val>::ALPHABET_SIZE;
		for(unsigned i=0;i<n;++i) {
			for(unsigned j=0;j<n;++j)
				printf("%3i", (int)matrix8_.data[i*32+j]);
			printf("\n");
		}
	}

	static const ScoreMatrix& get()
	{ 
        return *instance; 
    }

	const int8_t* matrix8() const
	{ 
        return matrix8_.data; 
    }

	const uint8_t* matrix8u() const
	{ 
        return matrix8u_.data; 
    }

	const int16_t* matrix16() const
	{ 
        return matrix16_.data; 
    }

	template<typename _val>
	int letter_score(_val a, _val b) const
	{ 
        return matrix8_.data[(int(a) << 5) + int(b)]; 
    }

	template<typename _val>
	uint8_t biased_score(_val a, _val b) const
	{ 
        return matrix8u_.data[(int(a) << 5) + int(b)]; 
    }

	char bias() const
	{ 
        return bias_; 
    }

	double getBitscore(int raw_score) const
	{ 
        return ( sb_.getLambda() * raw_score - sb_.getLnK()) / LN_2;
    }

	double getRawscore(double bitscore, double) const
	{ 
		cout << "double getRawscore: bitscore = " <<bitscore<<endl ;
		cout << "LN_2 = " <<LN_2<<endl ;
		cout << "double getRawscore: sb_.getLnK() = " <<sb_.getLambda()<<endl ;
        return (bitscore*LN_2 + sb_.getLnK()) / sb_.getLambda(); 
		cout << "after double getRawscore: bitscore = " <<bitscore<<endl ;

    }

	int getRawscore(double bitscore) const
	{
		//cout << "getRawscore: bitscore = " <<bitscore<<endl ;
		return (int)ceil(getRawscore(bitscore, double()));
		//cout << "(int)ceil(getRawscore(bitscore, double()))= " <<(int)ceil(getRawscore(bitscore, double()))<<endl ;

	}

	double getEvalue(int raw_score, size_t db_letters, unsigned query_len) const
	{ 
		//cout << "raw_score = " <<raw_score<<endl ;
        return static_cast<double>(db_letters) * query_len * pow(2,-getBitscore(raw_score)); 
    }

	double getBitscore(double evalue, size_t db_letters, unsigned query_len) const
	{ 
        return -log(evalue/db_letters/query_len)/log(2);
    }

	double getK() const
	{ 
        return sb_.getK(); 
    }

	double getLambda() const
	{ 
        return sb_.getLambda(); 
    }

	static auto_ptr<ScoreMatrix> instance;

private:

	template<typename _t>
	struct Scores
	{
		template<typename _val>
		Scores(const _val&, const BlastScore &sb, char bias = 0)
		{
			const unsigned n = ValueFeature<_val>::ALPHABET_SIZE;
			for(unsigned i=0;i<32;++i)
				for(unsigned j=0;j<32;++j)
					data[i*32+j] = i < n && j < n ? (_t)(sb.getScore((_val)i, (_val)j) + (int)bias) : std::numeric_limits<_t>::min();
		}
		_t data[32*32] __attribute__ ((aligned (16)));
	};

	const BlastScore sb_;
	const char bias_;
	const string name_;
	const Scores<int8_t> matrix8_;
	const Scores<uint8_t> matrix8u_;
	const Scores<int16_t> matrix16_;

};
auto_ptr<ScoreMatrix> ScoreMatrix::instance;


#endif // __SCOREMATRIX_H__