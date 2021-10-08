#ifndef _SEQ_INDEX_
#define _SEQ_INDEX_

#include "sequence.h"
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


  void DestructSFA(void);
  void DestructSequences(void);

  void LoadBlockSize(std::string& dir, std::string& file_stem);
  void SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem);
  void BuildSFADefault(void);
  void BuildSFAMulti(const int& num_blocks, std::string &dir, std::string &file_stem);
  void MergeSFAMulti(std::string &dir, std::string &file_stem);
  void MergeTwoSFA(std::string &file_sfa_1, std::string &file_sfa_2, std::string& out_file_sfa);
  void BuildKeyArrayDefault(void);
  void DumpSFA(std::string& dir, std::string& file_stem, const RIDTYPE& pivot);
  void LoadSFA(std::string& dir, std::string& file_stem);
  void LoadMultiSFA(std::string& dir, std::string& file_stem);
  void SplitSequence(std::vector<SeqIndex>& block_sfa);
  void CountDBSize(void);
  double GetDBSizeInMegaBase(void);
  void PrintAllSeqs(void);
  void PrintAllSuffixes(void);
  void PrintSeqLens(void);
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

  POSTYPE GetSeqLen(const RIDTYPE& r) {
    assert(is_sequence_loaded_);
    assert(r < num_seqs_);
    return seq_len_[r];
  }

  // returns the length of the suffix
  int GetSufLen(const int& block_ID, const GSATYPE& s);
  int GetSufLen(const GSATYPE& s);

  RIDTYPE GetFullRID(const int& block_ID, const RIDTYPE& r);
  int GetAlphabetSize(void) {
    return alphabet_.alphabet_size_;
  }

  void InitContainedInfo()  {
    assert(is_sequence_loaded_);
    contained_by_.resize(num_seqs_);
    is_contained_.resize(num_seqs_);
    is_contained_init_ = true;
    return;
  }

  RIDTYPE GetNumSeqs()  {
    assert(is_sequence_loaded_);
    return num_seqs_;
  }

 protected:
  int num_seqs_;
  double db_size_MB_;
  char** header_;
  BioAlphabet alphabet_;
  char** sequence_;
  SeqIdxType* seq_len_;
  GSA* suffix_array_;
  std::vector<GSA*> block_suffix_array_;
  std::vector<SFAIDXTYPE> key_array_;
  bool is_header_loaded_, is_sequence_loaded_, is_block_loaded_;
  bool is_sfa_built_, is_k_array_built_;
  bool is_size_counted_;
  bool is_multi_;
  bool is_rev_comp_;
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

#endif
