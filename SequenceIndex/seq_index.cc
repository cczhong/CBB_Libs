#include "seq_index.h"
#include "gsa.h"

using namespace std;

SFABuild::SFABuild(void)  {
  is_header_loaded_ = is_sequence_loaded_ = false;
  is_sfa_built_ = is_k_array_built_ = false;
  is_size_counted_ = false;
  is_alphabet_set_ = false;
  is_block_loaded_ = false;
  return;
}

SFABuild::SFABuild(BioAlphabet& alpha)  {
  alphabet_ = alpha;
  is_alphabet_set_ = true;
  is_block_loaded_ = false;
  is_header_loaded_ = is_sequence_loaded_ = is_sfa_built_ = false;
  is_size_counted_ = false;
  //LoadSequences(seq_file);
  return;
}


SFABuild::~SFABuild(void) {
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

// Accessing the contents in the object

void SFABuild::PrintAllSeqs(void)  {
  cout << "Sequences in object:" << endl;
  for(int i = 0; i < num_seqs_; ++ i) {
    cout << i << "  " << sequence_[i] << endl;
  }
  return;
}

void SFABuild::PrintSeqLens(void) {
  cout << "Sequence lengths in object:" << endl;
  for(int i = 0; i < num_seqs_; ++ i) {
    cout << i << "  " << seq_len_[i] << endl;
  }
  return;
}

void SFABuild::PrintAllSuffixes(void) {
  assert(is_sfa_built_);
  suffix_array_->printSuffix();
}

void SFABuild::PrintContainedInfo() {
  assert(is_contained_init_);
  assert(is_sequence_loaded_);
  for(RIDTYPE i = 0; i < num_seqs_; ++ i) {
    if(is_contained_[i])  {
      //cout << "===contained read found===" << endl;
      //cout << sequence_[i] << endl;
      //cout << sequence_[contained_by_[i]] << endl;
      cout << i << endl;
    }
  }
}

void SFABuild::DestructSFA(void)  {
  // clear suffix array and key array to save memory
  suffix_array_->clear();
  suffix_array_->purgeSA();
  suffix_array_->purgeDoc();
  suffix_array_->purgeLCP();
  suffix_array_->purgeMLCP();
  key_array_.clear();
  is_sfa_built_ = false;
  is_k_array_built_ = false;
}

void SFABuild::DestructSequences(void) {
  if(is_sequence_loaded_)  {
    for(int i = 0; i < num_seqs_; ++ i) {
      delete [] sequence_[i];
    }
    is_sequence_loaded_ = false;
  }
  if(is_header_loaded_)  {
    for(int i = 0; i < num_seqs_; ++ i) {
      delete [] header_[i];
    }
    is_header_loaded_ = false;
  }
  num_seqs_ = 0;
  return;
}

void SFABuild::DumpSFA(std::string& dir, std::string& file_stem, const RIDTYPE &pivot) {
  string idx_file = dir + "/" + file_stem + ".midx";
  suffix_array_->DumpAllBlock(idx_file.c_str(), pivot);
  //string lcp_file = dir + "/" + file_stem + ".lcp";
  //string mcp_file = dir + "/" + file_stem + ".mcp";
  //string gsa_file = dir + "/" + file_stem + ".gsa";
  // ignoring SFA and DOC files
  //suffix_array_->dump(
  //    "", "", lcp_file.c_str(), mcp_file.c_str(), gsa_file.c_str()
  //);
  return;
}

void SFABuild::LoadSFA(std::string& dir, std::string& file_stem) {
  string idx_file = dir + "/" + file_stem + ".midx";
  // ignoring SFA and DOC files
  cout << "idx_file:  " << idx_file << endl;
  if(!boost::filesystem::exists(idx_file))  {
    cerr << "Error: SFABuild::LoadSFA: Suffix array indexing file does not exist. Abort." << endl;
    exit(0);
  }
  // load the existing suffix array
  suffix_array_ = new GSA();
  suffix_array_->setReadCount(num_seqs_);
  suffix_array_->setSequences(sequence_);
  //cout << "here 1" << endl;
  suffix_array_->load(idx_file.c_str());
  //cout << "here 2" << endl;
  is_sfa_built_ = true;
  return;
}

void SFABuild::LoadMultiSFA(std::string& dir, std::string& file_stem) {
  assert(is_multi_ && is_block_loaded_);
  block_suffix_array_.resize(num_blocks_);
  for(int i = 0; i < num_blocks_; ++ i) {
    string idx_file = dir + "/" + file_stem + "." + std::to_string(i) + ".midx";
    // ignoring SFA and DOC files
    //cout << "idx_file:  " << idx_file << endl;
    if(!boost::filesystem::exists(idx_file))  {
      cerr << "Error: SFABuild::LoadMultiSFA: Suffix array indexing file does not exist. Abort." << endl;
      exit(0);
    }
    // load the existing suffix array
    block_suffix_array_[i] = new GSA();
    block_suffix_array_[i]->setReadCount(block_size_[i + 1] - block_size_[i] + 1);
    block_suffix_array_[i]->setSequences(sequence_ + block_size_[i]);
    //cout << "here 1" << endl;
    block_suffix_array_[i]->load(idx_file.c_str());
    //cout << "here 2" << endl;
  }
  is_sfa_built_ = true;
  return;
}

void SFABuild::SplitSequence(std::vector<SFABuild>& block_sfa)  {
  assert(this->is_sequence_loaded_ && this->is_header_loaded_);
  assert(this->is_alphabet_set_);
  assert(this->is_block_loaded_);
  assert(this->is_multi_);
  assert(block_sfa.size() == this->num_blocks_);

  //for(int i = 0; i < this->block_size_.size(); ++ i) {
  //  cout << "bs:  " << i << " " << block_size_[i] << endl;
  //}

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
    //cout << i << ": " << block_sfa[i].num_seqs_ << endl;

    block_sfa[i].sequence_ = new char* [block_sfa[i].num_seqs_];
    block_sfa[i].header_ = new char* [block_sfa[i].num_seqs_];
    block_sfa[i].seq_len_ = new int [block_sfa[i].num_seqs_];
    for(int j = 0; j < block_sfa[i].num_seqs_; ++ j) {
      block_sfa[i].sequence_[j] = new char[strlen(this->sequence_[this->block_size_[i] + j]) + 1];
      strcpy(block_sfa[i].sequence_[j], this->sequence_[this->block_size_[i] + j]);
      block_sfa[i].seq_len_[j] = strlen(block_sfa[i].sequence_[j]);
      block_sfa[i].header_[j] = new char[strlen(this->header_[this->block_size_[i] + j]) + 1];
      strcpy(block_sfa[i].header_[j], this->header_[this->block_size_[i] + j]);
    }
    block_sfa[i].is_sequence_loaded_ = block_sfa[i].is_header_loaded_ = true;

    //cout << "done copying sequences" << endl;
  }
  return;
}


