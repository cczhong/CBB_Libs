#include "BWTSearch.h"

using namespace std;

bool SortIvType(const IvType &a, const IvType &b) {
  if(a.pivot_len_ > b.pivot_len_ || a.ivA_.second - a.ivA_.first > b.ivA_.second - b.ivA_.first) return true;
  return false;
}

void IvSet::Sort(void) {
  /*
  cout << "======================" << endl;
  for(int i = 0; i < iv_list_.size(); ++ i) {
    cout << "len: " << iv_list_[i].pivot_len_ << endl;
  }
  cout << "======================" << endl;
  */  
  
  int n = iv_list_.size();
  if(n <= 1)  return;
  IvType tmp;
  for(int i = 0; i < (int) n / 2; ++ i) {
    tmp = iv_list_[i]; iv_list_[i] = iv_list_[n - i - 1]; iv_list_[n - i - 1] = tmp;
  }
  if(Check())  return;
  vector<IvType> sorted_iv_list = iv_list_;
  int max_pivot_len = iv_list_[0].pivot_len_;
  int min_pivot_len = iv_list_[0].pivot_len_;
  for(int i = 1; i < n; ++ i) {
    if(iv_list_[i].pivot_len_ > max_pivot_len) max_pivot_len = iv_list_[i].pivot_len_;
    if(iv_list_[i].pivot_len_ < min_pivot_len) min_pivot_len = iv_list_[i].pivot_len_;
  }
  int idx = 0;
  for(int i = max_pivot_len; i >= min_pivot_len; -- i) {
    for(int j = 0; j < n; ++ j) {
      if(iv_list_[j].pivot_len_ == i) sorted_iv_list[idx ++] = iv_list_[j];
    }
  }
  iv_list_ = sorted_iv_list;
  return;
}

// sorting the AlignType
bool SortAlignTypeBySize(const AlignType &a, const AlignType &b)  {
  int a_dist = a.q_end - a.q_begin;
  int b_dist = b.q_end - b.q_begin;
  if(a_dist > b_dist || (a_dist == b_dist && a.q_begin < b.q_begin))  {
    return true;
  }  
  return false;
} 

void BWTSearch::SearchExact(BWT &bwt, const char *str, AlignType &pos) {
  
  int len = strlen(str);
  if(len <= 0)  {
    cerr << "Searching empty sequence. An empty list will be returned." << endl;
    return;
  }
  pair<BWTIDX, BWTIDX> range;
  range.first = 0, range.second = bwt.GetSize();
  int i;
  for(i = strlen(str) - 1; i >= 0; -- i) {
    // indicating that there is no solution
    if(range.first >= range.second)  break;
    range = bwt.UpdateRange(str[i], range);    
  }
  // record the positions
  pos.q_begin = i + 1; pos.q_end = strlen(str) - 1; pos.cost = 0;
  pos.bwt_begin = range.first; pos.bwt_end = range.second;
  return;
}

void BWTSearch::SearchAllSubRegions(
    BWT &bwt, const int min_len, const char *str, std::vector<AlignType> &all_pos
) {
  int len = strlen(str);
  if(len <= 0)  {
    cerr << "BWTSearch::Search: Searching empty sequence. An empty list will be returned." << endl; return;
  }
  BWTIDX bwt_size = bwt.GetSize();
  // reverse the string
  string s = str; s = string(s.rbegin(), s.rend());
  //cout << "Query sequence:  " << str << endl;
  // construct a vector of ranges
  vector<AlignType> cand_pos; cand_pos.resize(len);
  vector<AlignType> temp_pos;
  for(int i = 0; i < cand_pos.size(); ++ i) {
    //cout << "=======================" << endl;
    cand_pos[i].bwt_begin = 0; cand_pos[i].bwt_end = bwt_size;
    cand_pos[i].q_begin = cand_pos[i].q_end = len - i - 1; 
    ++ cand_pos[i].q_begin; // this is because we need to subtract 1 for search of the first character
    for(int j = 0; j <= i; ++ j) {
      if(cand_pos[j].bwt_begin >= cand_pos[j].bwt_end) continue;
      BWTIDX x, y; 
      bwt.UpdateRange(s[i], cand_pos[j].bwt_begin, cand_pos[j].bwt_end, x, y);
      if(x >= y) { // this is the end of this search, record the range
        if(cand_pos[j].q_end - cand_pos[j].q_begin + 1 >= min_len)  {
          //cout << "hit recorded" << endl;
          temp_pos.push_back(cand_pos[j]);
        }
      }
      cand_pos[j].q_begin --; cand_pos[j].bwt_begin = x; cand_pos[j].bwt_end = y;
      //cout << "index: " << i << "  " << j  << "  " << s[i] << " query location:  " << cand_pos[j].q_begin << "  " << cand_pos[j].q_end << " " << (long long int) cand_pos[j].bwt_begin << " " << (long long int) cand_pos[j].bwt_end << endl;
    }
  }
  // record the final identified ranges
  for(int i = 0; i < cand_pos.size(); ++ i) {
    if(cand_pos[i].bwt_begin < cand_pos[i].bwt_end && cand_pos[i].q_end - cand_pos[i].q_begin + 1 >= min_len)
      temp_pos.push_back(cand_pos[i]);
  }
  //cout << "size of fragments: " << temp_pos.size() << endl;
  if(temp_pos.size() <= 0)  return;
  // remove duplicates (such that no one range is a subrange of another)
  sort(temp_pos.begin(), temp_pos.end(), SortAlignTypeBySize);
  for(int i = 0; i < temp_pos.size() - 1; ++ i) {
    if(temp_pos[i].q_begin < 0) continue;
    for(int j = i + 1; j < temp_pos.size(); ++ j) {
      // if the ith range contains the jth range, invalidate the jth
      if(temp_pos[i].q_begin <= temp_pos[j].q_begin && temp_pos[i].q_end >= temp_pos[j].q_end)  
        temp_pos[j].q_begin = -1;
    }
  }
  // record all remaining non-redundant set
  for(int i = 0; i < temp_pos.size(); ++ i) {
    if(temp_pos[i].q_begin >= 0)
      all_pos.push_back(temp_pos[i]);
  }
  return;
}

