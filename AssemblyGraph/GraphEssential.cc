# include "GraphEssential.h"

using namespace std;

// loads graph from ASQG file (the SGA overlap output)
void GraphEssential::LoadGraphASQG(const std::string & file)    {
    ifstream asqg_fh (file);
    if(asqg_fh.is_open())    {
        string line;
        while(getline(asqg_fh, line)) {
            vector<string> vs;
            StringUtils::SplitByDelimiter(line, '\t', vs);
            // TODO: continue working on read ASQG files
        }
        asqg_fh.close();
    }   else {
        cerr << "GraphEssential::LoadGraphASQG: " << "Unable to open ASQG file. Abort." << endl;
        exit(1);
    }
    return;
}

// add a node to the graph
// Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
// GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
BoostNodeType GraphEssential::AddNode(const GraphNodeType &n, AssemblyGraphType *g) {
    assert (g == graph_ptr_);   // make sure we are modifying the target object
    return boost::add_vertex(n, *graph_ptr_);
}
