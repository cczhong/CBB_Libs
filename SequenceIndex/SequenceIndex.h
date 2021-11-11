#ifndef __SEQUENCEINDEX_H__
#define __SEQUENCEINDEX_H__

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

//#include "GSA.h"
#include "Loader.h"
#include "UtilFunctions.h"
//#include "../CommonDef/dataType.h"


typedef uint32_t IDType;

/**
 * @brief 
 * 
 */
class SequenceIndex 
{
 public:
  SequenceIndex(void);
  SequenceIndex(BioAlphabet& alpha);
  ~SequenceIndex(void);

  void LoadSequences(std::string& seq_file, const bool &rev_comp);
  void SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem);
  void SplitSequence(std::vector<SequenceIndex>& block_sfa);
  void BuildSFADefault(void);
  //void DumpSFA(std::string& dir, std::string& file_stem, const IDType& pivot);


 protected:
  int num_seqs_;
  char** header_;
  BioAlphabet alphabet_;
  char** sequence_;
  int* seq_len_;
  //GSA* suffix_array_;
  bool is_header_loaded_, is_sequence_loaded_, is_block_loaded_;
  bool is_sfa_built_, is_k_array_built_;
  bool is_size_counted_;
  bool is_multi_;
  bool is_rev_comp_;
  int num_blocks_;
  std::vector<uint32_t> block_size_;
  bool is_alphabet_set_;

};

#endif