void BWTSearch::Search(
    BWT &bwt, BWT &rev_bwt, 
    const char *str, AlignType &pos
) {
  int len = strlen(str);
  if(len <= 0)  {
    cerr << "BWTSearch::Search: Searching empty sequence. An empty list will be returned." << endl; return;
  }
  int *bound = new int [len];
  CalLowerBound(rev_bwt, str, bound);
  SearchInExact(bwt, rev_bwt, bound, str, pos);  
  AlignType prev_pos = pos;
  if(pos.q_begin >= 0)  {
      string rev_seq = str;
      rev_seq = string(rev_seq.rbegin(), rev_seq.rend());
      CalLowerBound(bwt, rev_seq.c_str(), bound);   
      SearchInExact(rev_bwt, bwt, bound, rev_seq.c_str(), pos);
  }
  int prev_span = prev_pos.q_end - prev_pos.q_begin + 1;
  int span = pos.q_end - pos.q_begin + 1;
  if(prev_span > span || (prev_span == span && prev_pos.cost < pos.cost)) { 
    pos = prev_pos;
  } else  {
    // recall that "pos" was the search result of the reverse BWT
    // need to flip the starting and ending positions for the query
    pos.q_end = len - pos.q_begin - 1; pos.q_begin = 0;
    // TODO: notice that the BWT indexes are for the reverse BWT
    // TODO: and should be converted to exact reference location before returning
  }
  delete [] bound;
  return;
}


void BWTSearch::Enqueue(
    std::priority_queue<ExtInfo, std::vector<ExtInfo>, ExtInfoComp> &candidate, 
    ExtInfo &phase_info
) {
  if(candidate.size() < max_queue_size || phase_info.cost <= candidate.top().cost)
    candidate.push(phase_info);
  return;
}

void BWTSearch::SearchInExact(
    BWT &bwt, BWT &rev_bwt, const int *bound,
    const char *str, AlignType &pos
) {
  
  int len = strlen(str);
  int lower_cost = cost;
  // construct initial stack 
  priority_queue<ExtInfo, std::vector<ExtInfo>, ExtInfoComp> candidate;
  ExtInfo init;
  init.range.first = 0; init.range.second = bwt.GetSize(); init.q_pos = len - 1; init.cost = 0;
  init.cost_bound = bound[len - 1];
  Enqueue(candidate, init);
  pos.bwt_begin = 0; pos.bwt_end = bwt.GetSize(); 
  pos.q_begin = pos.q_end = len - 1; pos.cost = cost;
  // extend each element in the stack with fast mode
  while(!candidate.empty()) {
    // record the best extension so par
    if(candidate.top().q_pos < pos.q_begin || 
        (candidate.top().q_pos == pos.q_begin && candidate.top().cost < pos.cost)
    ) { 
      pos.q_begin = candidate.top().q_pos + 1; pos.cost = candidate.top().cost;
      pos.bwt_begin = candidate.top().range.first; pos.bwt_end = candidate.top().range.second;
    }
       
    if(candidate.top().q_pos < 0)  {
      if(candidate.top().cost < lower_cost) lower_cost = candidate.top().cost;
      candidate.pop();
    } else 
      ExtendInExactFast(bwt, str, candidate, bound, lower_cost);
  }
  // if the entire sequence cannot be aligned, report the longest alignment within 
  // the allowed number of errors
  return;
}

