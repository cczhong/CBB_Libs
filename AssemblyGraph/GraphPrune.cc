//  Author: Cuncong Zhong
//  Last modification: 11/10/2021

#include "GraphPrune.h"

using namespace std;

//==========================================================================================
//      Public methods implementations
//==========================================================================================

// remove orphant vertices
// parameter:
//    g: the pointer to the graph where the function operate on
void GraphPrune::RemoveOrphantVertices(AssemblyGraphType *g)    {
    // remove orphant vertices
    auto it_v = boost::vertices(*g).first;
    while(it_v != boost::vertices(*g).second) {
        BoostNodeType v = *it_v; ++ it_v;
        if(degree(v, *g) <= 0) boost::remove_vertex(v, *g);
    }
    return;
}

// Resolve read orientation; for each read only one direction is retained in the assembly graph
// Algorithm: Start with an unresolved node with the highest degree and set it to positive strand
// Repeat:  set its neigbhors' orientation according to the edge;
//          if the neighbor has already been set and is conflicting with the current edge, remove the current edge
//          mark the neighbors as resolved
//          return to step 1
// Internal data structure used: priority_queue
void GraphPrune::ResolveOrientation(AssemblyGraphType *g)   {
    // defining the comparison class for priority queue
    class cmp_node_degree   {
        AssemblyGraphType *g_ptr_;
      public:  
        // construction function
        explicit cmp_node_degree(AssemblyGraphType *gp)   {
            g_ptr_ = gp;
        }
        // comparison operator
        bool operator() (const BoostNodeType &lhs, const BoostNodeType &rhs) const {
            // implement a MAX queue; assign higher priority to nodes with higher degree
            return boost::degree(lhs, *g_ptr_) >= boost::degree(rhs, *g_ptr_) ? true : false;
        } 
    };
    
    return;
}