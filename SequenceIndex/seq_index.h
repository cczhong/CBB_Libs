#ifndef _SEQ_INDEX_
#define _SEQ_INDEX_

#include "loader.h"
#include "gsa.h"
#include "util_func.h"

#include <boost/filesystem.hpp>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <list>
#include <sys/stat.h>

#define MAX_BLOCK 128

typedef uint32_t SeqIdxType;

class SeqIndex {
 public:
  SeqIndex(void);
  SeqIndex(BioAlphabet& alpha);
  ~SeqIndex(void);

  void LoadSequences(std::string& seq_file, const bool &rev_comp);
  void SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem);
  void SplitSequence(std::vector<SeqIndex>& block_sfa);
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

#endif
