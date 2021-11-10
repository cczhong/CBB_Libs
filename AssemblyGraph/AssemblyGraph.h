//  Author: Cuncong Zhong
//  Last modification: 11/10/2021

#ifndef __ASSEMBLYGRAPH_H_
#define __ASSEMBLYGRAPH_H_

#include "GraphEssential.h"
#include "GraphPrune.h"
#include "GraphTraversal.h"

class AssemblyGraph : public GraphEssential {
  public:
    // constructor (defined in GraphEssential): initialize *graph_ptr_ 
    // destructor (defined in GraphEssentual): if initialized, delete *graph_ptr

    // wrapper function to call GraphPrune::RemoveOrphantVertices() from AssemblyGraph
    void RemoveOrphantVertices(void)  {
      //GraphPrune gp;
      gp.RemoveOrphantVertices(graph_ptr_);
    }

    // wrapper function to call GraphPrune::ResolveOrientation() from AssemblyGraph
    void ResolveOrientation(void) {
      //GraphPrune gp;
      gp.ResolveOrientation(graph_ptr_);
    }

  private:
    GraphPrune gp;
    GraphTraversal gt;
};

#endif  //__ASSEMBLYGRAPH_H_