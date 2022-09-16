# KmerCount
## Parameters
```c++
typedef std::unordered_map <std::string, RIDTYPE> KmerMap; // Map to store kmer seq and it's corresponding frequency
int num_seqs_;//number of sequence
int kmer_sizel //kmer size
BioAlphabet alphabet_; 
char** sequence_;// sequence
int* seq_len_;//length of sequence
bool is_alphabet_set_;
```
## Methods
```c++
/** Constructor **/
  KmerCount(void);
  
 /** Destructor **/
  ~KmerCount(void);

/** 
* Loading the sequences from sequence file
* @param seq_file string sequence file name
* @return void
*/
  void LoadSequences(std::string& seq_file, const bool &rev_comp);

/** 
* @param kmer_size kmer size to compute
* @param file_stem file path 
*/  
  void ExtractKmersFromSeq(const int& kmer_size, std::string &file_stem, set<int> & kmers);
  
  
  /** Building the kmer frequeunct map on the entire set of sequences (default)*/
  void BuildKmerFrequencyMap(KmerMap & kmer_c, set<int> &kmers);
  
  /**
  * Writing the built frquency map to a directory
  * @param dir dir path to dump kmer map
  * @param file_stem file path
  */
  void DumpKmerFrequencyMap(std::string& dir, std::string& file_stem);
  



```
## Class
```c++
class KmerCount {
 public:
  KmerCount(void);
  ~KmerCount(void);

  void LoadSequences(std::string& seq_file, const bool &rev_comp);
  void ExtractKmersFromSeq(const int& kmer_size, std::string &dir, std::string &file_stem);
  void BuildKmerFrequencyMap(KmerMap & kmer_c);
  void DumpKmerFrequencyMap(std::string& dir, std::string& file_stem, const IDType& pivot);
  

 protected:
  int num_seqs_;
  int kmer_size_;
  char** header_;
  BioAlphabet alphabet_;
  char** sequence_;
  SeqIdxType* seq_len_;
  bool is_alphabet_set_;

};

```
