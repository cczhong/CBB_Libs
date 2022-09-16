# Minimizer Sketch
## Parameters
```c++
 struct kmer_minimizer {
        uint64_t pos;
        uint32_t length;
        string seq;
    };
    
```
## Methods
```c++
/** 
* Constructor
*/
  MinimizerSketch(void);
  
 /** Destructor **/
  ~MinimizerSketch(void);

/** 
* Computing Minimizers for each input sequence
* @param seq string sequence file name
* @param k kmer length
* @param w sliding window size
* @param pattern (boolen value) to fit to pattern or not 
* @return void
*/
 vector<kmer_minimizer> ComputeMinimizers(const std::string& seq, int& k, int& w, bool& pattern);
 
 
/** 
* Getting kmer hash value for each sequence
* @param seq sting sequence
* @param len length of sequence
* @param k kmer size 
* @param hashes vector to store all hashes of given kmers
*/  
  void CalculareKmerHash(const std::string & seq, const int& len,const int& k, uint32_t*& hashes);
 

```
## Class
```c++
class MinimizerSketch {
 public:
  MinimizerSketch(void);
  ~MinimizerSketch(void);

  vector<kmer_minimizer> ComputeMinimizers(const std::string& seq, int& k, int& w, bool& pattern);
  void CalculareKmerHash(const std::string & seq, const int& len,const int& k, uint32_t*& hashes);
  void CalculareKmerPatternedHash(const std::string & seq, const int& len,const int& k, uint32_t*& hashes);

 
};

```
