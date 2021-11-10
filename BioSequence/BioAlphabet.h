#ifndef __BIOALPHABET_H__
#define __BIOALPHABET_H__



#include "ReducedAlphabet.h"

#include <iostream>
#include <map>
#include <cstdlib>
#include <cstring>
#include <tuple>
#include <time.h>
#include <vector>

enum BioSequence {PROT, DNA, RNA, OTHER};

class BioAlphabet{
 public:
  explicit BioAlphabet() {}
  explicit BioAlphabet(enum BioSequence s);
  explicit BioAlphabet(ReducedAlphabet &reduced);
  ~BioAlphabet() {}
  // returns a random character within the alphabet
  char RandomChar();
  bool CheckSuccess();
  bool IsValid(const char c);
  inline int GetSize(void) {return alphabet_size_;}
  inline int GetCharMap(const char c) {
    return char_map_[(int) c];
  }
  inline char GetInvCharMap(const int n) {
    return (char) inv_char_map_[n];
  }
  
  enum BioSequence GetSeqType(void) {
    return seq_type_;
  }

  BioAlphabet& operator=(const BioAlphabet &alpha_source) 
  {
    this->seq_type_ = alpha_source.seq_type_;
    this->alphabet_size_ = alpha_source.alphabet_size_;
    this->num_bits_ = alpha_source.num_bits_;
    this->char_map_ = alpha_source.char_map_;
    this->inv_char_map_ = alpha_source.inv_char_map_;
    return *this;
  }
  
  friend class KmerUnitcoder;
  friend class SFABuild;
  friend class Loader;
  friend class BWT;
  friend class BWTSearch;

 protected:
  void InitProt();
  void InitDNA();
  void InitRNA();
  void InitDNAAmbiguity();
  std::vector<int> char_map_;
  std::vector<int> inv_char_map_;
  BioSequence seq_type_;
  int alphabet_size_;   // number of elements in the alphabet
  int num_bits_;        // number of bits to represent each letter in the alphabet
  
};


#endif // __BIOALPHABET_H__