void SFABuild::CountDBSize(void) {
  if(is_size_counted_)  {
    return;
  }
  db_size_MB_ = 0;
  for(int i = 0; i < num_seqs_; ++ i) {
    db_size_MB_ += (double) seq_len_[i];
  }
  db_size_MB_ /= 1000000;
  is_size_counted_ = true;
  return;
}

double SFABuild::GetDBSizeInMegaBase(void)  {
  if(!is_size_counted_)  {
    assert(is_sequence_loaded_);
    this->CountDBSize();
  }
  return db_size_MB_;
}

bool SFABuild::CheckMultiParam(const SFAIDXTYPE& max_size)  {
  double m = max_size / 1000000;
  double n = GetDBSizeInMegaBase() / m;
  // DEBUG
  //cout << m << "  " << GetDBSizeInMegaBase() << endl;
  return (n <= MAX_BLOCK);
}


// the sequences will be stored in global variables "header" and "sequence_"
void SFABuild::LoadSequences(std::string& seq_file, const bool &rev_comp)  {
  assert(is_alphabet_set_);
  if(rev_comp && alphabet_.GetSeqType() != DNA)  {
    cout << "VAT::SFABuild::LoadSequence: reverse complementary is only available for DNA (nucl) sequences!!!" << endl;
    exit(0);
  }
  Loader seq_loader;
  num_seqs_ = seq_loader.CountFastaNumSeqs(seq_file.c_str());
  //cout << "num original seqs:  " << num_seqs_ << endl;
  // double the number of sequences if we need to consider reverse complementary
  RIDTYPE ns = num_seqs_;   // the original number of sequences
  if(rev_comp) {
    ns = num_seqs_;
    num_seqs_ *= 2;
  }
  //cout << "num seqs in DB:  " << num_seqs_ << endl;
  sequence_ = new char* [num_seqs_];
  header_ = new char* [num_seqs_];
  seq_len_ = new int [num_seqs_];
  for(RIDTYPE i = 0; i < num_seqs_; ++ i) {
    sequence_[i] = header_[i] = NULL;
    seq_len_[i] = 0;
  }
  seq_loader.LoadFasta(alphabet_, seq_file.c_str(), header_, sequence_, seq_len_);

  //cout << "sequence loaded: " << seq_len_[0] << endl;

  if(rev_comp)  {
    for(RIDTYPE i = 0; i < ns; ++ i) {
      //cout << "sequence length: " << seq_len_[i] << endl;
      sequence_[ns + i] = new char[seq_len_[i] + 1];
      seq_loader.ReverseComplement(sequence_[ns + i], sequence_[i], seq_len_[i]);
      seq_len_[ns + i] = seq_len_[i];
      header_[ns + i] = new char[strlen(header_[i]) + 1];
      strcpy(header_[ns + i], header_[i]);
    }
    //cout << "sequence reverse complemented." << endl;
  }
  is_sequence_loaded_ = true;
  is_header_loaded_ = true;
  is_rev_comp_ = rev_comp;
  return;
}

