# Suffix Array
## Parameters
```c++
int num_seqs_;//number of sequence
double db_size_MB_; //size of database
char** header_; //header
BioAlphabet alphabet_; 
char** sequence_;// sequence
int* seq_len_;//length of sequence
GSA* suffix_array_;//Generalized suffix array
std::vector<SFAIDXTYPE> key_array_;//suffix array index type
bool is_header_loaded_, is_sequence_loaded_, is_sfa_built_, is_k_array_built_;
bool is_size_counted_;
bool is_multi_;
int num_blocks_; //number of blocks
std::vector<RIDTYPE> block_size_; //size of read IDs
bool is_contained_init_;
bool is_alphabet_set_;
std::vector<RIDTYPE> contained_by_;
std::vector<bool> is_contained_;
## Methods
```c++
/** 
* Constructor
* @param alpha BioAlphabet DNA, Protein, RNA
* @param seq_file string sequence file name
* @param block_file string block file name
*/
SFABuild(BioAlphabet& alpha, std::string& seq_file);
SFABuild(BioAlphabet& alpha, std::string& seq_file, std::string& block_file);

/**clear suffix array and key array to save memory*/
void DestructSFA(void);
void DestructSequences(void);

/** 
* Loading the sequences from sequence file
* @param seq_file string sequence file name
* @return void
*/
void LoadSequences(std::string& seq_file);
/** 
* Loading the sequences from block file
* @param seq_file string block file name
* @return void
*/
void LoadBlockSize(std::string& block_file);

/** 
* Copying the number of the sequences from source to target
* @param num_sequences the number of the sequences
* @param source the number of the sequences
* @param target the number of the sequences
* @return void
*/
void CopySequences(int num_sequences, char **source, char **target);

/** Reverse the sequences in array sequence_*/
void InPlaceReverse(void);

/** Building the suffix array on the entire set of sequences (default)*/
void BuildSFADefault(void);

/** Construct multi block-SAs
*@param max_size max size of SA 
*@param dir Dump SFA to the 'dir' path
*@param file_stem file path
*note: The block information into disk
*/
void BuildSFAMulti(const SFAIDXTYPE& max_size, std::string &dir, std::string &file_stem);

/** Merge the two suffix arrays
*@param dir Dump SFA to the 'dir' path
*@param file_stem file path
*/
void MergeSFAMulti(std::string &dir, std::string &file_stem);
/** Merge the two suffix arrays to out_file_sfa
*@param file_sfa_1 SFA file path 1
*@param file_sfa_2 SFA file path 2
*@param out_file_sfa target file path
*/
void MergeTwoSFA(std::string &file_sfa_1, std::string &file_sfa_2, std::string& out_file_sfa);

/** Building the default key arrayy*/
void BuildKeyArrayDefault(void);

/** Write suffix array index*/
void DumpSFA(std::string& dir, std::string& file_stem, const RIDTYPE& pivot);

/** Suffix array indexing file from the path made by file_stem and dir*/
void LoadSFA(std::string& dir, std::string& file_stem);

/** Get database size*/
void CountDBSize(void);
double GetDBSizeInMegaBase(void);

/**Accessing the contents in the object*/
void PrintAllSeqs(void);
void PrintAllSuffixes(void);

/** Search the suffix array within the object*/
std::pair<SFAIDXTYPE, SFAIDXTYPE> SearchSFA(std::string& search_seed);

/** Find a list of maximal extension reads within the given range
*@param range pair<SFAIDXTYPE, SFAIDXTYPE> SFA range
*@param pos_list list postion
*@return pair<SFAIDXTYPE, SFAIDXTYPE> 
*/
void GetMaxExtInfoWithinRange(
        std::pair<SFAIDXTYPE, SFAIDXTYPE>& range, 
        std::list<GSATYPE>& pos_list);

/** Access a given sequence by index and block id*/
std::string GetSequence(const RIDTYPE& index);
std::string GetSequence(const int& block_ID, const RIDTYPE& index);
std::string GetHeader(int index);

/** Access a given suffix from the suffix array*/
std::string GetSuffixSFA(int index);

/** Get Suffix Sequence
*@param s the location of the sequence
*@param l the length of the sequence
*@return string
*/ 
std::string GetSuffixSeq(
        const GSATYPE& s,   
        const int& l        );

/** Get Suffix Sequence
*@param s the location of the sequence
*@return string
*/ 
std::string GetSuffixSeq(
        const GSATYPE& s   
    );

/** Get Suffix Sequence
*@param block_ID the ID of the block
*@param l the length of the sequence
*@param s the location of the sequence
*@return string
*/ 
std::string GetSuffixSeq(
        const int& block_ID,  
        const GSATYPE& s,    
        const int& l      );

/** Get Suffix Sequence
*@param block_ID the ID of the block
*@param s the location of the sequence
*@return string
*/ 
std::string GetSuffixSeq(
        const int& block_ID,  
        const GSATYPE& s  );
