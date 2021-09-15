#include "graph.h"

// AssemblyGraph::AssemblyGraph(const std::string& graph_file, const std::string& read_info_file=nullptr)
// {
//
// }

int main(int argc, char* argv[])
{
  AssemblyGraph* my_graph = new AssemblyGraph();
  my_graph->readGraphFromAsqg("transitive.example.asqg");
  // my_graph->showGraph();
  my_graph->condense();
  delete my_graph;
  return 0;
}

bool AssemblyGraph::addNode(const vertexType& v)
{
  if(vertex_map_.count(v.id_) == 1) return false;
  BoostVertex tmp_v = boost::add_vertex(v, *p_graph_);
  vertex_map_[v.id_] = tmp_v;
  return true;
}

bool AssemblyGraph::addEdge(const IDType_AG&  source, const IDType_AG&  target, const edgeType&  e)
{
  if(vertex_map_.count(source) == 0 || vertex_map_.count(target) == 0) return false;
  BoostVertex s = vertex_map_[source];
  BoostVertex t = vertex_map_[target];
  std::pair<BoostEdge, bool> e_search = boost::add_edge(s, t, e, *p_graph_);
  if(e_search.second) return true;
  std::cout << "Error:: [AssemblyGraph::readGraphFromAsqg]: Failed to add edges (" << source << ", " << target << ")!\n";
  return false;
}

bool AssemblyGraph::readGraphFromAsqg(const std::string& filename)
{
  std::ifstream fin;
  fin.open(filename.c_str());	// open file

  StringVector_AG seq_vector;
  if (!fin.is_open())	    // fail to open
  {
    std::cout << "Error: " << filename << " doesnot exist!\n";
    return false;
  }
  else
  {
    std::string line;
    boost::char_separator<char> sep(" \t");
    boost::tokenizer<boost::char_separator<char> >::iterator it;
    /** timer **/
    clock_t t;
    t = clock();
    /***********/
    while(std::getline(fin, line))
    {
      if(line[0] == 'V')
      {
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        it = tokens.begin();            // cont[0], VT

        ++it;                           // cont[1], read name
        read_name_map_[(*it)] = order_;
        order_++;

        ++it;                           // cont[2], sequence
        seq_vector.push_back(*it);
      }
      if(line[0] == 'E')      break;
    }
    /** timer **/
    t = clock() - t;
    std::cout << "VT takes " << ((double)t)/CLOCKS_PER_SEC << " s\n";
    t = clock();
    /***********/

    do
    {
      if(line[0] == 'E')  // edge, sperate by '\t' and ' '
      {
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        it = tokens.begin();            // cont[0], ED
        ++it;                           // cont[1], read1 name
        int source_id = read_name_map_[(*it)];
        ++it;                           // cont[2], read2 name
        int target_id = read_name_map_[(*it)];
        ++it;                           // cont[3], A
        int a = std::stoi(*it);
        ++it;                           // cont[4], B
        int b = std::stoi(*it);
        ++it;                           // cont[5], l1
        int l1 = std::stoi(*it);
        ++it;                           // cont[6], C
        int c = std::stoi(*it);
        ++it;                           // cont[7], D
        int d = std::stoi(*it);
        ++it;                           // cont[8], l2
        int l2 = std::stoi(*it);
        ++it;                           // cont[9], "1" == RC
        bool ff_RC = (*it == "1");

        SeqLenType_AG overlap = b - a + 1;
        /* add or locate the two vertices in the graph */
        if(vertex_map_.count(source_id) == 0)
        {
          vertexType node(source_id);
          node.len_ = l1;
          node.p_seq_ = seq_vector[source_id].c_str();
          addNode(node);
        }
        if(vertex_map_.count(source_id + order_) == 0)
        {
          vertexType node(source_id + order_);
          node.len_ = l1;
          addNode(node);
        }
        if(vertex_map_.count(target_id) == 0)
        {
          vertexType node(target_id);
          node.len_ = l2;
          node.p_seq_ = seq_vector[target_id].c_str();
          addNode(node);
        }
        if(vertex_map_.count(target_id + order_) == 0)
        {
          vertexType node(target_id + order_);
          node.len_ = l2;
          addNode(node);
        }

        edgeType edge(overlap);
        if(source_id != target_id)
        {
          /* add edge */
          if(ff_RC)         // r2 is reversed
          {
            if(a != 0)      // r1->r2
            {
              /* r1 -> r2' */
              addEdge(source_id, target_id + order_, edge);
              /* r2 -> r1' */
              addEdge(target_id, source_id + order_, edge);

            }
            else            // r2->r1
            {
              /* r2' -> r1 */
              addEdge(target_id + order_, source_id, edge);
              /* r1' -> r2 */
              addEdge(source_id + order_, target_id, edge);
            }
          }
          else
          {
            if(a != 0)     // r1->r2
            {
              /* r1 -> r2 */
              addEdge(source_id, target_id, edge);
              /* r2' -> r1' */
              addEdge(target_id + order_, source_id + order_, edge);
            }
            else            // r2->r1
            {
              /* r2 -> r1 */
              addEdge(target_id, source_id, edge);
              /* r1' -> r2' */
              addEdge(source_id + order_, target_id + order_, edge);
            }
          }
        }
      }
    }
    while(std::getline(fin, line));
    /** timer **/
    t = clock() - t;
    std::cout << "ED takes " << ((double)t)/CLOCKS_PER_SEC << " s\n";
    /***********/

  }
  fin.clear();
  fin.close();
  return true;
}

