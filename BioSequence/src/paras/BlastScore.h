#ifndef __BLASTSCORE_H__
#define __BLASTSCORE_H__

#include "../blastalgo/blast/core/blast_stat.h"
#include "../blastalgo/blast/core/blast_encoding.h"
#include "ScoreFeatures.h"
using std::auto_ptr;
class BlastScore
{
    public:
	template<typename _val>
	BlastScore(const string &matrix, int gap_open, int gap_extend, int reward, int penalty, const _val&):
		data_ (BlastScoreBlkNew(getBlastSeqCode<_val>(), 1))
	{
		if (data_ == 0)
			cout << "parameter data" << endl;
		if ((data_->kbp_gap_std[0] = Blast_KarlinBlkNew()) == 0 || (data_->kbp_std[0] = Blast_KarlinBlkNew()) == 0)
			cout << "parameter exception" << endl;
		if (getBlastLoadKarlinBlk<_val>(data_->kbp_gap_std[0],
										data_->kbp_std[0],
										gap_open,
										gap_extend,
										reward,
										penalty,
										matrix.c_str()) != 0)
			cout << "parameter BlastLoadKarlinBlk" << endl;
		data_->name = const_cast<char *>(matrix.c_str());
		data_->reward = reward;
		data_->penalty = penalty;
		if(Blast_ScoreBlkMatrixFill (data_, 0) != 0)
			cout << "Blast_ScoreBlkMatrixFill exception" << endl;
		data_->name = 0;
	}

	~BlastScore()
	{ 
        BlastScoreBlkFree(data_); 
    }

	template<typename _val>
	int getScore(_val x, _val y) const
	{ 
		return data_->matrix->data[(long)getBlastAlphabet<_val>()[(long)ValueFeature<_val>::alpha[x]]][(long)getBlastAlphabet<_val>()[(long)ValueFeature<_val>::alpha[y]]]; }

	double getLambda() const
	{ 
		return data_->kbp_gap_std[0]->Lambda; 
	}

	double getK() const
	{ 
		return data_->kbp_gap_std[0]->K; 
	}

	double getLnK() const
	{
		if(data_==nullptr)
		{
			cout << "data is null" << endl;
		}
		return data_->kbp_gap_std[0]->logK;
	}

	int getLowScore() const
	{ 
		return data_->loscore; 
	}

	private:

		BlastScoreBlk *data_;

};
const double LN_2 = 0.69314718055994530941723212145818;

#endif // __BLASTSCORE_H__