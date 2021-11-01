# GSA
Generalized suffix array
## Parameters
```c++
typedef uint32_t RIDTYPE;
typedef LCPTYPE POSTYPE;
/** 
 * @brief Generalized suffix array type definition
 */
struct GSATYPE
{
	RIDTYPE doc; ///< document ID
	POSTYPE pos; ///< position
	GSATYPE(){}
	GSATYPE(RIDTYPE d, POSTYPE p) 
	{
		doc = d; pos = p;
	}
};

RIDTYPE *Ids; // Array of read IDs
POSTYPE *Pos; // Array of positions in concatenated string

```
## Methods

```c++
	/** Default constructor */
	GSA();

	/** 
	 * Constructor
	 * @param s char** a set of sequence reads
	 * @param n RIDTYPE(Integer) a number of sequences
	 */
	GSA( char **s, RIDTYPE n );

	/** 
	 * Constructor
	 * @param s char ** a set of sequence reads
	 * @param n RIDTYPE a number of sequences
	 * @param f bool whether creating LCPs
	 */
	GSA( char **s, RIDTYPE n, bool f);

	/** 
	 * Constructor
	 * @param seqs char ** a set of sequence reads
	 * @param nseq RIDTYPE a number of sequences
	 * @param flag bool whether create LCPs
	 * @param gsa_file char * GSA output file name
	 * @param lcp_file char * LCP output file name
	 * @param mcp_file char * internal LCP output file name
	 */
	GSA( char **seqs, 
		 RIDTYPE    nseq, 
		 bool   flag, 
		 const char *gsa_file, 
		 const char *lcp_file, 
		 const char *mcp_file);

	/** Default destructor */
	~GSA();

	/** Clear all objects */
	void clear();

	/** 
	 * Get a suffix of read starting from given position 
	 * @param SFAIDXTYPE a given position
	 * @return char*
	*/ 
	char*    getSuffix(SFAIDXTYPE);

	/** 
	 * Get a suffix of entire string from given position
	 * @param SFAIDXTYPE a given position
	 * @return char*
	 */ 
	char*    getEntireSuffix(SFAIDXTYPE);

	/** 
	 * Get a record at given position
	 * @param SFAIDXTYPE a given position
	 * @return char*
	 */ 
	GSATYPE  getAt(SFAIDXTYPE);

	/** 
	 * Get a number of reads
	 * @param SFAIDXTYPE a given position
	 * @return RIDTYPE
	 */ 
	RIDTYPE  getReadCount();

	/** 
	 * Set sequence reads
	 * @param char ** sequence reads
	 * @return void
	 */ 
	void setSequences( char **s );

	/** 
	 * Set read count
	 * @param RIDTYPE n
	 * @return void
	 */ 
	void setReadCount( RIDTYPE n );

	/** Set verbosity */
	void setVerbosity( bool v);

	/** Print suffix array */
	void printSFA();

	/** Print suffix */
	void printSuffix();

	/**
	 * Search a matching range of a pattern.
	 * @param srch SFACHARTYPE* pattern to search
	 * @param len POSTYPE a length of the pattern
	 * @return BOUNDTYPE
	 */
	BOUNDTYPE search( const SFACHARTYPE *srch, POSTYPE len );

	/** 
	 * Load data from index file
	 * @param char* idx_file
	 * @return void
	 */ 
	void load(const char* idx_file);

	/**
	 * Load binary objects
	 * @param lcp_file LCP file
	 * @param mcp_file internal LCP file
	 * @param gsa_file GSA file
	 * @return void
	 */
	void load( const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);

	/**
	 * Load binary objects
	 * @param sfa_file SFA file
	 * @param con_file concatenated string file
	 * @param lcp_file LCP file
	 * @param mcp_file internal LCP file
	 * @param gsa_file GSA file
	 * @return void
	 */
	void load( const char* sfa_file, 
			   const char* con_file, 
			   const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);

	/**
	 * Dump binary objects
	 * @param sfa_file SFA file
	 * @param con_file concatenated string file
	 * @param lcp_file LCP file
	 * @param mcp_file internal LCP file
	 * @param gsa_file GSA file
	 * @return void
	 */
	void dump( const char* sfa_file, 
			   const char* con_file, 
			   const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);
	/** 
	 * Dump All Block into index file
	 * @param idx_file const char * index file name
	 * @param pivot RIDTYPE document ID
	 * @return void
	 */ 

	void DumpAllBlock(const char *idx_file, const RIDTYPE &pivot);


```

