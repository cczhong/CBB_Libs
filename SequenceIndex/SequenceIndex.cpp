#include "SequenceIndex.h"

using namespace std;

SequenceIndex::SequenceIndex(void)  {
  is_header_loaded_ = is_sequence_loaded_ = false;
  is_sfa_built_ = is_k_array_built_ = false;
  is_size_counted_ = false;
  is_alphabet_set_ = false;
  is_block_loaded_ = false;
  return;
}

SequenceIndex::SequenceIndex(BioAlphabet& alpha)  {
  alphabet_ = alpha;
  is_alphabet_set_ = true;
  is_block_loaded_ = false;
  is_header_loaded_ = is_sequence_loaded_ = is_sfa_built_ = false;
  is_size_counted_ = false;
  return;
}


SequenceIndex::~SequenceIndex(void) {
  if(is_sequence_loaded_)  {
    for(int i = 0; i < num_seqs_; ++ i) {
      if(sequence_[i] != NULL)  delete [] sequence_[i];
    }
    delete [] sequence_;
    delete [] seq_len_;
  }
  if(is_header_loaded_)  {
    for(int i = 0; i < num_seqs_; ++ i) {
      if(header_[i] != NULL) delete [] header_[i];
    }
    delete [] header_;
  }
  if(is_sfa_built_)  {
    delete suffix_array_;
  }
  return;
}

void SequenceIndex::DumpSFA(std::string& dir, std::string& file_stem, const IDType &pivot) {
  string idx_file = dir + "/" + file_stem + ".midx";
  suffix_array_->DumpAllBlock(idx_file.c_str(), pivot);
  return;
}

void SequenceIndex::SplitSequence(std::vector<SequenceIndex>& block_sfa)  {
  assert(this->is_sequence_loaded_ && this->is_header_loaded_);
  assert(this->is_alphabet_set_);
  assert(this->is_block_loaded_);
  assert(this->is_multi_);
  assert(block_sfa.size() == this->num_blocks_);


  for(int i = 0; i < this->num_blocks_; ++ i) {
    block_sfa[i].alphabet_ = this->alphabet_;
    block_sfa[i].is_alphabet_set_ = true;
    block_sfa[i].is_block_loaded_ = false;
    block_sfa[i].is_multi_ = false;
    block_sfa[i].is_sfa_built_ = block_sfa[i].is_k_array_built_ = false;
    block_sfa[i].is_size_counted_ = false;
    if(i < this->num_blocks_ - 1)  {
      block_sfa[i].num_seqs_ = this->block_size_[i + 1] - this->block_size_[i];
    } else{
      block_sfa[i].num_seqs_ = this->num_seqs_ - this->block_size_[i];
    }

    block_sfa[i].sequence_ = new char* [block_sfa[i].num_seqs_];
    block_sfa[i].header_ = new char* [block_sfa[i].num_seqs_];
    block_sfa[i].seq_len_ = new SeqIdxType [block_sfa[i].num_seqs_];
    for(int j = 0; j < block_sfa[i].num_seqs_; ++ j) {
      block_sfa[i].sequence_[j] = new char[strlen(this->sequence_[this->block_size_[i] + j]) + 1];
      strcpy(block_sfa[i].sequence_[j], this->sequence_[this->block_size_[i] + j]);
      block_sfa[i].seq_len_[j] = strlen(block_sfa[i].sequence_[j]);
      block_sfa[i].header_[j] = new char[strlen(this->header_[this->block_size_[i] + j]) + 1];
      strcpy(block_sfa[i].header_[j], this->header_[this->block_size_[i] + j]);
    }
    block_sfa[i].is_sequence_loaded_ = block_sfa[i].is_header_loaded_ = true;
  }
  return;
}

// the sequences will be stored in global variables "header" and "sequence_"
void SequenceIndex::LoadSequences(std::string& seq_file, const bool &rev_comp)  {
  assert(is_alphabet_set_);
  if(rev_comp && alphabet_.GetSeqType() != DNA)  {
    cout << "SequenceIndex::LoadSequence: reverse complementary is only available for DNA (nucl) sequences!" << endl;
    exit(0);
  }
  Loader seq_loader;
  num_seqs_ = seq_loader.CountFastaNumSeqs(seq_file.c_str());
  // double the number of sequences if we need to consider reverse complementary
  IDType ns = num_seqs_;   // the original number of sequences
  if(rev_comp) {
    ns = num_seqs_;
    num_seqs_ *= 2;
  }
  sequence_ = new char* [num_seqs_];
  header_ = new char* [num_seqs_];
  seq_len_ = new SeqIdxType [num_seqs_];
  for(int i = 0; i < num_seqs_; ++ i) {
    sequence_[i] = header_[i] = NULL;
    seq_len_[i] = 0;
  }
  seq_loader.LoadFasta(alphabet_, seq_file.c_str(), header_, sequence_, seq_len_);


  if(rev_comp)  {
    for(IDType i = 0; i < ns; ++ i) {
      sequence_[ns + i] = new char[seq_len_[i] + 1];
      seq_loader.ReverseComplement(sequence_[ns + i], sequence_[i], seq_len_[i]);
      seq_len_[ns + i] = seq_len_[i];
      header_[ns + i] = new char[strlen(header_[i]) + 1];
      strcpy(header_[ns + i], header_[i]);
    };
  }
  is_sequence_loaded_ = true;
  is_header_loaded_ = true;
  is_rev_comp_ = rev_comp;
  return;
}

// building the suffix array on the entire set of sequences (default)
void SequenceIndex::BuildSFADefault(void) {
  if(is_sfa_built_) { delete suffix_array_; }
  suffix_array_ = new GSA((char**) sequence_, num_seqs_, true);
  is_sfa_built_ = true;
  return;
}

void SequenceIndex::SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem) {
  block_size_.push_back(0);

  IDType npb = (IDType) num_seqs_ / num_blocks;
  if(npb < 1) npb = 1;  // set at least one sequence per block
  for(int i = 1; i < num_blocks; ++ i) {
    IDType acc_ns = npb * i;
    if(acc_ns < num_seqs_)  {
      block_size_.push_back(npb * i);
    } else  {
      break;  // break the loop if we have more block than sequences
    }
  }

  num_blocks_ = num_blocks;
  // write the block information
  std::string out_file = dir + "/" + file_stem + ".bsz";
  std::ofstream out_fh(out_file.c_str(), ios_base::out | ios_base::binary);
  if(!out_fh.good())  {
    cout << "SequenceIndex::SetBlockConfig: Cannot write block-size index file: " << out_file << endl;
    exit(1);
  }
  out_fh.write((char*) &num_blocks_, sizeof(IDType));
  for(int i = 0; i < block_size_.size(); ++ i) {
    out_fh.write((char*) &block_size_[i], sizeof(IDType));
  }
  out_fh.close();

  is_multi_ = is_block_loaded_ = true;
  return;
}
