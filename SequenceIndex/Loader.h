#ifndef __LOADER_H__
#define __LOADER_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <list>
#include <unordered_map>
#include <ctype.h>

#include "BioAlphabet.h"
#include "SequenceIndex.h"

#ifndef MAXSEQLEN
#define MAXSEQLEN 1000000
#endif

class Loader  {
 public:
  Loader();
  ~Loader();
  // return the number of sequences in the FASTA file
  // file_name: the name of the file to be read
  int CountFastaNumSeqs(const char *file_name);
  /* loading the FASTA file
     return the number of sequences
     alphabet: the alphabet for the file
     file_name: the name of the file to be read
     header: the two-dimensional array that stores the header information
     seq: the two-dimensional array that stores the sequence information*/
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** header, char** seq,const SeqIdxType* seq_len);
  int LoadFasta(BioAlphabet &alphabet, const char* file_name, char** seq, const SeqIdxType* seq_len);
  void ReverseComplement(
    char *target, char *source, const int& len
  );

 private:
  // checking for special characters in the string
  // alphabet: the alphabet for the string
  // sseq: the string to be checked
  // freq_cutoff: the portion of non-standard char cutoff to drop the string (return FALSE)
  // otherwise modify the string and returns TRUE
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

#endif
