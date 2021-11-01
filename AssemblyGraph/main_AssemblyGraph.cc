#include <iostream>

#include "AssemblyGraph.h"

using namespace std;

int main()  {
    AssemblyGraph graph;
    cout << "is graph initialized:  " << graph.IsInitialized() << endl;

    graph.LoadGraphASQG("/home/cczhong/Works/MANA/Data/sga_graph.asqg");

    return 0;
}