void BWTSearch::ExtendInExactFast(
    BWT &bwt, const char *str,
    std::priority_queue<ExtInfo, std::vector<ExtInfo>, ExtInfoComp> &candidate, 
    const int *bound, int lower_cost
) {

  if(candidate.empty()) return;
  // get the top extension
  ExtInfo current = candidate.top();
  candidate.pop(); 
  if(current.q_pos < 0 || current.cost + bound[current.q_pos] > lower_cost)  return;
  // perform extension
  ExtInfo next;
  // tries to find if the next is a match
  next.range = bwt.UpdateRange(str[current.q_pos], current.range);
  if(next.range.first < next.range.second)  {
    next.q_pos = current.q_pos - 1; next.cost = current.cost;
    next.cost_bound = next.cost;
    if(next.q_pos >= 0) next.cost_bound += bound[next.q_pos];
    if(next.q_pos > 0)  {
      pair<BWTIDX, BWTIDX> peek_ahead = bwt.UpdateRange(str[current.q_pos - 1], next.range);
      if(peek_ahead.first < peek_ahead.second && next.cost_bound <= lower_cost) {
        Enqueue(candidate, next); return;
      }
    } else {
      // can't peek ahead, directly insert
      if(next.cost_bound <= lower_cost) {
        Enqueue(candidate, next); return;
      }
    }
  }
  // otherwise try all possible mismatch/insertion/deletions
  for(int i = 0; i < bwt.alphabet_.alphabet_size_; ++ i) {
    BWTCHAR c = bwt.alphabet_.GetInvCharMap(i);
    // refine the range
    next.range = bwt.UpdateRange(c, current.range);
    if(next.range.first >= next.range.second) continue;    
    // the match/mismatch case; subtract the index
    if(c == str[current.q_pos]) next.cost = current.cost;
    else next.cost = current.cost + m_cost;
    next.q_pos = current.q_pos - 1;
    next.cost_bound = next.cost;
    if(next.q_pos >= 0) next.cost_bound += bound[next.q_pos];
    if(next.cost_bound <= lower_cost) {
      Enqueue(candidate, next);
    }   
    // the insertion case (any character "c" is inserted into the sequence)
    // add gap cost, keep index
    next.cost = current.cost + g_cost;
    next.q_pos = current.q_pos;
    next.cost_bound = next.cost;
    if(next.q_pos >= 0) next.cost_bound += bound[next.q_pos];
    if(next.cost_bound <= lower_cost) {
      Enqueue(candidate, next);
    }
  }
  // the deletion case (skip current character, add gap cost)
  next = current;
  -- next.q_pos; next.cost += g_cost;
  next.cost_bound = next.cost;
  if(next.q_pos >= 0) next.cost_bound += bound[next.q_pos];
  if(next.cost_bound <= lower_cost) {
    Enqueue(candidate, next);
  }
  return;
}

void BWTSearch::CalLowerBound(BWT &rev_bwt, const char *str, int *bound) {
  int cost = 0;
  pair<BWTIDX, BWTIDX> range;
  range.first = 0; range.second = rev_bwt.GetSize();
  for(int i = 0; i < strlen(str); ++ i) {
    range = rev_bwt.UpdateRange(str[i], range);
    if(range.first >= range.second)  {
      range.first = 0; range.second = rev_bwt.GetSize();
      cost += m_cost;
    }
    bound[i] = cost;
  }
  return;
}