void SFABuild::LoadBlockSize(std::string& dir, std::string& file_stem) {
  string block_file = dir + "/" + file_stem + ".bsz";
  std::ifstream in_fh(block_file.c_str(), std::ios::in | std::ios::binary);
  if(!in_fh.good())  {
    cout << "VAT::SFABuild::LoadBlockSize: Cannot read block size index file " << block_file << endl;
    exit(1);
  }
  in_fh.read((char*) &num_blocks_, sizeof(RIDTYPE));
  //cout << "num_block: " << num_blocks_ << endl;

  RIDTYPE t;
  while(!in_fh.eof())  {
    in_fh.read((char*) &t, sizeof(RIDTYPE));
    block_size_.push_back(t);
    //cout << "block_size:  " << t << endl;
  }
  in_fh.close();
  block_size_.resize(block_size_.size() - 1);
  is_multi_ = true;
  is_block_loaded_ = true;

  // DEBUG
  //cout << "=====Block sizes=====" << endl;
  //for(int i = 0; i < block_size_.size(); ++ i) {
  //  cout << i << "  " << block_size_[i] << endl;
  //}
  return;
}

// copy sequence from one array to another
void SFABuild::CopySequences(int num_sequences, char **source, char **target) {
  target = new char* [num_sequences];
  for(int i = 0; i < num_sequences; ++ i) {
    target[i] = new char[strlen(source[i]) + 1];
    strcpy(target[i], source[i]);
  }
  //is_sequence_loaded_ = true;
  //num_seqs_ = num_sequences;
  return;
}

// reverse the sequences in array "sequence_"
void SFABuild::InPlaceReverse(void)  {
  for(int i = 0; i < num_seqs_; ++ i) {
    int l = strlen(sequence_[i]);
    for(int j = 0; j < floor(l / 2); ++ j) {
      swap(sequence_[i][j], sequence_[i][l - 1 - j]);
    }
  }
  return;
}

// building the suffix array on the entire set of sequences (default)
void SFABuild::BuildSFADefault(void) {
  if(is_sfa_built_) { delete suffix_array_; }
  //cout << "num_seqs:  " << num_seqs_ << endl;
  suffix_array_ = new GSA((char**) sequence_, num_seqs_, true);
  is_sfa_built_ = true;
  // write the suffix array index
  //DumpSFA(dir, file_stem, 0);
  return;
}

