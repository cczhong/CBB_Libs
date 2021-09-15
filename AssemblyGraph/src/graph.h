#ifndef ASSEMBLYGRAPH_H_
#define ASSEMBLYGRAPH_H_
#include <cstdint>
#include <boost/tokenizer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stack>
#include <list>

/*
adjacency_list<         // a 2D structure, the first is a vertex, vertex contains its edge list
   OutEdgeList,         // container is used to represent the edge lists, vecS faster to iterate, listS faster to add
   VertexList,          // container is used to represent the outer two-dimensional container
   Directed,            // directed / undirected / (bidirectional) directed with access to both the in-edges and out-edges
   VertexProperties,    //
   EdgeProperties,      //
   GraphProperties,     //
   EdgeList>            //
*/
typedef uint64_t IDType_AG;
typedef uint16_t SeqLenType_AG;
typedef std::vector<bool>         BooleanVector_AG;
typedef std::vector<std::string>  StringVector_AG;
typedef std::vector<int>          IntegerVector_AG;
typedef struct Cycle
{
  IDType_AG         head;
  IDType_AG         tail;
  IntegerVector_AG  path;
  bool              ff_cycle;
} cycle_s;

class vertexType
{
public:
  explicit vertexType(const IDType_AG& i) { id_ = i; p_seq_ = nullptr; len_ = 0; deleted_ = false; }
  IDType_AG       id_;                // the ID for the current vertex
  const char*     p_seq_ = nullptr;   // the suquence
  SeqLenType_AG   len_ = 0;           // the length of sequence
  bool            deleted_ = false;   // set true when current vertex is deleted, or to be delete
};

class edgeType
{
public:
  explicit edgeType(const SeqLenType_AG& op) { op_ = op; deleted_ = false; }
  SeqLenType_AG     op_;                // overlap length
  bool              deleted_ = false;   // set true when current edge is deleted, or to be delete
};

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, vertexType, edgeType> BoostGraph;
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
typedef boost::graph_traits<BoostGraph>::vertex_iterator   BoostVertexIter;
typedef boost::graph_traits<BoostGraph>::edge_iterator     BoostEdgeIter;

class AssemblyGraph{
private:
  IDType_AG          order_;                               // order of graph
  BoostGraph*     p_graph_;                             // pointer to boost graph object
  std::unordered_map<int, BoostVertex> vertex_map_;      // map to vertex object
  // this is used when reading from ASQG file
  std::unordered_map<std::string, int> read_name_map_;  // map read actual name to vertex ID

  void condense(const BoostVertex seed,
    std::list<cycle_s>& to_add_cycle,
    BooleanVector_AG& p_traversed);
  void condense(const BoostEdge source_edge,
    std::list<cycle_s>& to_add_cycle,
    BooleanVector_AG& p_traversed);

public:
  AssemblyGraph()  { p_graph_ = new BoostGraph(); order_ = 0; };
  // AssemblyGraph(const std::string& graph_file, const std::string& read_info_file=nullptr);
  ~AssemblyGraph() { delete p_graph_; };
  void showGraph();
  bool addNode(const vertexType& v);
  bool addEdge(const IDType_AG&  source, const IDType_AG&  target, const edgeType&  e);

  // bool deleteNode(const IDType_AG& node_id);
  // bool deleteEdge(const IDType_AG& source_id, const IDType_AG& target_id);
  // bool modifyNode(const vertexType& v);
  // bool modifyEdge(const IDType_AG&  source, const IDType_AG&  target, const edgeType&  e);
  // void DFSRight(int anchor_id, int right);

  // bool readGraph(const std::string& graph_file, const std::string& read_info_file = nullptr);
  // bool writeGraph(const std::string& graph_file, const std::string& read_info_file = nullptr);
  bool readGraphFromAsqg(const std::string& filename);
  void condense();    // only valid for graph readed from ASQG


};
#endif