## Class
```c++
class GSA : public SFA
{
 private:
	RIDTYPE *Ids; ///< Array of read IDs
	POSTYPE *Pos; ///< Array of positions in concatenated string

//=========================
// Private member functions	
//=========================
 private:
  
  
	/**
	 * Initialize GSA
	 * \param Strings a set of sequence reads
	 * \param Integer a number of reads
	 */
	void init(char **s, RIDTYPE n);

	/** 
	 * Build GSA
	 * First, build SFA. 
	 * Then, find read ID and positions in concatenated string by
	 * calling __convertWithArrays().
	 */
	void buildGSA();

	/**
	 * Build GSA and write GSA to file. 
	 */
	void buildGSA(const char*);

	/** 
	 * Alternative read ID and position search using binary search.
	 * It is more memory efficient but slow.
	 */
	void __convertWithBinarySearch();

	/**
	 * Read ID and position search from concatenated string.
	 * It is fast but requires more RAMs 
	 * because of two additional arrays to keep track of reads and IDs.
	 */
	void __convertWithArrays();

	/** Build LCP */
	void buildLCPs();
	
	/** Build LCP and internal LCP 	*/
	void buildLCPs(const char*, const char*);


	/** Range search of given string without LCP arrays */
	BOUNDTYPE searchOnGSA( const SFACHARTYPE *srch, POSTYPE len );
	
	/** Get left boundary of query sequence match using LCP arrays */
	SFAIDXTYPE getLeftBoundWithLCPs(const SFACHARTYPE* pat, POSTYPE len);

	/** Get right boundary of query sequence match using LCP arrays */
	SFAIDXTYPE getRightBoundWithLCPs(const SFACHARTYPE* pat, POSTYPE len);

	/** Get left boundary of query sequence match without LCP arrays */
	SFAIDXTYPE getLeftBoundOnGSA(const SFACHARTYPE* pat, POSTYPE len);

	/** Get right boundary of query sequence match without LCP arrays */
	SFAIDXTYPE getRightBoundOnGSA(const SFACHARTYPE* pat, POSTYPE len);
	
	/** Get a sequence of given position */
	char* getSeq( SFAIDXTYPE p );

	/** Write generalized suffix array */
	void writeGSA( const char *filename );

	/** Read generalized suffix array from a file */
	void readGSA( const char *filename );

 public:
	/** Default constructor */
	GSA();

	/** 
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n Integer a number of sequences
	 */
	GSA( char **s, RIDTYPE n );

	/** 
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n a number of sequences
	 * \param f create LCPs?
	 */
	GSA( char **s, RIDTYPE n, bool f);

	/** 
	 * Constructor
	 * \param seqs a set of sequence reads
	 * \param nseq a number of sequences
	 * \param flag create LCPs?
	 * \param gsa_file GSA output file name
	 * \param lcp_file LCP output file name
	 * \param mcp_file internal LCP output file name
	 */
	GSA( char **seqs, 
		 RIDTYPE    nseq, 
		 bool   flag, 
		 const char *gsa_file, 
		 const char *lcp_file, 
		 const char *mcp_file);

	/** Default destructor */
	~GSA();

	/** Clear all objects */
	void clear();

	/** Get a suffix of read starting from given position */ 
	char*    getSuffix(SFAIDXTYPE);

	/** Get a suffix of entire string from given position */ 
	char*    getEntireSuffix(SFAIDXTYPE);

	/** Get a record at given position */
	GSATYPE  getAt(SFAIDXTYPE);

	/** Get a number of reads */
	RIDTYPE  getReadCount() { return nreads; }

	/** Set sequence reads */
	void setSequences( char **s ) { seqs   = s; }

	/** Set read count */
	void setReadCount( RIDTYPE n )    { nreads = n; }

	/** Set verbosity */
	void setVerbosity( bool v)    { verbose = v; }

	/* /\** */
	/*  * Build generalized suffix array */
	/*  * \param  */
	/*  *\/ */
	/* void buildSFA(bool); */

	/** Print suffix array */
	void printSFA();

	/** Print suffix */
	void printSuffix();
	
	/** 
	 * Search a matching range of a pattern.
	 * \param srch pattern to search
	 * \param len  a length of the pattern
	 */
	BOUNDTYPE search( const SFACHARTYPE *srch, POSTYPE len );


	void load(const char* idx_file);

	/**
	 * Load binary objects
	 * \param lcp_file LCP file
	 * \param mcp_file internal LCP file
	 * \param gsa_file GSA file
	 */
	void load( const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);

	/**
	 * Load binary objects
	 * \param sfa_file SFA file
	 * \param con_file concatenated string file
	 * \param lcp_file LCP file
	 * \param mcp_file internal LCP file
	 * \param gsa_file GSA file
	 */
	void load( const char* sfa_file, 
			   const char* con_file, 
			   const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);

	/**
	 * Dump binary objects
	 * \param sfa_file SFA file
	 * \param con_file concatenated string file
	 * \param lcp_file LCP file
	 * \param mcp_file internal LCP file
	 * \param gsa_file GSA file
	 */
	void dump( const char* sfa_file, 
			   const char* con_file, 
			   const char* lcp_file, 
			   const char* mcp_file, 
			   const char* gsa_file);
			   
  /********************Newly added or redefined************************************/
  /** Range search of given string using LCP arrays */
	BOUNDTYPE searchWithLCPs( const SFACHARTYPE *srch, POSTYPE len );
	char* getSuffix_explicit(RIDTYPE rid, POSTYPE pos);
	char* getSequence_explicit(RIDTYPE rid);
	POSTYPE  getSuffixLength(SFAIDXTYPE p);
	POSTYPE  getFullSequenceLength(SFAIDXTYPE p);

	RIDTYPE getId(SFAIDXTYPE p);
  POSTYPE getPos(SFAIDXTYPE p);	
  POSTYPE getLcpArbitrary(SFAIDXTYPE p, SFAIDXTYPE q);
  
  BOUNDTYPE searchWithLCPs_bounded(
      const SFACHARTYPE *srch, POSTYPE len, 
      SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
  );
	SFAIDXTYPE getLeftBoundWithLCPs_bounded(
	    const SFACHARTYPE* pat, POSTYPE len, 
	    SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
	);
	SFAIDXTYPE getRightBoundWithLCPs_bounded(
	    const SFACHARTYPE* pat, POSTYPE len, 
	    SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
	);
	
	POSTYPE getLcp(SFAIDXTYPE p);
	POSTYPE getSeqLength_RID(RIDTYPE rid);

	void DumpAllBlock(const char *idx_file, const RIDTYPE &pivot);

};

```