void AssemblyGraph::showGraph()
{
  std::cout << "vertices:\n";
  for(auto it = boost::vertices(*p_graph_).first; it != boost::vertices(*p_graph_).second; ++it)
  {
    std::cout << (*p_graph_)[*it].id_ << "("<< (*p_graph_)[*it].len_ << ")";
    if( (*p_graph_)[*it].id_ < order_ ) { std::cout << ": " << (*p_graph_)[*it].p_seq_; }
    std::cout << "\n";
  }
  std::cout << "\n";

  std::cout << "edges:\n";
  for(auto it_e = boost::edges(*p_graph_).first; it_e != boost::edges(*p_graph_).second; ++it_e)
  {
    BoostVertex s = boost::source(*it_e, *p_graph_);
    BoostVertex t = boost::target(*it_e, *p_graph_);
    std::cout << "Edge: ("<< (*p_graph_)[s].id_ << ", " << (*p_graph_)[t].id_ << "): " << (*p_graph_)[*it_e].op_ << "\n";
  }
  std::cout << "\n";
}

void AssemblyGraph::condense()
{
  /** timer **/
  clock_t t;
  /***********/
  /* find seed vertex */
  t = clock();
  BooleanVector_AG p_traversed = BooleanVector_AG(2*order_, false);
  BooleanVector_AG p_crossed   = BooleanVector_AG(2*order_, false);
  BooleanVector_AG ff_seed     = BooleanVector_AG(2*order_, false);
  for(auto it = boost::vertices(*p_graph_).first; it != boost::vertices(*p_graph_).second; ++it)
  {
    if(boost::in_degree(*it, *p_graph_) <= 0)
    { // orphant read is not seed
      if(boost::out_degree(*it, *p_graph_) > 0)   ff_seed[(*p_graph_)[*it].id_] = true;
    }
    else
    {
      if(boost::out_degree(*it, *p_graph_) != 1)  ff_seed[(*p_graph_)[*it].id_] = true;
      if(boost::in_degree(*it, *p_graph_) > 1)    ff_seed[(*p_graph_)[*it].id_] = true;
    }
  }
  t = clock() - t;
  std::cout << "find seed takes " << ((double)t)/CLOCKS_PER_SEC << " s\n";


  /* for each component */
  std::list<cycle_s> to_add_cycle;
  int cnt_cycle_node = 0;
  double condeseSE = 0, condenseCycle = 0, findComponent = 0;
  BooleanVector_AG connected = BooleanVector_AG(2*order_, false);
  for(auto it = boost::vertices(*p_graph_).first; it != boost::vertices(*p_graph_).second; ++it)
  {
    if(! p_crossed[ (*p_graph_)[*it].id_ ]) // untraversed vertex, new component
    {
      /* find connected component and source_edges */
      t = clock();
      std::stack<BoostVertex> to_visit;
      to_visit.push(*it);
      std::list<BoostVertex> this_component;
      int cnt_vertex = 0;
      while(!to_visit.empty())
      {
        BoostVertex top_vertex = to_visit.top();
        to_visit.pop();
        int rid = (*p_graph_)[top_vertex].id_;
        if(!connected[rid])
        {
          connected[rid] = true;
          this_component.push_back(top_vertex);
          ++cnt_vertex;
          p_crossed[rid] = true;

          for(auto it_v = boost::adjacent_vertices(top_vertex, *p_graph_).first; it_v != boost::adjacent_vertices(top_vertex, *p_graph_).second; ++it_v)
          {
            if(!connected[ (*p_graph_)[*it_v].id_ ]) to_visit.push(*it_v);
          }
          for(auto it_v = boost::inv_adjacent_vertices(top_vertex, *p_graph_).first; it_v != boost::inv_adjacent_vertices(top_vertex, *p_graph_).second; ++it_v)
          {
            if(!connected[ (*p_graph_)[*it_v].id_ ]) to_visit.push(*it_v);
          }
        }
      }
      t = clock() - t;
      findComponent += ((double)t)/CLOCKS_PER_SEC;

      /* condense this component */
      t = clock();
      bool ff_singleCycle = true;
      for(auto it_v = this_component.begin(); it_v != this_component.end(); ++it_v)
      {
        if( ff_seed[(*p_graph_)[*it_v].id_] && (!p_traversed[ (*p_graph_)[*it_v].id_ ]))
        {
          ff_singleCycle = false;
          condense(*it_v, to_add_cycle, p_traversed);
        }
      }
      t = clock() - t;
      condeseSE += ((double)t)/CLOCKS_PER_SEC;

      t = clock();
      if(ff_singleCycle)  // if this component is single loop
      {
        cnt_cycle_node += cnt_vertex;
        auto it_v = this_component.begin();
        ff_seed[(*p_graph_)[*it_v].id_] = true;
        condense(*it_v, to_add_cycle, p_traversed);
      }
      t = clock() - t;
      condenseCycle += ((double)t)/CLOCKS_PER_SEC;
    }
  }
  std::cout << "find components takes " << findComponent << " s\n";
  std::cout << "consense SE takes " << condeseSE << " s\n";
  std::cout << "consense cycle takes " << condenseCycle << " s\n";
  std::cout << cnt_cycle_node << " cycle nodes\n";

  /* modify graph */
  // t = clock();
  // std::list<BoostSTREdge> to_delete_edge;
  // for(auto it_e = boost::edges(*p_graph_).first; it_e != boost::edges(*p_graph_).second; ++it_e)
  // {
  //   if( (*p_graph_)[*it_e].ff_delete ) to_delete_edge.push_back(*it_e);
  // }
  // std::list<BoostSTRVertex> to_delete_vertex;
  // for(auto it_v = boost::vertices(*p_graph_).first; it_v != boost::vertices(*p_graph_).second; ++it_v)
  // {
  //   if( (*p_graph_)[*it_v].ff_delete ) to_delete_vertex.push_back(*it_v);
  // }
  // for(auto it = to_delete_edge.begin(); it != to_delete_edge.end(); ++it)     boost::remove_edge(*it, *p_graph_);
  // for(auto it = to_delete_vertex.begin(); it != to_delete_vertex.end(); ++it) boost::remove_vertex(*it, *p_graph_);
  //
  // p_node_id = 0;
  // for(auto it = to_add_cycle.begin(); it != to_add_cycle.end(); ++it)
  // {
  //   BoostSTRVertex s = it->head;
  //   BoostSTRVertex t = it->tail;
  //   std::pair<BoostSTREdge, bool> edge_new = add_edge(s, t, *p_graph_);
  //   if(edge_new.second)
  //   {
  //     (*p_graph_)[edge_new.first].SetCondensedTag(true);
  //     (*p_graph_)[edge_new.first].path_info_ = it->path;
  //     (*p_graph_)[edge_new.first].ff_cycle   = it->ff_cycle;
  //     (*p_graph_)[edge_new.first].sid_       = p_node_id;
  //     ++p_node_id;
  //   }
  // }
  //
  // t = clock() - t;
  // std::cout << "modify graph takes " << ((double)t)/CLOCKS_PER_SEC << " s\n";
}