void BWTSearch::ComputeOverlapInfo(
    char **seqs, const int rid, const int target_id_begin, BWT &bwt, const int min_len,
    std::vector<std::vector<TargetOverlapType> *> *extension, std::vector<bool> *contained
) {
  //cout << "processing sequence: " << seqs[rid] << endl;
  // the the current read is contained by some other reads detected in previous searches, skip
  if((*contained)[rid]) return;
  string s = seqs[rid];
  int n = s.length();
  // if the total length of the read is less than the minimum overlap, return
  if(n < min_len) return;
  // initialize the search for the overlapping region
  pair<BWTIDX, BWTIDX> fw_range, fw_range_terminal;
  fw_range.first = 0; 
  fw_range.second = bwt.GetSize();
  int i;
  for(i = 0; i < min_len; ++ i) {
    //cout << "search sequence: " << &seq[n - i - 1] << endl;
    char c = s[n - i - 1];
    BWTIDX occbegin = bwt.CountOccurrence(c, fw_range.first);
    BWTIDX occend = bwt.CountOccurrence(c, fw_range.second);
    //cout << "initial range: " << fw_range.first << "  " << fw_range.second << endl;
    //cout << "search occurrence:  " << c << " " << occbegin << "  " << occend << endl;
    if(occbegin >= occend)  break;
    //cout << "lexicographical smaller: " << search_info.bwtF_->CountLexicoLess(c, fw_range.first) << "  " << search_info.bwtF_->CountLexicoLess(c, fw_range.second) << endl;
    int c_id = bwt.alphabet_.GetCharMap(c);
    fw_range.first = bwt.acc_freq_[c_id + 1] + occbegin;
    fw_range.second = bwt.acc_freq_[c_id + 1] + occend;
    //cout << "updated range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
  }
  // if no sequence overlap for the given minumum overlap length, return
  if(i < min_len) return;
  // otherwise also try to search the delimitor to detect begin intervals
  // recall that i is less than n - 1 because we do not want the read itself
  // or other wise the read is contained by other reads
  vector<pair<BWTIDX, BWTIDX> > range_holder;
  vector<int> len_holder;
  int covered_range = min_len;
  for(i = min_len; i < n; ++ i) {
    // extend the sequence
    //cout << "search sequence: " << &seq[n - i - 1] << " " << n << " " << i << endl;
    //cout << "initial range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
    char c = s[n - i - 1];
    BWTIDX occbegin = bwt.CountOccurrence(c, fw_range.first);
    BWTIDX occend = bwt.CountOccurrence(c, fw_range.second);
    //cout << "search occurrence:  " << c << " " << occbegin << "  " << occend << endl;
    //cout << "lexicographical smaller: " << search_info.bwtF_->CountLexicoLess(c, fw_range.first) << "  " << search_info.bwtF_->CountLexicoLess(c, fw_range.second) << endl;
    if(occbegin >= occend)  break;
    int c_id = bwt.alphabet_.GetCharMap(c);
    fw_range.first = bwt.acc_freq_[c_id + 1] + occbegin;
    fw_range.second = bwt.acc_freq_[c_id + 1] + occend;
    covered_range = i;
    // also search for the delimitor (with the updated fw_range and re_range)
    //cout << "initial terminal range: " << fw_range.first << "  " << fw_range.second << endl;
    occbegin = bwt.CountOccurrence(DELIM, fw_range.first);
    occend = bwt.CountOccurrence(DELIM, fw_range.second);
    //cout << "search terminal occurrence:  " << occbegin << "  " << occend << endl;
    if(occbegin >= occend)  continue;
    // note that delimitor is the lexico-smallest char, 
    // no need to add CountLexicoLess nor acc_freq
    fw_range_terminal.first = occbegin;
    fw_range_terminal.second = occend;
    // interpret such interval and record the read IDs
    range_holder.push_back(fw_range_terminal);
    len_holder.push_back(i + 1);
    //cout << "overlap recorded !!!" << endl;
    //cout << "overlap  " << i + 1 << endl;
  }
  if(range_holder.size() < 1) return;
  //cout << "start here:  " << rid << endl;
  // the read is contained, record it and terminate
  //cout << "non-terminal range:  " << fw_range.first << "  " << fw_range.second << endl;
  if(covered_range >= n - 1)  {
    for(BWTIDX j = fw_range.first; j < fw_range.second; ++ j)  {
      BWTINT tid, tpos;
      bwt.GetRefLocation(j, tid, tpos); 
      tid += target_id_begin; // notice that this is not terminal, so do not need to add 1 to the target ID
      if(rid == tid) continue;
      if(strlen(seqs[tid]) > s.length() || (strlen(seqs[tid]) == s.length() && !(*contained)[tid]) )  {
        // this read is contained
        (*contained)[rid] = true; 
          
        //if(rid == 7641357 || rid == 3356853)  {
        //  cout << "mark contained by ComputeOverlapInfo" << endl;
        //  cout << tid << "  " << seqs[tid] << endl;
        //} 
        
        return;
      }
    }  
  }
  //cout << "good here  " << rid << endl;
  // record the overlap information
  vector<TargetOverlapType> overlaps;
  for(int range_id = 0; range_id < range_holder.size(); ++ range_id) {
    if(covered_range >= n - 1 && range_id >= range_holder.size() - 1) continue;
    int o_len = len_holder[range_id];
    // note that the last range correspond to the entire sequence, which has been considered previously
    //cout << "range: " << range_holder[range_id].first << "  " << range_holder[range_id].second << endl;
    for(BWTIDX j = range_holder[range_id].first; j < range_holder[range_id].second; ++ j) {
      TargetOverlapType t; BWTINT foo;
      bwt.GetRefLocation(j, t.rid, foo); 
      t.rid += target_id_begin + (foo > 0); // note that the terminal correspond to the start of the next sequence (except for the first sequence)
      if((*contained)[t.rid]) continue;

      //cout << "target id: " << t.rid << endl;
      //cout << "foo: " << foo << endl;
      t.overlap_len = (BWTSHORT) o_len;
      //t.edge_len = strlen(seqs[rid]) + strlen(seqs[t.rid]) - o_len;
      if(rid != t.rid)  overlaps.push_back(t);
    }
  } 
  //cout << "good2 here  " << rid << endl;
  for(int j = 0; j < (*extension)[rid]->size(); ++ j) {
    BWTINT tid = (*(*extension)[rid])[j].rid;
    if(!(*contained)[tid]) overlaps.push_back((*(*extension)[rid])[j]);
  }
  // find all irriducible edges
  //cout << "before" << endl;
  //cout << "source:  " << s << endl;
  //for(int j = 0; j < overlaps.size(); ++ j) {
  //  cout << "target:  " << seqs[overlaps[j].rid] << endl;
  //}
  RemoveReducibleEdges(seqs, rid, contained, overlaps);

  //cout << "after" << endl;
  //cout << "source:  " << s << endl;
  //for(int j = 0; j < overlaps.size(); ++ j) {
  //  cout << "target:  " << seqs[overlaps[j].rid] << endl;
  //}
  // update the extension information
  (*extension)[rid]->clear();
  for(int j = 0; j < overlaps.size(); ++ j) {
    (*extension)[rid]->push_back(overlaps[j]);
  }
  /*
  for(BWTIDX j = fw_range_terminal.first; j < fw_range_terminal.second; ++ j) {
    TargetOverlapType t; BWTINT foo;
    bwt.GetRefLocation(j, t.rid, foo); t.rid += target_id_begin;
    t.overlap_len = i + 1;
    //t.edge_len = strlen(seqs[rid]) + strlen(seqs[t.rid]) - (i + 1);
    if(rid != t.rid)  {
      (*extension)[rid]->push_back(t);
    }
    //cout << "target location: " << t.rid << " " << foo << endl;
    //if(rid != overlap[rid][idx].rid)  ++ idx;
  }
  */
  //overlap_count[rid] = overlap_holder.size();
  //overlap[rid] = new TargetOverlapType [overlap_holder.size()];
  //for(i = 0; i < overlap_holder.size(); ++ i) {
  //  overlap[rid][i] = overlap_holder[i];
  //}  
  //cout << "before return: " << rid << endl;
  return;
}


