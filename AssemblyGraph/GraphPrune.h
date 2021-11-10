//  Author: Cuncong Zhong
//  Last modification: 11/10/2021

#ifndef __GRAPHPRUNE_H_
#define __GRAPHPRUNE_H_

#include <queue>

#include "GraphEssential.h"

class GraphPrune    {
  public:
    // void construction function, does nothing
    GraphPrune()    {}
    // void destruction function, does nothing
    ~GraphPrune()   {}

    // Remove orphant vertices
    // parameter:
    //    g: the pointer to the graph where the function operate on
    void RemoveOrphantVertices(AssemblyGraphType *g);

    // Resolve read orientation; for each read only one direction is retained in the assembly graph
    // Algorithm: Start with an unresolved node with the highest degree and set it to positive strand
    // Repeat:  set its neigbhors' orientation according to the edge;
    //          if the neighbor has already been set and is conflicting with the current edge, remove the current edge
    //          mark the neighbors as resolved
    //          return to step 1
    // Internal data structure used: priority_queue
    // parameter:
    //    g: the pointer to the graph where the function operate on
    void ResolveOrientation(AssemblyGraphType *g);

};

#endif          // __GRAPHPRUNE_H_