void SFABuild::SetBlockConfig(const int& num_blocks, std::string &dir, std::string &file_stem) {
  RIDTYPE begin = 0;
  SFAIDXTYPE accum_size = 0;
  block_size_.push_back(0);

  RIDTYPE npb = (RIDTYPE) num_seqs_ / num_blocks;
  if(npb < 1) npb = 1;  // set at least one sequence per block
  for(int i = 1; i < num_blocks; ++ i) {
    RIDTYPE acc_ns = npb * i;
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
    cout << "VAT::BuildIndex::BuildSFAMulti: Cannot write block-size index file: " << out_file << endl;
    exit(1);
  }
  out_fh.write((char*) &num_blocks_, sizeof(RIDTYPE));
  for(int i = 0; i < block_size_.size(); ++ i) {
    out_fh.write((char*) &block_size_[i], sizeof(RIDTYPE));
  }
  out_fh.close();

  is_multi_ = is_block_loaded_ = true;
  return;
}

void SFABuild::BuildSFAMulti(const int& num_blocks, std::string &dir, std::string &file_stem)  {

  // construct multi block-SAs
  RIDTYPE begin = 0;
  RIDTYPE block_ID = 0;
  SFAIDXTYPE accum_size = 0;
  block_size_.push_back(0);

  RIDTYPE npb = (RIDTYPE) num_seqs_ / num_blocks;
  if(npb < 1) npb = 1;  // set at least one sequence per block
  for(int i = 1; i < num_blocks; ++ i) {
    RIDTYPE acc_ns = npb * i;
    if(acc_ns < num_seqs_)  {
      block_size_.push_back(npb * i);
    } else  {
      break;  // break the loop if we have more block than sequences
    }
  }
  for(int i = 0; i < block_size_.size() - 1; ++ i) {
    if(is_sfa_built_) { delete suffix_array_; }
    suffix_array_ = new GSA((char**) (sequence_ + block_size_[i]), npb, true);
    is_sfa_built_ = true;
    // write the suffix array index
    std::string block_stem = file_stem + "." + std::to_string(block_ID);
    DumpSFA(dir, block_stem, begin);
    ++ block_ID;
  }
  // handle the last block
  if(block_size_[block_size_.size() - 1] < num_seqs_)  {
    if(is_sfa_built_) { delete suffix_array_; }
    suffix_array_ = new GSA((char**) (sequence_ + block_size_[block_size_.size() - 1]), num_seqs_ - block_size_[block_size_.size() - 1], true);
    is_sfa_built_ = true;
    // write the suffix array index
    std::string block_stem = file_stem + "." + std::to_string(block_ID);
    DumpSFA(dir, block_stem, begin);
    ++ block_ID;
  }

  // write the block information into disk
  std::string out_file = dir + "/" + file_stem + ".bsz";
  std::ofstream out_fh(out_file.c_str(), ios_base::out | ios_base::binary);
  if(!out_fh.good())  {
    cout << "VAT::BuildIndex::BuildSFAMulti: Cannot write block-size index file: " << out_file << endl;
    exit(1);
  }
  out_fh.write((char*) &block_ID, sizeof(RIDTYPE));
  for(int i = 0; i < block_size_.size(); ++ i) {
    out_fh.write((char*) &block_size_[i], sizeof(RIDTYPE));
  }
  out_fh.close();

  //DEBUG
  //cout << "Num blocks:  " << block_ID << endl;
  return;
}


void SFABuild::MergeSFAMulti(std::string &dir, std::string &file_stem)  {

  for(int i = 0; i < ceil(log2(block_size_.size())); ++ i)   {
    int k = (int) pow(2, i);
    for(int j = 0; j < block_size_.size(); j += 2 * k)   {
      if(j + k < block_size_.size())    {
        // merge the two suffix arrays
        string sfa_file_1 = dir + "/" + file_stem + "." + std::to_string(j) + ".midx";
        string sfa_file_2 = dir + "/" + file_stem + "." + std::to_string(j + k) + ".midx";
        string out_sfa = dir + "/" + file_stem + ".tmp.gsa";

        // DEBUG
        //cout << "merging files: " << sfa_file_1 << "  " << sfa_file_2 << endl;
        MergeTwoSFA(sfa_file_1, sfa_file_2, out_sfa);
        // delete the obsolete files and rename the merged one
        if(std::rename(out_sfa.c_str(), sfa_file_1.c_str()) != 0) {
          std::cout << "VAT::BuildIndex::MergeSFAMulti: Cannot rename file: " << out_sfa << endl;
          exit(1);
        }
        if(std::remove(sfa_file_2.c_str()) != 0) {
          std::cout << "VAT::BuildIndex::MergeSFAMulti: Cannot remove file: " << sfa_file_2 << endl;
          exit(1);
        }
      }
    }
  }
  // rename the merged file
  string src_file = dir + "/" + file_stem + ".0.midx";
  string merged_file = dir + "/" + file_stem + ".midx";
  if(std::rename(src_file.c_str(), merged_file.c_str()) != 0) {
    std::cout << "VAT::BuildIndex::MergeSFAMulti: Cannot rename file: " << src_file << endl;
    exit(1);
  }
  return;
}