bool CheckMultiParam(const SFAIDXTYPE& max_size);

/** Access the number of index blocks*/
int GetNumBlocks(void);

int GetSeqLen(const RIDTYPE& r);
/** Get the length of the suffix
*@param block_ID the ID of the block
*@param s the location of the sequence   
*/
int GetSufLen(const int& block_ID, const GSATYPE& s); 
int GetSufLen(const GSATYPE& s);

/** Get full read ID
*@param block_ID the ID of the block
*@param r the location of reads 
*/
RIDTYPE GetFullRID(const int& block_ID, const RIDTYPE& r);
void InitContainedInfo() ;

/** Get the number of sequences*/
RIDTYPE GetNumSeqs() ;
void PrintContainedInfo();  

```
## Class
```c++
class SFABuild {
        public:
        SFABuild(void);
        SFABuild(BioAlphabet& alpha, std::string& seq_file);
        SFABuild(BioAlphabet& alpha, std::string& seq_file, std::string& block_file);
        ~SFABuild(void);
        void DestructSFA(void);
        void DestructSequences(void);
        void LoadSequences(std::string& seq_file);
        void LoadBlockSize(std::string& block_file);
        void CopySequences(int num_sequences, char **source, char **target);
        void InPlaceReverse(void);
        void BuildSFADefault(void);
        void BuildSFAMulti(const SFAIDXTYPE& max_size, std::string &dir, std::string &file_stem);
        void MergeSFAMulti(std::string &dir, std::string &file_stem);
        void MergeTwoSFA(std::string &file_sfa_1, std::string &file_sfa_2, std::string& out_file_sfa);
        void BuildKeyArrayDefault(void);
        void DumpSFA(std::string& dir, std::string& file_stem, const RIDTYPE& pivot);
        void LoadSFA(std::string& dir, std::string& file_stem);
        void CountDBSize(void);
        double GetDBSizeInMegaBase(void);
        void PrintAllSeqs(void);
        void PrintAllSuffixes(void);
        std::pair<SFAIDXTYPE, SFAIDXTYPE> SearchSFA(std::string& search_seed);
        void GetMaxExtInfoWithinRange(
            std::pair<SFAIDXTYPE, SFAIDXTYPE>& range, 
            std::list<GSATYPE>& pos_list
        );
        std::string GetSequence(const RIDTYPE& index);
        std::string GetSequence(const int& block_ID, const RIDTYPE& index);

        std::string GetHeader(int index);
        std::string GetSuffixSFA(int index);
        
        std::string GetSuffixSeq(
            const GSATYPE& s,   // the location of the sequence needs to be copied
            const int& l        // the length of the sequence needs to be copied
        );

        std::string GetSuffixSeq(
            const GSATYPE& s    // the location of the sequence needs to be copied
        );

        std::string GetSuffixSeq(
            const int& block_ID,  // the ID of the block if multiple SFA was generated
            const GSATYPE& s,     // the location of the sequence needs to be copied
            const int& l          // the length of the sequence needs to be copied
        );

        std::string GetSuffixSeq(
            const int& block_ID,  // the ID of the block if multiple SFA was generated
            const GSATYPE& s      // the location of the sequence needs to be copied
        );

        bool CheckMultiParam(const SFAIDXTYPE& max_size);

        int GetNumBlocks(void); // access the number of index blocks

        int GetSeqLen(const RIDTYPE& r);

        // returns the length of the suffix
        int GetSufLen(const int& block_ID, const GSATYPE& s); 
        int GetSufLen(const GSATYPE& s);

        RIDTYPE GetFullRID(const int& block_ID, const RIDTYPE& r);

        void InitContainedInfo() ;

        RIDTYPE GetNumSeqs() ;

        void PrintContainedInfo();
        
        friend class DatabaseIndex;
        friend class ReachableReads;
        friend class ReadAlignment;
        friend class GreedyAssembly;
        friend class AssembleExtend;
        friend class ReMap;
        friend class Unitiger;
        friend class Clump;
        friend class Overlap;
        friend class Unitig;
        friend class Seeding;
        friend class SpliceAlign;

        protected:
        // data begin
        int num_seqs_;
        double db_size_MB_;
        char** header_;
        BioAlphabet alphabet_;
        char** sequence_;
        int* seq_len_;
        GSA* suffix_array_;
        std::vector<SFAIDXTYPE> key_array_;
        bool is_header_loaded_, is_sequence_loaded_, is_sfa_built_, is_k_array_built_;
        bool is_size_counted_;
        bool is_multi_;
        int num_blocks_;
        std::vector<RIDTYPE> block_size_; 
        bool is_contained_init_;
        bool is_alphabet_set_;
        std::vector<RIDTYPE> contained_by_;
        std::vector<bool> is_contained_;
        // methods begin
        void BuildSuffixArray(char** sequences, GSA* suffix_array);
        void BuildKeyArray(GSA* suffix_array, std::vector<SFAIDXTYPE>& key_array);
};

```
