# BioSequence
BioSequence
## Methods
```c++
/**
 * Create SFA
 * @param seq_type including "DNA", "Protein" and "RNA"
 * @param db_file sequence file path
 * @param sfab SFABuild get SFABuild object
 * @return void
 */
void createSFA(string seq_type,string db_file, SFABuild& sfab);

/**
 * Guess input file format
 * @param seq_file file path 
 * @return int 1 = Fasta format; 2 = Fastq format; 0 = invalid format 
 */
int guessFormat(std::string& seq_file);

/**
 * Check input file format valid
 * @param seq_file file path 
 * @return bool true = a valid format; false = Non-valid format file for input
 */
bool isValidFormat(std::string& seq_file); 
  
/**
 * Get number of sequences in sequence file
 * @param seq_file file path 
 * @return int number
 */
int getNumSeqs(std::string& seq_file);  

/**
 * Get BioSequence Instance
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return BioAlphabet  object
 */
BioAlphabet& getBioAlphabetInstance(string seq_type);

/**
 * Write the 'T' type data with 's' size to 'filename' path
 * @param t T*          the object you write
 * @param st SizeType   the size of the object you write
 * @param filename const char       the path you write to
 * @return bool 
 */
template<typename T, typename SizeType>    
bool writeData(T* t, SizeType& st, const char *filename);

/**
 * Read the 'T' type data from 'filename' path
 * @param t T*          the object you write
 * @param filename const char       the path you write to
 * @return size
 */
template<typename T, typename SizeType>    
SizeType readData(T* t, const char *filename);

/**
 * Check letters if valid
 * @param c char Sequence letter
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return bool
 */
bool checkValid(char c, string seq_type);

```

## Class
```c++
class BioSequences
{

public:
    BioSequences(){}
    ~BioSequences(){};

/**
 * Create SFA
 * @param seq_type including "DNA", "Protein" and "RNA"
 * @param db_file sequence file path
 * @param sfab SFABuild get SFABuild object
 * @return void
 */
void createSFA(string seq_type,string db_file, SFABuild& sfab);

/**
 * Guess input file format
 * @param seq_file file path 
 * @return int 1 = Fasta format; 2 = Fastq format; 0 = invalid format 
 */
int guessFormat(std::string& seq_file) ;

/**
 * Check input file format valid
 * @param seq_file file path 
 * @return bool true = a valid format; false = Non-valid format file for input
 */
bool isValidFormat(std::string& seq_file); 
 
/**
 * Get number of sequences in sequence file
 * @param seq_file file path 
 * @return int number
 */
int getNumSeqs(std::string& seq_file) ; 

/**
 * Get BioSequence Instance
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return BioAlphabet  object
 */

BioAlphabet& getBioAlphabetInstance(string seq_type);

/**
 * Write the 'T' type data with 's' size to 'filename' path
 * @param t T*          the object you write
 * @param st SizeType   the size of the object you write
 * @param filename const char       the path you write to
 * @return bool 
 */
    template<typename T, typename SizeType>    
bool writeData(T* t, SizeType& st, const char *filename);
/**
 * Read the 'T' type data from 'filename' path
 * @param t T*          the object you write
 * @param filename const char       the path you write to
 * @return size
 */
template<typename T, typename SizeType>    
SizeType readData(T* t, const char *filename);

/**
 * Check letters if valid
 * @param c char Sequence letter
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return bool
 */
bool checkValid(char c, string seq_type);


    private:
        BioAlphabet alphabet_DNA;
        BioAlphabet alphabet_PROT;
        BioAlphabet alphabet_RNA;
        BioAlphabet alphabet_OTHER;
};


```