void SFABuild::MergeTwoSFA(std::string &file_sfa_1, std::string &file_sfa_2, std::string& out_file_sfa)
{
  assert(is_sequence_loaded_);
  // prepare the input and output file handles
  std::ifstream in_sfa_fh_1(file_sfa_1.c_str(), ios_base::in | ios_base::binary);
  if(!in_sfa_fh_1.good())  {
    cout << "MANA::SFABuild::MergeTwoSFA: Cannot open suffix array index file: " << file_sfa_1 << endl;
    exit(1);
  }

  std::ifstream in_sfa_fh_2(file_sfa_2.c_str(), ios_base::in | ios_base::binary);
  if(!in_sfa_fh_2.good())  {
    cout << "MANA::SFABuild::MergeTwoSFA: Cannot open suffix array index file: " << file_sfa_2 << endl;
    exit(1);
  }

  std::ofstream out_sfa_fh(out_file_sfa.c_str(), ios_base::out | ios_base::binary);
  if(!out_sfa_fh.good())  {
    cout << "MANA::SFABuild::MergeTwoSFA: Cannot create merged suffix array index: " << out_file_sfa << endl;
    exit(1);
  }

  // compare the suffixes one-by-one
  UtilFunc util;
  int pab = 0;    // the LCP between the two suffixes that are compared in the previous stage
  LCPTYPE lcp_buffer = 0;             // the LCP from the last comparison
  bool is_prev_a = false;             // indicates whether the previous suffix comes from a

  RIDTYPE doc_a, doc_b;
  POSTYPE pos_a, pos_b;
  POSTYPE lcp_a, lcp_b;
  bool read_a = true, read_b = true;

  SFAIDXTYPE size1, size2, size_new, size_check = 0;
  in_sfa_fh_1.read((char*) &size1, sizeof(SFAIDXTYPE));
  in_sfa_fh_2.read((char*) &size2, sizeof(SFAIDXTYPE));
  assert(size1 > 0 && size2 > 0);
  size_new = size1 + size2;
  out_sfa_fh.write((char*) &size_new, sizeof(SFAIDXTYPE));

  while(true) {
    // check end of file, and add the last entrie if necessary
    if(in_sfa_fh_1.peek() == EOF || in_sfa_fh_2.peek() == EOF)    {
      if(is_prev_a) {
        // the previous written suffix is in A, so we write the suffix of B
        out_sfa_fh.write((char*) &doc_b, sizeof(RIDTYPE));
        out_sfa_fh.write((char*) &pos_b, sizeof(POSTYPE));
        out_sfa_fh.write((char*) &lcp_buffer, sizeof(POSTYPE));
        ++ size_check;
        is_prev_a = false;
      } else  {
        out_sfa_fh.write((char*) &doc_a, sizeof(RIDTYPE));
        out_sfa_fh.write((char*) &pos_a, sizeof(POSTYPE));
        out_sfa_fh.write((char*) &lcp_buffer, sizeof(POSTYPE));
        ++ size_check;
        is_prev_a = true;
      }
      break;
    }

    // loads in the information
    if(read_a)  {
      in_sfa_fh_1.read((char*) &doc_a, sizeof(RIDTYPE));
      in_sfa_fh_1.read((char*) &pos_a, sizeof(POSTYPE));
      in_sfa_fh_1.read((char*) &lcp_a, sizeof(POSTYPE));
    }
    if(read_b)  {
      in_sfa_fh_2.read((char*) &doc_b, sizeof(RIDTYPE));
      in_sfa_fh_2.read((char*) &pos_b, sizeof(POSTYPE));
      in_sfa_fh_2.read((char*) &lcp_b, sizeof(POSTYPE));
    }
    // DEBUG
    //cout << "comparison:    " << endl;
    //cout << doc_a << "  " << pos_a << " " << lcp_a << endl;
    //cout << doc_b << "  " << pos_b << " " << lcp_b << endl;
    //cout << (char*) (sequence_[doc_a] + pos_a) << endl;
    //cout << (char*) (sequence_[doc_b] + pos_b) << endl;

    pair<int, int> c = util.CmpStrWithLCP(
      (char*) (sequence_[doc_a] + pos_a),
      (char*) (sequence_[doc_b] + pos_b),
      pab
    );

    // DEBUG
    //cout << "results:   " << c.first << "   " << c.second << endl;

    if(c.first < 0 || (c.first == 0 && doc_a < doc_b))  {
      // write suffix in the first file
      out_sfa_fh.write((char*) &doc_a, sizeof(RIDTYPE));
      out_sfa_fh.write((char*) &pos_a, sizeof(POSTYPE));
      POSTYPE nlcp = is_prev_a ? lcp_a : lcp_buffer;
      out_sfa_fh.write((char*) &nlcp, sizeof(POSTYPE));
      ++ size_check;
      // update information for the next phase
      read_a = true; read_b = false;
      is_prev_a = true;
      lcp_buffer = c.second;              // record the LCP
      pab = pab > lcp_a ? lcp_a : pab;    // pick the smaller one
    }   else    {
      // write the suffix in the second file
      out_sfa_fh.write((char*) &doc_b, sizeof(RIDTYPE));
      out_sfa_fh.write((char*) &pos_b, sizeof(POSTYPE));
      POSTYPE nlcp = !is_prev_a ? lcp_b : lcp_buffer;
      out_sfa_fh.write((char*) &nlcp, sizeof(POSTYPE));
      ++ size_check;
      // update information for the next phase
      read_a = false; read_b = true;
      is_prev_a = false;
      lcp_buffer = c.second;              // record the LCP
      pab = pab > lcp_b ? lcp_b : pab;    // pick the smaller one
    }
  }
  // copy the rest information if either clump is not exhausted
  bool is_first = true;
  if(in_sfa_fh_2.peek() == EOF) {
    while(true) {
      if(in_sfa_fh_1.peek() == EOF)  break;
      in_sfa_fh_1.read((char*) &doc_a, sizeof(RIDTYPE));
      in_sfa_fh_1.read((char*) &pos_a, sizeof(POSTYPE));
      in_sfa_fh_1.read((char*) &lcp_a, sizeof(POSTYPE));
      out_sfa_fh.write((char*) &doc_a, sizeof(RIDTYPE));
      out_sfa_fh.write((char*) &pos_a, sizeof(POSTYPE));
      if(is_first)  {
        lcp_a = lcp_a < lcp_buffer ? lcp_a : lcp_buffer;
        is_first = false;
      }
      out_sfa_fh.write((char*) &lcp_a, sizeof(POSTYPE));
      ++ size_check;
    }
  }   else if(in_sfa_fh_1.peek() == EOF)   {
    while(true) {
      if(in_sfa_fh_2.peek() == EOF)  break;
      in_sfa_fh_2.read((char*) &doc_b, sizeof(RIDTYPE));
      in_sfa_fh_2.read((char*) &pos_b, sizeof(POSTYPE));
      in_sfa_fh_2.read((char*) &lcp_b, sizeof(POSTYPE));
      out_sfa_fh.write((char*) &doc_b, sizeof(RIDTYPE));
      out_sfa_fh.write((char*) &pos_b, sizeof(POSTYPE));
      if(is_first)  {
        lcp_b = lcp_b < lcp_buffer ? lcp_b : lcp_buffer;
        is_first = false;
      }
      out_sfa_fh.write((char*) &lcp_b, sizeof(POSTYPE));
      ++ size_check;
    }
  }
  if(size_new != size_check)  {
    cout << "MANA::SFABuild::MergeTwoSFA: Index file(s) could be corrupted. Abort." << file_sfa_1 << " or " << file_sfa_2 << endl;
  }
  return;
}

