#ifndef __GRAPHESSENTIAL_H_
#define __GRAPHESSENTIAL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

#include "../CommonUtils/StringUtils.h"
#include "GraphNodeType.h"
#include "GraphEdgeType.h"

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, GraphNodeType, GraphEdgeType> AssemblyGraphType;
typedef boost::graph_traits<AssemblyGraphType>::vertex_descriptor BoostNodeType;
typedef boost::graph_traits<AssemblyGraphType>::edge_descriptor BoostEdgeType;
typedef boost::graph_traits<AssemblyGraphType>::vertex_iterator BoostNodeItrType;
typedef boost::graph_traits<AssemblyGraphType>::edge_iterator BoostEdgeItrType;

class GraphEssential {
  public:

    GraphEssential()    {
      graph_ptr_ = new AssemblyGraphType;
      initialized_ = true;
    }

    ~GraphEssential()   {
        Purge();
    }

    // collect the memory if the graph has been initialized
    void Purge(void)  {
        if(initialized_) {  delete graph_ptr_;    }
        return;
    }

    // loads graph from ASQG file (the SGA overlap output)
    void LoadGraphASQG(const std::string & file);

    // loads graph from file
    void LoadGraph(const std::string &file);

    // writes graph into file
    void WriteGraph(const std::string &file);

    // add a node to the graph
    // Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
    // GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
    BoostNodeType AddNode(const GraphNodeType &n, AssemblyGraphType *g);

    // delete a node from the graph

    // add an edge between two nodes

    // delete an edge between two nodes


    // check if the graph is initialized
    bool IsInitialized(void)  {
      return initialized_;
    }
    


    friend class GraphPrune;
    friend class GraphTraversal;

  protected:
    bool initialized_;
    AssemblyGraphType *graph_ptr_;

};

#endif  // __GRAPHESSENTIAL_H_