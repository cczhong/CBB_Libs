//  Author: Cuncong Zhong
//  Last modification: 11/03/2021

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

#include "../CommonDef/DataType.h"
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

    // loads graph from ASQG file (the SGA overlap output)
    // param:
    //    file: the input file to read from
    void LoadGraphASQG(const std::string & file);

    // writes graph into ASQG file
    // param:
    //    file: the output file to write into
    void WriteGraphASQG(const std::string & file);

    // loads graph from file
    void LoadGraph(const std::string &file);

    // writes graph into file
    void WriteGraph(const std::string &file);

    // check if the graph is initialized
    bool IsInitialized(void)  {
      return initialized_;
    }

    // prints the graph information
    // parameters:
    //    c: whether to print complete information including vertice and edge details; true for yes, false for no
    void PrintInfo(const bool c);
    
    friend class GraphPrune;
    friend class GraphTraversal;

  private:

    // collect the memory if the graph has been initialized
    void Purge(void)  {
      if(initialized_) {  delete graph_ptr_;    }
      return;
    }

    // add a node to the graph
    // Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
    // GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
    //    n: the node
    //    g: the graph where we should add the node
    BoostNodeType AddNode(const GraphNodeType &n, AssemblyGraphType *g);

    // delete a node from the graph

    // add an edge between two nodes
    // Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
    // GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
    // Parameter list:
    //    s: the source node
    //    t: the target node
    //    g: the graph where we should add the edge
    // Return: a pair made of the edge descriptor (if success) and whether adding the edge is success
    std::pair<BoostEdgeType, bool> AddEdge(const BoostNodeType &s, const BoostNodeType &t, AssemblyGraphType *g);

    // delete an edge between two nodes


    // gets the edge information between two nodes
    // Parameter list:
    //    s: the source node
    //    t: the target node
    //    g: the graph where we should add the edge
    // Return: a pair made of the edge descriptor (if success) and whether adding the edge is success
    std::pair<BoostEdgeType, bool> GetEdge(const BoostNodeType &s, const BoostNodeType &t, AssemblyGraphType *g);

    // reads in the information contained in ASQG file without checking consistency
    // parmeter lst:
    //    file: the ASQG file
    void ReadASQG(const std::string &file);

    // reads in the information contained in the ASQG file and returns the number of reads of the dataset
    // parameter list:
    //    file: the ASQG file
    IDType GetNumReadsASQG(const std::string &file);


  protected:
    // tag for deciding whther the graph has been initialized
    bool initialized_;
    // the graph
    AssemblyGraphType *graph_ptr_;

};

#endif  // __GRAPHESSENTIAL_H_