bool SortFullTargetOverlapType(const FullTargetOverlapType &a, const FullTargetOverlapType &b)  {
  if(a.edge_len < b.edge_len || (a.edge_len == b.edge_len && a.overlap_len > b.overlap_len) || 
    (a.edge_len == b.edge_len && a.overlap_len == b.overlap_len && a.rid > b.rid)    
  )  {
    return 1;
  }
  return 0;
}

void BWTSearch::RemoveReducibleEdges(
    char **seqs, const int rid,
    std::vector<bool> *contained, std::vector<TargetOverlapType> &overlaps
)  {
  int i, j, k;  
  vector<FullTargetOverlapType> full_overlaps; full_overlaps.resize(overlaps.size());
  for(i = 0; i < overlaps.size(); ++ i) { 
    full_overlaps[i].rid = overlaps[i].rid;
    full_overlaps[i].overlap_len = overlaps[i].overlap_len;
    full_overlaps[i].edge_len = strlen(seqs[rid]) + strlen(seqs[full_overlaps[i].rid]) - full_overlaps[i].overlap_len; 
  }
  // sort the overlaps
  sort(full_overlaps.begin(), full_overlaps.end(), SortFullTargetOverlapType);
  int max_len = full_overlaps[full_overlaps.size() - 1].edge_len - strlen(seqs[rid]);
  // detect possible contained reads
  
  vector<FullTargetOverlapType> part_init;  
  for(i = 0; i < full_overlaps.size(); ++ i) {
    if(full_overlaps[i].edge_len == strlen(seqs[rid]))  {
      (*contained)[full_overlaps[i].rid] = true;
      
      //if(rid == 7641357 || rid == 3356853)  {
      //  cout << "mark contained by RemoveReducibleEdges" << endl;
      //}

    } else  {
      part_init.push_back(full_overlaps[i]);
    }
  }
  queue<vector<FullTargetOverlapType> > part_queue; part_queue.push(part_init);
  queue<int> len_queue; len_queue.push(0);
  vector<FullTargetOverlapType> record_overlaps;
  // search all sequences
  while(!part_queue.empty()) {
    vector<FullTargetOverlapType> current = part_queue.front(); part_queue.pop();
    int current_len = len_queue.front(); len_queue.pop();

    //if(rid == 3356853)  {
    //  cout << "current len: " << current_len << endl;
    //  for(k = 0; k < current.size(); ++ k) {
    //    cout << "seqs:  " << seqs[current[k].rid] << "  " << current[k].overlap_len << endl;
    // }
    //}
    // if current parition is empty, continue
    if(current.size() <= 0) continue;
    // if current partiion size is 1, this is an irredicible edge, record and continue
    if(current.size() == 1) {
      record_overlaps.push_back(current[0]); continue;
    }
    // if the current parition has multiple reads but reaches the boundary of one read
    // take the first one as irreducible, the rest ones are reducible
    if(current_len + current[0].overlap_len >= strlen(seqs[current[0].rid]))  {
      record_overlaps.push_back(current[0]); continue;
    }
    // if none of these happens, we need to redistribute the reads into further partitions
    unordered_map<char, vector<FullTargetOverlapType> > redist;
    for(j = 0; j < current.size(); ++ j) {
      char c = seqs[current[j].rid][current[j].overlap_len + current_len];
      redist[c].push_back(current[j]);
      //cout << "checking character:  " << c << " " << j << endl;
    }
    for(auto it = redist.begin(); it != redist.end(); ++ it) {
      part_queue.push(it->second);
      len_queue.push(current_len + 1);
    }
  }
  // record the non-reducible edges
  overlaps.resize(record_overlaps.size());
  for(i = 0; i < record_overlaps.size(); ++ i) { 
    overlaps[i].rid = record_overlaps[i].rid;
    overlaps[i].overlap_len = record_overlaps[i].overlap_len;
  }
  return;
}

