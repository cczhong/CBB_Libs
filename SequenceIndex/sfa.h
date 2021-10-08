#ifndef __SUFFIX_ARRAY_H__
#define __SUFFIX_ARRAY_H__

#include <stack>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>
#include <sstream>
#include <limits>


#include "interval_array.h"
#include "divsufsort.h"
#include "seq_index.h"

typedef sauchar_t SFACHARTYPE;
typedef saidx_t SFAIDXTYPE;


typedef std::pair<SFAIDXTYPE, SFAIDXTYPE> BOUNDTYPE;

const char DELIMITER = '$';

class SFA {
 protected:
	SFACHARTYPE *concat;  	///< concatenated string
	SFAIDXTYPE *SA;      	///< suffix array
	SeqIdxType *LCP;     		///< longest common prefix array
	SeqIdxType *mLCP;    		///< internal LCPs
	SFAIDXTYPE size;     	///< size of suffix array
	char **seqs;   			///< sequence reads
	int nreads;   			///< # of reads
	//IntervalArray *ia;    ///< interval array
	IntervalArray ia;
	bool verbose;  			///< verbosity

//=========================
// private member functions
//=========================
 private:
	/** initialization */
	void init();

	/**
	 * Determine the size of the suffix array.
	 * Raise an error and terminate the program if the size is too big to load.
	 */
	void setStorageSize();

	/** Concatenate all sequence reads */
	void concatenateAllSeqs();

	/** Clear all */
	void clear();

//========================
// Public member functions
//========================
 public:
	/** Empty default constructor */
	SFA();

	/**
	 * Constructor
	 * \param seqs sequences
	 * \param nseq number of sequences
	 */
	SFA(char **seqs, int nseq);

	/** Empty destructor */
	~SFA();

	//========
	// Getters
	//========
	/** Get suffix array */
	SFAIDXTYPE*  getSFA() { return SA; }

	/** Get concatenated string */
	SFACHARTYPE*  getDoc() { return concat; }

	/** Get the size of suffix array */
	SFAIDXTYPE   getSize() { return size; }

	/** Get LCP value at position i */
	SeqIdxType getLcpAt(SFAIDXTYPE i) { return LCP[i]; }

	/** Get internal LCP value at position i */
	SeqIdxType getMcpAt(SFAIDXTYPE i) { return mLCP[i]; }


	//========
	// Setters
	//========
	/** Set a set of sequences */
	void setSequences(char **s) { seqs    = s; }

	/** Set read count */
	void setReadCount(int    n) { nreads  = n; }

	/** Set verbose option */
	void setVerbosity(bool   v) { verbose = v; }


	//=========
	// Builders
	//=========
	/** Construct SFA */
	void buildSFA();

	/** Construct LCP */
	void buildLCP();

	/** Construct internal LCP to make fast query */
	void buildMLCP();


  /** Delete suffix array */
	void purgeSA();

	/** Delete concatenated string */
	void purgeDoc();

	/** Delete LCP */
	void purgeLCP();

	/** Delete internal LCP */
	void purgeMLCP();


};


#endif
