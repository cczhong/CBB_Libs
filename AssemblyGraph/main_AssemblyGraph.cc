//  Author: Cuncong Zhong
//  Last modification: 11/09/2021

#include <iostream>

#include "AssemblyGraph.h"

using namespace std;

int main()  {
    AssemblyGraph *graph = new AssemblyGraph;
    cout << "is graph initialized:  " << graph->IsInitialized() << endl;

    graph->LoadGraphASQG("/home/cczhong/Works/MANA/Data/sga_graph.small.asqg");
    graph->PrintInfo(false);

    graph->RemoveOrphantVertices();

    graph->PrintInfo(false);

    return 0;
}