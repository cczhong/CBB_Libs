# Sequence Index
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
```
## Methods
```c++
/** 
* Constructor
* @param alpha BioAlphabet DNA, Protein, RNA
* @param seq_file string sequence file name
* @param block_file string block file name
*/
  SequenceIndex(void);
  SequenceIndex(BioAlphabet& alpha);
  
 /** Destructor **/
  ~SequenceIndex(void);

/** 
* Loading the sequences from sequence file
* @param seq_file string sequence file name
* @return void
*/
  void LoadSequences(std::string& seq_file, const bool &rev_comp);

/** 
* Setting the block size 
* @param num_blocks number of blocks to allote
* @param dir Dump block SFA to dir path
* @param file_stem file path 
*/  
  void SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem);
  
  /**
  * Splitting the input query sequences based on number of blocks
  * @param block_sfa vector to store the sequences
  */
  void SplitSequence(std::vector<SequenceIndex>& block_sfa);
  
  /** Building the suffix array on the entire set of sequences (default)*/
  void BuildSFADefault(void);
  
  /**
  * Writing the built SFA to a directory
  * @param dir dir path to dump SFA
  * @param file_stem file path
  */
  void DumpSFA(std::string& dir, std::string& file_stem, const IDType& pivot);



```
## Class
```c++
class SequenceIndex {
 public:
  SequenceIndex(void);
  SequenceIndex(BioAlphabet& alpha);
  ~SequenceIndex(void);

  void LoadSequences(std::string& seq_file, const bool &rev_comp);
  void SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem);
  void SplitSequence(std::vector<SequenceIndex>& block_sfa);
  void BuildSFADefault(void);
  void DumpSFA(std::string& dir, std::string& file_stem, const IDType& pivot);


 protected:
  int num_seqs_;
  char** header_;
  BioAlphabet alphabet_;
  char** sequence_;
  SeqIdxType* seq_len_;
  GSA* suffix_array_;
  bool is_header_loaded_, is_sequence_loaded_, is_block_loaded_;
  bool is_sfa_built_, is_k_array_built_;
  bool is_size_counted_;
  bool is_multi_;
  bool is_rev_comp_;
  int num_blocks_;
  std::vector<IDType> block_size_;
  bool is_alphabet_set_;

};

```
