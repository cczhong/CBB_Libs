#ifndef _BWT_SEARCH_H_
#define _BWT_SEARCH_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <unordered_map>

#include "BWT.h"

static int cost = 6;
static int m_cost = 3;
static int g_cost = 4;
static int max_queue_size = 100;

struct ExtInfo  {
  std::pair<BWTIDX, BWTIDX> range;
  BWTINT q_pos;
  BWTINT cost;
  BWTINT cost_bound;
#ifdef DEBUG
  std::list<BWTIDX> history;
#endif
};

struct TargetOverlapType  {
  BWTINT rid;
  BWTSHORT overlap_len;
  bool visited;
};

struct FullTargetOverlapType  {
  BWTINT rid;
  BWTINT edge_len;
  BWTSHORT overlap_len;
};

struct FwReSearchType {
  std::pair<BWTIDX, BWTIDX> fw_range, re_range;
  int search_cost;
  int pivot_len, db_len;
  //std::string s;
};

class ExtInfoComp {
 public:
  ExtInfoComp() {}
  
  bool operator() (const ExtInfo &lhs, const ExtInfo &rhs) const
  {
    return (lhs.cost >= rhs.cost);
  }
};

struct IvType {
  std::pair<BWTIDX, BWTIDX> ivA_, ivB_;
  int pivot_len_, db_len_; 
};

// defined as a set of forward intervals, reverse intervals, and lengths of these intervals
class IvSet {
 public:
  
  //std::vector<std::pair<BWTIDX, BWTIDX> > ivA_, ivB_;
  //std::vector<int> pivot_len_;
  //std::vector<int> db_len_;
  std::vector<IvType> iv_list_; 
  
  explicit IvSet(void) {}
  void Clear() {  iv_list_.clear();  }
  inline void PushIV(const BWTIDX a_first, const BWTIDX a_second, const BWTIDX b_first, const BWTIDX b_second, const int p_len, const int d_len)  {
    IvType iv; 
    iv.ivA_ = std::make_pair(a_first, a_second);
    iv.ivB_ = std::make_pair(b_first, b_second);
    iv.pivot_len_ = p_len;
    iv.db_len_ = d_len;
    iv_list_.push_back(iv);
  }
  bool IsRedundantA(const BWTIDX first, const BWTIDX second) {
    for(int i = 0; i < iv_list_.size(); ++ i) {
      if(iv_list_[i].ivA_.first == first && iv_list_[i].ivA_.second == second)  return true;
    }
    return false;
  }
  inline int GetSize(void)  { return iv_list_.size(); }
  bool Check(void)  {
    for(int i = 0; i < iv_list_.size() - 1; ++ i) {
      if(iv_list_[i].pivot_len_ < iv_list_[i + 1].pivot_len_) return false;
    }
    return true;
  }

  void Sort(void);
  void PrintInterval(void)  {
    std::cout << "==================" << std::endl;
    for(int i = 0; i < iv_list_.size(); ++ i) {
      std::cout << iv_list_[i].ivA_.first << " " << iv_list_[i].ivA_.second << std::endl;
    }
    std::cout << "==================" << std::endl;
  }
};

class IvInfo  {
 public:
  BWT *bwtF_, *bwtR_;
  IvSet intervals_;
  
  explicit IvInfo(BWT *fw_bwt, BWT *re_bwt) {
    assert(fw_bwt->GetSize() == re_bwt->GetSize());
    bwtF_ = fw_bwt; bwtR_ = re_bwt;
    return;
  }
  // reset the intervals in the object while keeping the BWTs
  inline void Reset() {intervals_.Clear();}
};

struct AlignType {
  BWTIDX bwt_begin, bwt_end;
  BWTINT q_begin, q_end, cost;
};

class BWTSearch {
 public:
  explicit BWTSearch() {}
  ~BWTSearch() {}
  // search "str" against the database for exact match
  void SearchExact(BWT &bwt, const char *str, AlignType &pos);
  // a wrapper function for SearchInExact from both directions
  void Search(BWT &bwt, BWT &rev_bwt, 
      const char *str, AlignType &pos
  );
  // search all prefixes of "str"; the minimum length of prefix to search is "min_len"
  void SearchAllSubRegions(
      BWT &bwt, const int min_len, const char *str, std::vector<AlignType> &all_pos
  );
  bool UpdateForwardBackward(
      IvInfo &search_info, const char c, 
      std::pair<BWTIDX, BWTIDX> &fw_range, std::pair<BWTIDX, BWTIDX> &re_range
  );

  // given a read of interest and minimum overlap, find all intervals (in both fw and re FM-indexes) 
  // corresponding to the prefix of the reads that perfectly overlap with the given read
  void SearchBeginIntervals(const char* seq, const int min_len, IvInfo &search_info);
  void SearchBeginIntervals_V2(const char* seq, const int min_len, IvInfo &search_info);
  // given a set of intervals identified by SearchBeginIntervals,
  // find a set of positions that correspond to the irreducible positions in the reverse BWT
  void FindIrreducible(
      IvInfo &search_info, std::vector<BWTIDX> &ir_positions, std::vector<int> &ir_overlap
  );
  // check if the read is contained (a PROPER substring) by another read
  // if yes return true or otherwise return false 
  // also output the position in the BWT using argument "pos"
  bool IsContainedRead(const char* seq, BWT &bwt, AlignType &pos);

  // compute all reads that overlap with the current sequence
  void ComputeOverlapInfo(
      char **seqs, const int rid, const int target_id_begin, BWT &bwt, const int min_len, 
      std::vector<std::vector<TargetOverlapType> *> *extension, std::vector<bool> *contained
  );

  void RemoveReducibleEdges(
    char **seqs, const int rid, 
    std::vector<bool> *contained, std::vector<TargetOverlapType> &overlaps
  );

 protected:
  // the function calucates the lower bound and stores in the array "bound"
  // returns the length of the longest stretch of pefectly matched substring
  void CalLowerBound(BWT &rev_bwt, const char *str, int *bound);
  // search "str" against the database for in-exact mathc with up to n mismatch/gaps
  // cost is the total cost, m_cost is mismatch cost, and g_cost is gap cost
  void SearchInExact(
      BWT &bwt, BWT &rev_bwt, const int *bound,
      const char *str, AlignType &pos
  );
  
  // each phase in the searchInExact extension
  // progressively extend unless mismatch is found
  void ExtendInExactFast(
      BWT &bwt, const char *str, 
      std::priority_queue<ExtInfo, std::vector<ExtInfo>, ExtInfoComp> &candidate, 
      const int *bound, int lower_cost
  );
  void Enqueue(
      std::priority_queue<ExtInfo, std::vector<ExtInfo>, ExtInfoComp> &candidate, 
      ExtInfo &phase_info
  );  

};

#endif
