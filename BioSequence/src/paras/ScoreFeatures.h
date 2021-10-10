#ifndef __SCOREFEATURES_H__
#define __SCOREFEATURES_H__

#include "Value.h"
#include "../blastalgo/blast/core/blast_encoding.h"

template<typename _val>
uint8_t getBlastSeqCode()
{ 
    return 0; 
}

template<>
uint8_t getBlastSeqCode<Protein>()
{ 
    return BLASTAA_SEQ_CODE; 
}

template<typename _val>
int16_t getBlastLoadKarlinBlk(Blast_KarlinBlk* kbp,
		Blast_KarlinBlk* kbp_ungap,
		int gap_open,
		int gap_extend,
		int reward,
		int penalty,
		const char *matrix)
{ 
    return 0; 
}

template<>
int16_t getBlastLoadKarlinBlk<Protein>(Blast_KarlinBlk* kbp,
		Blast_KarlinBlk* kbp_ungap,
		int gap_open,
		int gap_extend,
		int reward,
		int penalty,
		const char *matrix)
{
	return Blast_KarlinBlkGappedLoadFromTables(kbp,
			gap_open,
			gap_extend,
			matrix);
}

template<typename _val>
const uint8_t* getBlastAlphabet()
{ 
    return 0; 
}

template<>
const uint8_t* getBlastAlphabet<Protein>()
{ 
    return AMINOACID_TO_NCBISTDAA;
};



#endif // __SCOREFEATURES_H__