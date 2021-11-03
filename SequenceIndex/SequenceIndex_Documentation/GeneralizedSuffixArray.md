# Generalized Suffix Array
## Parameters
```c++
/**
 * \brief Generalized suffix array type definition
 */
struct GSATYPE
{
	IDType doc; ///< document ID
	IDType pos; ///< position
	GSATYPE(){}
	GSATYPE(IDType d, IDType p)
	{
		doc = d; pos = p;
	}
};

```
## Methods
```c++
/** Default constructor */
	GSA();

	/**
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n Integer a number of sequences
	 */
	GSA( char **s, IDType n );

	/**
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n a number of sequences
	 * \param f create LCPs?
	 */
	GSA( char **s, IDType n, bool f);


	/** Default destructor */
	~GSA();

	/** Clear all objects */
	void clear();

  /** Dump all GSA to dir **/
	void DumpAllBlock(const char *idx_file, const IDType &pivot);

```
## Class
```c++
class GSA : public SFA
{
 private:
	IDType *Ids; ///< Array of read IDs
	IDType *Pos; ///< Array of positions in concatenated string

//=========================
// Private member functions
//=========================
 private:

	/**
	 * Initialize GSA
	 * \param Strings a set of sequence reads
	 * \param Integer a number of reads
	 */
	void init(char **s, IDType n);

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
	 * Read ID and position search from concatenated string.
	 * It is fast but requires more RAMs
	 * because of two additional arrays to keep track of reads and IDs.
	 */
	void __convertWithArrays();

	/** Build LCP */
	void buildLCPs();

	/** Build LCP and internal LCP 	*/
	void buildLCPs(const char*, const char*);


//=========================
// Public member functions
//=========================
 public:
	/** Default constructor */
	GSA();

	/**
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n Integer a number of sequences
	 */
	GSA( char **s, IDType n );

	/**
	 * Constructor
	 * \param s a set of sequence reads
	 * \param n a number of sequences
	 * \param f create LCPs?
	 */
	GSA( char **s, IDType n, bool f);


	/** Default destructor */
	~GSA();

	/** Clear all objects */
	void clear();

	void DumpAllBlock(const char *idx_file, const IDType &pivot);

	UtilFunc util;

};

```