// given a read of interest and minimum overlap, find all intervals (in both fw and re FM-indexes) 
// corresponding to the prefix of the reads that perfectly overlap with the given read
void BWTSearch::SearchBeginIntervals(const char* seq, const int min_len, IvInfo &search_info) {
  int n = strlen(seq);
  // if the total length of the read is less than the minimum overlap, return
  if(n < min_len) return;
  // initialize the search for the overlapping region
  pair<BWTIDX, BWTIDX> fw_range, re_range, fw_range_terminal, re_range_terminal;
  fw_range.first = re_range.first = 0; 
  fw_range.second = re_range.second = search_info.bwtF_->GetSize();
  int i;
  for(i = 0; i < min_len - 1; ++ i) {
    //cout << "search sequence: " << &seq[n - i - 1] << endl;
    char c = seq[n - i - 1];
    BWTIDX occbegin = search_info.bwtF_->CountOccurrence(c, fw_range.first);
    BWTIDX occend = search_info.bwtF_->CountOccurrence(c, fw_range.second);
    //cout << "initial range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
    //cout << "search occurrence:  " << c << " " << occbegin << "  " << occend << endl;
    if(occbegin >= occend)  break;
    //cout << "lexicographical smaller: " << search_info.bwtF_->CountLexicoLess(c, fw_range.first) << "  " << search_info.bwtF_->CountLexicoLess(c, fw_range.second) << endl;
    re_range.first = re_range.first
        + search_info.bwtF_->CountLexicoLess(c, fw_range.second)
        - search_info.bwtF_->CountLexicoLess(c, fw_range.first); 
    re_range.second = re_range.first + occend - occbegin;
    int c_id = search_info.bwtF_->alphabet_.GetCharMap(c);
    fw_range.first = search_info.bwtF_->acc_freq_[c_id + 1] + occbegin;
    fw_range.second = search_info.bwtF_->acc_freq_[c_id + 1] + occend;
    //cout << "updated range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
  }
  // if no sequence overlap for the given minumum overlap length, return
  if(i < min_len - 1) return;
  // otherwise also try to search the delimitor to detect begin intervals
  // recall that i is less than n - 1 because we do not want the read itself
  // or other wise the read is contained by other reads
  for(i = min_len - 1; i < n - 1; ++ i) {
    // extend the sequence
    //cout << "search sequence: " << &seq[n - i - 1] << " " << n << " " << i << endl;
    //cout << "initial range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
    char c = seq[n - i - 1];
    BWTIDX occbegin = search_info.bwtF_->CountOccurrence(c, fw_range.first);
    BWTIDX occend = search_info.bwtF_->CountOccurrence(c, fw_range.second);
    //cout << "search occurrence:  " << c << " " << occbegin << "  " << occend << endl;
    //cout << "lexicographical smaller: " << search_info.bwtF_->CountLexicoLess(c, fw_range.first) << "  " << search_info.bwtF_->CountLexicoLess(c, fw_range.second) << endl;
    if(occbegin >= occend)  break;
    re_range.first = re_range.first
        + search_info.bwtF_->CountLexicoLess(c, fw_range.second)
        - search_info.bwtF_->CountLexicoLess(c, fw_range.first);
    re_range.second = re_range.first + occend - occbegin;
    int c_id = search_info.bwtF_->alphabet_.GetCharMap(c);
    fw_range.first = search_info.bwtF_->acc_freq_[c_id + 1] + occbegin;
    fw_range.second = search_info.bwtF_->acc_freq_[c_id + 1] + occend;
    // also search for the delimitor (with the updated fw_range and re_range)
    //cout << "initial terminal range: " << fw_range.first << "  " << fw_range.second << "  " << re_range.first << "  " << re_range.second << endl;
    occbegin = search_info.bwtF_->CountOccurrence(DELIM, fw_range.first);
    occend = search_info.bwtF_->CountOccurrence(DELIM, fw_range.second);
    //cout << "search terminal occurrence:  " << occbegin << "  " << occend << endl;
    if(occbegin >= occend)  continue;
    // note that delimitor is the lexico-smallest char, 
    // no need to add CountLexicoLess nor acc_freq
    re_range_terminal.first = re_range.first; 
    re_range_terminal.second = re_range.first + occend - occbegin;
    fw_range_terminal.first = occbegin;
    fw_range_terminal.second = occend;
    // record such interval
    search_info.intervals_.PushIV(
        fw_range_terminal.first, fw_range_terminal.second, 
        re_range_terminal.first, re_range_terminal.second, 
        i + 1, i + 1
    );
    
    //cout << "overlap recorded !!!" << endl;
  }
  return;
}

bool BWTSearch::UpdateForwardBackward(
    IvInfo &search_info, const char c, 
    std::pair<BWTIDX, BWTIDX> &fw_range, std::pair<BWTIDX, BWTIDX> &re_range
) {
  BWTIDX occbegin = search_info.bwtF_->CountOccurrence(c, fw_range.first);
  BWTIDX occend = search_info.bwtF_->CountOccurrence(c, fw_range.second);
  if(occbegin >= occend)  return false; // remember the pivot sequence also exists in the BWT, so it should at least hit itself
  re_range.first = re_range.first
      + search_info.bwtF_->CountLexicoLess(c, fw_range.second)
      - search_info.bwtF_->CountLexicoLess(c, fw_range.first); 
  re_range.second = re_range.first + occend - occbegin;
  int c_id = search_info.bwtF_->alphabet_.GetCharMap(c);
  fw_range.first = search_info.bwtF_->acc_freq_[c_id + 1] + occbegin;
  fw_range.second = search_info.bwtF_->acc_freq_[c_id + 1] + occend;
  return true;
}

