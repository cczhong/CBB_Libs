# Loader
Loading the FASTA file
## Methods
```c
/**
 *Loading the FASTA file 
 *@param file_name the name of the file to be read
 *@return int the number of sequences in the FASTA file
 */
int CountFastaNumSeqs(const char *file_name);
/**
 *Loading the FASTA file 
 *@param alphabet the alphabet for the file
 *@param file_name the name of the file to be read
 *@param header the two-dimensional array that stores the header information
 *@param seq the two-dimensional array that stores the sequence information
 *@param seq_len the length of sequence
 *@return int the number of sequences
 */
int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** header, char** seq, int* seq_len);
int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** seq, int* seq_len);
```
## Class
```c++

class Loader  {
 public:
  Loader();
  ~Loader();
  int CountFastaNumSeqs(const char *file_name);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** header, char** seq, int* seq_len);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** seq, int* seq_len);
  
 private:
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