// building the suffix array
void SFABuild::BuildSuffixArray(char** sequences, GSA* suffix_array) {
  suffix_array_ = new GSA(sequence_, num_seqs_, true);
  is_sfa_built_ = true;
  return;
}

// building key array that indicates the maximal-extension suffix for each entry
void SFABuild::BuildKeyArray(GSA* suffix_array, std::vector<SFAIDXTYPE>& key_array)  {
  SFAIDXTYPE n = suffix_array->getSize();
  key_array.resize(n);
  SFAIDXTYPE block_begin = 0;
  for(SFAIDXTYPE i = 0; i < n - 1; ++ i) {
    // if current suffix length greater than LCP with the next suffix
    // then the current suffix is the end of the block
    if(suffix_array->getSuffixLength(i) > suffix_array->getLcpAt(i + 1)) {
      for(SFAIDXTYPE j = block_begin; j <= i; ++ j) {
        key_array[j] = i;
      }
      block_begin = i + 1;
    }
  }
  // the last block
  for(SFAIDXTYPE j = block_begin; j < n; ++ j) {
    key_array[j] = n - 1;
  }
  return;
}

// building the default key array
void SFABuild::BuildKeyArrayDefault(void)  {
  if(!is_sfa_built_)  {
    cout << "Warning: SFABuild::BuildKeyArrayDefault attempt to build key array without suffix array built" << endl;
  }
  BuildKeyArray(suffix_array_, key_array_);
  is_k_array_built_ = true;
  return;
}

