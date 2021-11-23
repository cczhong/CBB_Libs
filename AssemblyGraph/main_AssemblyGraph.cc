//  Author: Cuncong Zhong
//  Last modification: 11/09/2021

#include <iostream>

#include "AssemblyGraph.h"

using namespace std;

int main()  {
    AssemblyGraph *graph = new AssemblyGraph;
    cout << "is graph initialized:  " << graph->IsInitialized() << endl;

    
    graph->LoadGraphASQG("/home/cczhong/Works/MANA/Data/sga_graph.asqg");     //sga_graph.small.asqg
    cout << "DEBUG: after loading graph:" << endl;
    graph->PrintInfo(false);

    graph->RemoveOrphanVertices();
    cout << "DEBUG: after removing orphan reads:" << endl;
    graph->PrintInfo(false);
    
    graph->ResolveOrientation();
    cout << "DEBUG: after resolving orientation:" << endl;
    graph->PrintInfo(false);

    graph->ResolveSequence();
    cout << "DEBUG: after resolving sequence:" << endl;
    graph->PrintInfo(false);
    

    graph->WriteGraphASQG("tmp.out");

    return 0;
}