void AssemblyGraph::condense(const BoostVertex seed,
  std::list<cycle_s>& to_add_cycle,
  BooleanVector_AG& p_traversed)
{
  for(auto it_e = boost::out_edges(seed, *p_graph_).first; it_e != boost::out_edges(seed, *p_graph_).second; ++it_e)
    condense(*it_e, to_add_cycle);
}

// void AssemblyGraph::condense(const BoostEdge source_edge,
//   std::list<cycle_s>& to_add_cycle,
//   BooleanVector_AG& p_traversed)
// {
//   BoostEdge current_edge = source_edge;
//   BoostVertex head = boost::source(current_edge, *p_graph_);
//   BoostVertex tail = head;
//   IntegerVector path_info;
//   path_info.push_back((*p_graph_)[head].id_);
//   path_info.push_back((*p_graph_)[head].len_);
//   path_info.push_back(0);
//   while(1)
//   {
//     p_traversed[ (*p_graph_)[tail].id_ ] = true;
//     (*p_graph_)[current_edge].deleted_ = true;
//     if(tail != head)  (*p_graph_)[tail].deleted_ = true;   // boost::remove_vertex(to_delete, *p_graph_);
//
//     tail = boost::target(current_edge, *p_graph_);          // define the new tail vertex
//     path_info.push_back((*p_graph_)[current_edge].p_A);
//     path_info.push_back((*p_graph_)[tail].rid_);
//     path_info.push_back((*p_graph_)[tail].len_);
//
//     if((*p_graph_)[tail].IsSeed())  break;
//     else                            current_edge = *(boost::out_edges(tail, *p_graph_)).first;
//   }
//   path_info.push_back(-1);
//   if( boost::out_degree(tail, *p_graph_) == 0)  p_traversed[ (*p_graph_)[tail].rid_ ] = true;
//
//   cycle_s new_cycle;
//   new_cycle.head = head;
//   new_cycle.tail = tail;
//   new_cycle.path = path_info;
//   new_cycle.ff_cycle = ((*p_graph_)[head].rid_ == (*p_graph_)[tail].rid_ );
//   to_add_cycle.push_back(new_cycle);
// }
