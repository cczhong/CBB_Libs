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

#include "GraphNodeType.h"
#include "GraphEdgeType.h"

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, GraphNodeType, GraphEdgeType> AssemblyGraphType;
typedef boost::graph_traits<AssemblyGraphType>::vertex_descriptor NodeDescriptorType;
typedef boost::graph_traits<AssemblyGraphType>::edge_descriptor EdgeDescriptorType;
typedef boost::graph_traits<AssemblyGraphType>::vertex_iterator NodeIteratorType;
typedef boost::graph_traits<AssemblyGraphType>::edge_iterator EdgeIteratorType;

class GraphEssential {
  public:


    GraphEssential()    {

    }

    ~GraphEssential()   {
        Purge();
    }

    // collect the memory if the graph has been initialized
    void Purge(void)  {
        if(initialized_) {  delete graph_ptr_;    }
        return;
    }
    
  protected:
    bool initialized_;
    AssemblyGraphType *graph_ptr_;

};

#endif  // __GRAPHESSENTIAL_H_