// access a given sequence
std::string SFABuild::GetSequence(const RIDTYPE& index)  {
  if(!is_sequence_loaded_)  {
    cout << "Warning: SFABuild::GetSequence no sequence loaded" << endl;
    return string("");
  }
  if(index < 0 || index >= num_seqs_)  {
    cout << "Warning: SFABuild::GetSequence sequence index out of range" << endl;
    return string("");
  }
  return string(sequence_[index]);
}

std::string SFABuild::GetSequence(const int& block_ID, const RIDTYPE& index)  {
  assert(is_sequence_loaded_);
  assert(is_multi_);
  assert(block_ID < num_blocks_);
  assert(block_size_[block_ID] + index < num_seqs_);
  return string(sequence_[index + block_size_[block_ID]]);
}

RIDTYPE SFABuild::GetFullRID(const int& block_ID, const RIDTYPE& r) {
  assert(is_sequence_loaded_);
  assert(is_multi_);
  assert(block_ID < num_blocks_);
  assert(block_size_[block_ID] + r < num_seqs_);
  return block_size_[block_ID] + r;
}

std::string SFABuild::GetHeader(int index)  {
  if(!is_sequence_loaded_)  {
    cout << "Warning: SFABuild::GetSequence no sequence loaded" << endl;
    return string("");
  }
  if(index < 0 || index >= num_seqs_)  {
    cout << "Warning: SFABuild::GetSequence sequence index out of range" << endl;
    return string("");
  }
  return string(header_[index]);
}

// access a given suffix from the suffix array
std::string SFABuild::GetSuffixSFA(int index) {
  if(!is_sfa_built_)  {
    cout << "Warning: SFABuild::GetSuffixSFA suffix array not built" << endl;
    return string("");
  }
  if(index < 0 || index >= suffix_array_->getSize())  {
    cout << "Warning: SFABuild::GetSuffixSFA suffix index out of range" << endl;
    return string("");
  }
  return string(suffix_array_->getSuffix(index));
}

// search the suffix array within the object
std::pair<SFAIDXTYPE, SFAIDXTYPE> SFABuild::SearchSFA(std::string& search_seed) {
  if(!is_sfa_built_)  {
    cout << "Fatal Error: SFABuild::SearchSFA suffix array not built" << endl;
    exit(1);
  }
  return suffix_array_->searchWithLCPs(
      (SFACHARTYPE*) search_seed.c_str(), search_seed.length()
  );
}

// find a list of maximal extension reads within the given range
void SFABuild::GetMaxExtInfoWithinRange(
    std::pair<SFAIDXTYPE, SFAIDXTYPE>& range,
    std::list<GSATYPE>& pos_list
)  {
  if(!is_k_array_built_)  {
    cout << "Fatal Error: SFABuild::GetMaxExtRIDWithRange key array not built" << endl;
    exit(1);
  }
  SFAIDXTYPE index = range.first;
  do  {
    index = key_array_[index] + 1;
    GSATYPE posT;
    posT.doc = (RIDTYPE) suffix_array_->getId(index - 1);
    posT.pos = (POSTYPE) suffix_array_->getPos(index - 1);
    pos_list.push_back(posT);
  } while (index <= range.second);
  return;
}

