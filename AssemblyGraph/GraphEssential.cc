# include "GraphEssential.h"

using namespace std;

// add a node to the graph
// Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
// GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
BoostNodeType GraphEssential::AddNode(const GraphNodeType &n, AssemblyGraphType *g) {
    assert (g == graph_ptr_);   // make sure we are modifying the target object
    return boost::add_vertex(n, *graph_ptr_);
}