void BWTSearch::SearchBeginIntervals_V2(const char* seq, const int min_len, IvInfo &search_info) {
  int n = strlen(seq);
  // if the total length of the read is less than the minimum overlap, return
  if(n < min_len) return;
  // initialize the search for the overlapping region
  //search_info.bwtF_->PrintBWT();
  //search_info.bwtR_->PrintBWT();
  //cout << "pivor sequence:  " << seq << endl;
  queue<FwReSearchType> candidates;
  FwReSearchType init;
  init.fw_range.first = init.re_range.second = 0;
  init.fw_range.second = init.re_range.second = search_info.bwtF_->GetSize();
  init.search_cost = 0; init.pivot_len = 0; init.db_len = 0;
  candidates.push(init);
  queue<FwReSearchType> passed;
  while(!candidates.empty()) {
    FwReSearchType current = candidates.front(); candidates.pop();
    //cout << "Current info:  " << current.s << " " << current.search_cost << "  " << current.pivot_len << "  " << current.db_len << "  " << current.fw_range.first << "  " << current.fw_range.second << "  " << current.re_range.first << "  " << current.re_range.second << endl;
    // if the score drops below the threshold, skip the search
    if(current.search_cost > cost) {
      //cout << "cost over threshold" << endl;      
      continue;
    }
    // if the length satisfy the minimum length, record the search as passed
    if(current.pivot_len >= min_len)  {
      //cout << "min length reached" << endl;
      passed.push(current); continue;
    }
    char c = seq[n - current.pivot_len - 1];
    //cout << "current character: " << c << endl;
    pair<BWTIDX, BWTIDX> fw_range_record = current.fw_range;
    pair<BWTIDX, BWTIDX> re_range_record = current.re_range;
    //string s_record = current.s;
    bool success = UpdateForwardBackward(search_info, c, current.fw_range, current.re_range);
    if(success && (current.search_cost > 0 || current.fw_range.second - current.fw_range.first > 1))  {
      // record the search
      //current.s += c;
      current.pivot_len ++; current.db_len ++; candidates.push(current); 
      //cout << "pushed match" << endl;
    }   
    // rollback to the previous information    
    current.fw_range = fw_range_record; current.re_range = re_range_record; 
    current.pivot_len --; current.db_len --;
    //current.s = s_record;
    // insertion in the query
    FwReSearchType next_ins = current;
    next_ins.pivot_len ++; next_ins.search_cost += g_cost;  // do not need to increase db_len
    if(next_ins.search_cost <= cost) {
      candidates.push(next_ins);
      //cout << "pushed insertion" << endl;
    }
    // mutation and deletion in the query, try all possible characters in the alphabet
    for(int i = 0; i < search_info.bwtF_->alphabet_.alphabet_size_; ++ i) {
      char ca = search_info.bwtF_->alphabet_.GetInvCharMap(i);
      //cout << "current character all test:  " << ca << endl;
      // refine the range
      FwReSearchType next = current;
      success = UpdateForwardBackward(search_info, ca, next.fw_range, next.re_range);
      if(success) {
        //next.s += ca;
        // deletion case; do not increase pivot_len but need to increase db_len, and add gap cost
        next.search_cost += g_cost; next.db_len ++; 
        if(next.search_cost <= cost && next.db_len > 1) {
          candidates.push(next);
          //cout << "pushed deletion: " << ca << endl;
        }
        // mutation case; add length and add mutation cost (remember to rollback the g_cost)
        if(ca != c)  {
          // do not increase db_len because it has already been increase in the calculation of deletion case
          next.pivot_len ++; next.search_cost += m_cost - g_cost; 
          if(next.search_cost <= cost) candidates.push(next);
          //cout << "pushed mutation: " << ca << endl;
        }
        
      } 
    }
  }  

  // continue searching all intervals that have been passed; also search for terminal character
  while(!passed.empty()) {
    FwReSearchType current = passed.front(); passed.pop();
    //cout << "In passed Current info:  " << current.s << " " << current.search_cost << "  " << current.pivot_len << "  " << current.db_len << "  " << current.fw_range.first << "  " << current.fw_range.second << "  " << current.re_range.first << "  " << current.re_range.second << endl;
    // if the score drops below the threshold, skip the search
    if(current.search_cost > cost) continue;
    // if the length satisfy the minimum length, record the search as passed
    if(current.pivot_len >= n)   continue;

    // check if this is a terminal
    pair<BWTIDX, BWTIDX> fw_range_record = current.fw_range;
    pair<BWTIDX, BWTIDX> re_range_record = current.re_range;
    bool success = UpdateForwardBackward(search_info, DELIM, current.fw_range, current.re_range);
    if(success && (current.search_cost > 0 || current.fw_range.second - current.fw_range.first > 1)) {
      // record the interval
      if(!search_info.intervals_.IsRedundantA(current.fw_range.first, current.fw_range.second)) {
        search_info.intervals_.PushIV(
            current.fw_range.first, current.fw_range.second,
            current.re_range.first, current.re_range.second, 
            current.pivot_len, current.db_len
        );
        //cout << "Overlap detected:  " << current.fw_range.first << " " << current.fw_range.second << "  " << current.re_range.first << " " << current.re_range.second << endl;
      }
    }
    // rollback the range
    current.fw_range = fw_range_record;
    current.re_range = re_range_record;

    // attempt to further extend
    char c = seq[n - current.pivot_len - 1];
    //cout << "current character: " << c << endl;
    fw_range_record = current.fw_range;
    re_range_record = current.re_range;
    //string s_record = current.s;
    success = UpdateForwardBackward(search_info, c, current.fw_range, current.re_range);
    if(success)  {
      // record the search
      //current.s += c;
      current.pivot_len ++; current.db_len ++; passed.push(current);
      //cout << "match found!!!" << endl;
    }
    // roll back to the previous information    
    current.fw_range = fw_range_record; current.re_range = re_range_record; 
    current.pivot_len --; current.db_len --; 
    //current.s = s_record;
    // insertion in the query
    FwReSearchType next_ins = current;
    next_ins.pivot_len ++; next_ins.search_cost += g_cost; // do not need to increase db_len
    //cout << "New insertion cost:  " << next_ins.search_cost << endl;
    if(next_ins.search_cost <= cost) {
      passed.push(next_ins);
      //cout << "pushed insertion" << endl;
    }
    // mutation and deletion in the query, try all possible characters in the alphabet
    for(int i = 0; i < search_info.bwtF_->alphabet_.alphabet_size_; ++ i) {
      char ca = search_info.bwtF_->alphabet_.GetInvCharMap(i);
      // refine the range
      //cout << "tested chracater:  " << ca << endl;
      FwReSearchType next = current;
      success = UpdateForwardBackward(search_info, ca, next.fw_range, next.re_range);
      if(success) {
        //next.s += ca;
        // deletion case; do not change length and add gap cost
        next.search_cost += g_cost; next.db_len ++; 
        //cout << "New deletion cost:  " << next.search_cost << endl;
        if(next.search_cost <= cost && next.db_len > 1)  passed.push(next);
        //cout << "pushed deletion" << endl;
        // mutation case; add length and add mutation cost (remember to rollback the g_cost)
        if(ca != c)  {
          next.pivot_len ++; next.search_cost += m_cost - g_cost; 
          //cout << "New mutation cost:  " << next.search_cost << endl;
          if(next.search_cost <= cost) passed.push(next);
          //cout << "pushed mutation" << endl;
        }
      } 
    }
  }
  return;
}