// interface function that helps the access of the protected "sequence_" variable
std::string SFABuild::GetSuffixSeq(
  const GSATYPE& s,   // the location of the sequence needs to be copied
  const int& l        // the length of the sequence needs to be copied
) {
  assert(is_sequence_loaded_);
  assert(s.doc < num_seqs_);
  string str((char*) (sequence_[s.doc] + s.pos), l * sizeof(char));
  return str;
}

std::string SFABuild::GetSuffixSeq(
  const GSATYPE& s    // the location of the sequence needs to be copied
) {
  assert(is_sequence_loaded_);
  assert(s.doc < num_seqs_);
  string str((char*) (sequence_[s.doc] + s.pos));
  return str;
}

std::string SFABuild::GetSuffixSeq(
  const int& block_ID,  // the ID of the block if multiple SFA was generated
  const GSATYPE& s,     // the location of the sequence needs to be copied
  const int& l          // the length of the sequence needs to be copied
) {
  assert(is_multi_);
  assert(is_sequence_loaded_);
  assert(block_ID < block_size_.size());

  // DEBUG
  //cout << "block ID:  " << block_ID << "  block_size: " << block_size_[block_ID] << endl;
  string str((char*) (sequence_[s.doc + block_size_[block_ID]] + s.pos), l * sizeof(char));
  //cout << str << endl;
  return str;
}

std::string SFABuild::GetSuffixSeq(
  const int& block_ID,  // the ID of the block if multiple SFA was generated
  const GSATYPE& s      // the location of the sequence needs to be copied
) {
  assert(is_multi_);
  assert(block_ID < block_size_.size());

  // DEBUG
  //cout << "block ID:  " << block_ID << "  block_size: " << block_size_[block_ID] << endl;
  string str((char*) (sequence_[s.doc + block_size_[block_ID]] + s.pos));
  //cout << str << endl;
  return str;
}

int SFABuild::GetNumBlocks(void) {
  assert(is_multi_);        // verify the object contains multiple suffix array blocks
  assert(num_blocks_ > 0);  // verify at least one block exists
  assert(num_blocks_ == block_size_.size());   // verify that the begin sequence ID for each block is load
  return num_blocks_;
}

int SFABuild::GetSufLen(const int& block_ID, const GSATYPE& s) {
  assert(is_sequence_loaded_);
  assert(is_multi_);
  assert(block_ID < num_blocks_);
  //if(block_size_[block_ID] + s.doc >= num_seqs_)  {
    // TODO: solving the problem by adding the number of entries at top for each index
  //  cout << "Num seqs:  " << num_seqs_ << endl;
  //  cout << "block size:  " << block_size_[block_ID] << endl;
  //  cout << "seq index: " << s.doc << endl;
  //}
  assert(block_size_[block_ID] + s.doc < num_seqs_);
  // DEBUG
  //cout << "GetSufLen: " << seq_len_[s.doc + block_size_[block_ID]] << endl;
  //cout << "GetSufLen: " << s.pos << endl;
  return seq_len_[s.doc + block_size_[block_ID]] - s.pos;
}

int SFABuild::GetSufLen(const GSATYPE& s) {
  assert(is_sequence_loaded_);
  //if(block_size_[block_ID] + s.doc >= num_seqs_)  {
    // TODO: solving the problem by adding the number of entries at top for each index
  //  cout << "Num seqs:  " << num_seqs_ << endl;
  //  cout << "block size:  " << block_size_[block_ID] << endl;
  //  cout << "seq index: " << s.doc << endl;
  //}
  if(s.doc >= num_seqs_)  {
    cout << "GetSufLen: " << s.pos << " " << num_seqs_ << endl;
  }
  assert(s.doc < num_seqs_);
  // DEBUG
  //cout << "GetSufLen: " << seq_len_[s.doc + block_size_[block_ID]] << endl;
  return seq_len_[s.doc]- s.pos;
}
