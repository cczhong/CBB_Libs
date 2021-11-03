# Loader
Loading the input files
## Methods
```c++
/** Default constructor **/
  Loader();
/** Default destructor **/
  ~Loader();
  
  /**
  * return the number of sequences in the FASTA file
  * @param file_name: the name of the file to be read
  */
  int CountFastaNumSeqs(const char *file_name);
  /**
  * loading the FASTA file
  * @return the number of sequences
  * @param alphabet: the alphabet for the file
  * @param file_name: the name of the file to be read
  * @param header: the two-dimensional array that stores the header information
  * @param seq: the two-dimensional array that stores the sequence information
  */
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** header, char** seq,const SeqIdxType* seq_len);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** seq, const SeqIdxType* seq_len);
  
  /**
  * Getting the reverse complement of the sequence
  */
  void ReverseComplement(
    char *target, char *source, const int& len
  );

```
## Class
```c++
class Loader  {
 public:
  Loader();
  ~Loader();
  int CountFastaNumSeqs(const char *file_name);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** header, char** seq,const SeqIdxType* seq_len);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** seq, const SeqIdxType* seq_len);
  void ReverseComplement(
    char *target, char *source, const int& len
  );

 private:
  /** checking for special characters in the string
  * @param alphabet: the alphabet for the string
  * @param sseq: the string to be checked
  * @param freq_cutoff: the portion of non-standard char cutoff to drop the string (return FALSE)
  * otherwise modify the string and returns TRUE
  */
  bool CheckSpecialChar(BioAlphabet &alphabet, std::string &sseq, float freq_cutoff = 0.9);
  bool IsLowComplexity(std::string &seq);
  void RecordSequence(
    char **header, char **seq,
    std::string &single_header, std::string &single_seq,
    const int& index
  );
  void RecordSequence(
    char** seq, std::string& single_seq, const int& index
  );

};

```
