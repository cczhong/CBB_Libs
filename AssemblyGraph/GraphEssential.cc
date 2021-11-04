//  Author: Cuncong Zhong
//  Last modification: 11/03/2021

# include "GraphEssential.h"

using namespace std;

//==========================================================================================
//      Public methods implementations
//==========================================================================================

// loads graph from ASQG file (the SGA overlap output)
void GraphEssential::LoadGraphASQG(const std::string & file)    {

    vector<bool> orientation;
    InferReadOrientationASQG(file, orientation);
    /*
    ifstream asqg_fh (file);
    if(asqg_fh.is_open())    {
        string line;
        while(getline(asqg_fh, line)) {
            vector<string> vs;
            StringUtils::SplitByDelimiter(line, '\t', vs);
            if(vs[0] == "VT")    {
                // add the node to the graph
                GraphNodeType node(vs[2].c_str());
                AddNode(node, graph_ptr_);
            }   else if(vs[0] == "ED")   {
                // add the edge to the graph
            }
        }
        asqg_fh.close();
    }   else {
        cerr << "GraphEssential::LoadGraphASQG: " << "Unable to open ASQG file. Abort." << endl;
        exit(1);
    }
    */
    return;
}

//==========================================================================================
//      Private methods implementations
//==========================================================================================

//  add a node to the graph
//  Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
//  GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
//  parameter list:
//      n: the node to be insert
//      g: the pointer to the graph where the node should be insert, should always be graph_ptr_
BoostNodeType GraphEssential::AddNode(const GraphNodeType &n, AssemblyGraphType *g) {
    assert (g == graph_ptr_);   // make sure we are modifying the target object
    return boost::add_vertex(n, *graph_ptr_);
}

//  infer the orientation of the read that needs to be represented in the graph
//  with a goal to remove duplicated reads
//  parameter list:
//    file: the ASQG file
//    ot: the orientations for each read, 0 indicates 5'->3', 1 indicates 3'->5'
void  GraphEssential::InferReadOrientationASQG(const std::string& file, std::vector<bool> &ot)  {
    IDType max_ID = 0;
    bool is_ot_init = false;        // whether we have initilaized the ot array
    vector<bool> is_set;            // array indicating whether the orientation of a read is set
    int num_inconsistent = 0;       // number of inconsistent overlaps, only for analysis purpose and can be deleted
    int num_edges = 0;              // number of edges, only for analysis purpose and can be deleted
    ifstream asqg_fh (file);    
    if(asqg_fh.is_open())    {
        string line;
        while(getline(asqg_fh, line)) {
            vector<string> vs;
            StringUtils::SplitByDelimiter(line, "\t ", vs);
            assert(vs.size() >= 1);     //  fail assertion is likely due to sequence corruption
            if(vs[0] == "VT")    {
                // record the maximun read ID so far
                assert(vs.size() >= 3);     // fail assertion is likely due to sequence corruption
                max_ID = max_ID < stoi(vs[1]) ? stoi(vs[1]) : max_ID;   
                
                //cout << "DEBUG: " << max_ID << endl;

            }   else if(vs[0] == "ED")   {
                //cout << "DEBUG: entering edge info." << endl;
                assert(vs.size() >= 10);    // fail assertion is likely due to sequence corruption
                // if ot array is not initalized, initialize it
                if(!is_ot_init) {
                    ot.resize(max_ID + 1, 0);
                    is_set.resize(max_ID + 1, 0);   
                    is_ot_init = true;
                    //cout << "DEBUG: initialization or array done" << endl;
                }
                // use greedy algorithm to set the read orientation
                IDType s = stoi(vs[1]);     // the source ID
                IDType t = stoi(vs[2]);     // the target ID

                
                //cout << "DEBUG: source/target computed." << endl;

                bool rc = vs[9] == "0" ? false : true;
                if(!is_set[s] && !is_set[t])    {
                    // the source and target are both unset
                    is_set[s] = true;   // mark the source as set
                    ;                       // set the source orientation as forward (5'->3', do nothing)
                    is_set[t] = true;   // mark the target as set 
                    ot[t] = rc;         // set the target orentation as indicated in the ASQG info
                }   else if(is_set[s] && !is_set[t])   {
                    // the source is set but the target is not
                    is_set[t] = true;   // mark the target as set
                    ot[t] = rc ? ot[s] : !ot[s];     // set based on whether the source and target have different orientations
                }   else if(!is_set[s] && is_set[t])   {
                    // the source is unset but the target is set
                    is_set[s] = true;   // mark the target as set
                    ot[s] = rc ? ot[t] : !ot[t];     // set based on whether the source and target have different orientations
                }   else    {
                    if((!rc && ot[s] != ot[t]) || (rc && ot[s] == ot[t]))    {
                        ++ num_inconsistent;    // count the number of incompatible overlaps
                    }   
                    ;                   // do nothing because both source and targets are set
                }
                ++ num_edges;
            }
        }
        asqg_fh.close();
        cout << "DEBUG: num of inconsistent edges:   " << num_inconsistent  << endl;
        cout << "DEBUG: num of total edges:  " << num_edges << endl;
    }    
    return;
}