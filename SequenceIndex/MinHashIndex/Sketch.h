//  Author: Sirisha Thippabhotla
//  Last modified: 09/12/22
#ifndef _Sketch_H 
#define _Sketch_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <cassert>
#include <zlib.h>  

#include "../UtilFunctions.h"
#include "../../CommonDef/DataType.h"


//minimizer index
struct MH_INDEX_SEQ{
    char *name;
    DBIdxType offset;
    IDType len;

};

struct MH_INDEX{
    IDType b, w, k, flag;
    IDType num_seqs;
    IDType index;
    MH_INDEX_SEQ *seq;
};

// indexing and mapping options
struct MH_INDEX_OPTS {
	short k, w, flag, bucket_bits;
	int64_t mh_batch_size;
	DBIdxType batch_size;
};


struct MH_INDEX_READER{
    int is_index, num_blocks;
    DBIdxType index_size;
    MH_INDEX_OPTS opt;
    FILE *fp_out;
};

class Sketch{


private:
/**
 * Initialize an index reader
 *
 * @param fn         index or fasta/fastq file name (this function tests the file type)
 * @param opt        indexing parameters
 * @param fn_out     if not NULL, write built index to this file
 *
 * @return an index reader on success; NULL if fail to open _fn_
 */
MH_INDEX_READER *mh_reader_open(const char *fn, const MH_INDEX_OPTS *opt, const char *fn_out);


/**
 * Build a minimizer index
 *
 * This function constructs the index for about MH_INDEX_OPTS::batch_size bases.
 * Importantly, for a huge collection of sequences, this function may only
 * return an index for part of sequences. It needs to be repeatedly called
 * to traverse the entire index/sequence file.
 *
 * @param r          index reader
 * @param n_threads  number of threads for constructing index
 *
 * @return an index on success; NULL if reaching the end of the input file
 */
MH_INDEX *MH_INDEX_READ(MH_INDEX_READER *r, int n_threads);


/**
 * Create an index from strings in memory
 *
 * @param w            minimizer window size
 * @param k            minimizer k-mer size
 * @param bucket_bits  number of bits for the first level of the hash table
 * @param n            number of sequences
 * @param seq          sequences in A/C/G/T
 * @param name         sequence names; could be NULL
 *
 * @return mininizer index
 */
MH_INDEX *MH_INDEX_STR(int w, int k, int bucket_bits, int n, const char **seq, const char **name);


public:

/** Default constructor */
Sketch();

/**
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n Integer a number of sequences
	 */
	Sketch( char **s, IDType n );

/** Default destructor */
~Sketch();

/** Clear all objects */
	void clear();

void DumpSketch(const char *idx_file, const MH_INDEX *idx);

UtilFunc util;

};
#endif