void BWTSearch::FindIrreducible(
    IvInfo &search_info, std::vector<BWTIDX> &ir_positions, std::vector<int> &ir_overlap
) {  
  if(search_info.intervals_.GetSize() <= 0)  return;
  search_info.intervals_.Sort();
  // check boundary conditions
  if(!search_info.intervals_.Check()) {
    cout << "Warning: corrupted intervals, no irreducible edges can be detected." << endl;
    return;
  }
  // the stack contains all intervals that ends with the same sequences
  stack<IvSet> candidates;
  candidates.push(search_info.intervals_);
  //cout << "size of candidate ranges:  " << candidates.size() << endl;
  // recursively check each intervals
  bool used_char[256];  
  while(!candidates.empty()) {
    //cout << "============ handling each interval group ===============  " << candidates.size() << endl;
    IvSet current = candidates.top(); candidates.pop();
    //for(int h = 0; h < current.GetSize(); ++ h) {
    //  cout << "search_info " << h << ": " << current.len_[h] << "  " << current.ivA_[h].first << " " << current.ivA_[h].second << "  " << current.ivB_[h].first << " " << current.ivB_[h].second << endl;
    //}
    // check all presented characters in the alphabet
    // note that for the first time we do not check "$"-extension because it would
    // mean that the extension read is contained
    int i, j, k, n = current.GetSize();
    memset(used_char, 0, 256 * sizeof(bool));
    for(i = 0; i < n; ++ i) { // for each interval
      // for each character in the alphabet (look at the reverse BWT)
      for(k = current.iv_list_[i].ivB_.first; k < current.iv_list_[i].ivB_.second; ++ k) { 
        //cout << "Interval info: " << k << ": " <<  current.len_[i] << "  " << current.ivA_[i].first << " " << current.ivA_[i].second << "  " << current.ivB_[i].first << " " << current.ivB_[i].second << endl;
        char c = (char) search_info.bwtR_->bwt_[k]; // the kth char in the reverse BWT string
        //cout << "Character to be searched:  " << c << endl;
        // check if the character has been tested
        if(c == DELIM || used_char[c])  continue;
        used_char[c] = true;
        IvSet next;
        // try to update the intervals by appending such character
        for(j = i; j < n; ++ j) {
          // try to append the character
          pair<BWTIDX, BWTIDX> r1 = search_info.bwtR_->UpdateRange(c, current.iv_list_[j].ivB_);
          // try to check if the read ends after appending the character
          pair<BWTIDX, BWTIDX> r2 = search_info.bwtR_->UpdateRange(DELIM, r1); 
          //cout << "phase: " << j << ": " << r1.first << " " << r1.second << " " << r2.first << "  " << r2.second << endl; 
          // if the first read leads to a termination, we found an irreducible read
          // in cases where multiple reads end at the same time, take the first position
          // terminate current loop
          if(j == i && r1.second - r1.first == r2.second - r2.first)  {
            //cout << "irreducible read found!!!  " << current.pivot_len_[j] << " " << current.db_len_[j] << endl;
            ir_positions.push_back(r2.first); 
            ir_overlap.push_back(current.iv_list_[j].db_len_);
            break;
          }
          // for other reads that do not terminate, add to interval set next
          if(r1.second - r1.first >= 1 && r2.second - r2.first <= 0)  {
            // we only need to record intervals at the reverse BWT
            //cout << "read extension recorded!!!" << endl;
            next.PushIV(
              -1, -1, r1.first, r1.second, 
              current.iv_list_[j].pivot_len_, current.iv_list_[j].db_len_
            );
          }
        }
        if(next.GetSize() > 0)  candidates.push(next);
      }
    }
    //cout << "============ done handling current interval group ===============  " << candidates.size() << endl;
  }
  return;
}

bool BWTSearch::IsContainedRead(const char* seq, BWT &bwt, AlignType &pos)  {
  // constructing the extended string
  int n = strlen(seq);
  char *extended_seq = new char [n + 3];
  extended_seq[0] = DELIM;
  strcpy(&extended_seq[1], seq);
  extended_seq[n + 1] = DELIM; extended_seq[n + 2] = '\0';
  // searching the entire sequence with and without delimitor against the BWT
  SearchExact(bwt, seq, pos);
  int r1 = pos.bwt_end - pos.bwt_begin;
  SearchExact(bwt, extended_seq, pos);
  int r2 = pos.bwt_end - pos.bwt_begin;
  delete [] extended_seq;
  if(r2 == r1)  return false;
  return true;
}

