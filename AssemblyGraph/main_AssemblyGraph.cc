//  Author: Cuncong Zhong
//  Last modification: 11/53/2021

#include <iostream>

#include "AssemblyGraph.h"

using namespace std;

int main()  {
    AssemblyGraph *graph = new AssemblyGraph;
    cout << "is graph initialized:  " << graph->IsInitialized() << endl;

    graph->LoadGraphASQG("/home/cczhong/Works/MANA/Data/test");
    graph->PrintInfo(true);

    return 0;
}