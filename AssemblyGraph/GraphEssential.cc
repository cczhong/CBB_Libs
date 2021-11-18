//  Author: Cuncong Zhong
//  Last modification: 11/09/2021

# include "GraphEssential.h"

using namespace std;

//==========================================================================================
//      Public methods implementations
//==========================================================================================

// prints the graph information
// parameters:
//    c: whether to print complete information including vertice and edge details; true for yes, false for no
void GraphEssential::PrintInfo(const bool c)  {
    std::cout << "Printing GraphEssential object info..." << std::endl;
    if(!initialized_)  {
        std::cout << "Graph is empty." << std::endl;
        return;
    }
    std::cout << "Num. of vertices:  " << boost::num_vertices(*graph_ptr_) << endl;
    std::cout << "Num. of edges:  " << boost::num_edges(*graph_ptr_) << endl;
    if(c) {
        // print complete information
        std::cout << "Printing all vertices information..." << std::endl;
        auto it_v = boost::vertices(*graph_ptr_).first;
        while(it_v != boost::vertices(*graph_ptr_).second) {
            std::cout << "===   begin of vertex:    " << std::endl;
            (*graph_ptr_)[*it_v].PrintInfo();
            std::cout << "===   end of vertex:    " << std::endl;
            ++ it_v;
        }
        std::cout << "Printing all edges information..." << std::endl;
        auto it_e = boost::edges(*graph_ptr_).first;
        while(it_e != boost::edges(*graph_ptr_).second) {
            std::cout << "===   begin of edge:    " << std::endl;
            (*graph_ptr_)[*it_e].PrintInfo();
            (*graph_ptr_)[boost::source(*it_e, *graph_ptr_)].PrintInfo();
            (*graph_ptr_)[boost::target(*it_e, *graph_ptr_)].PrintInfo();
            std::cout << "===   end of edge:  " << std::endl;
            ++ it_e;
        }
    }
    return;
}

// loads graph from ASQG file (the SGA overlap output)
void GraphEssential::LoadGraphASQG(const std::string & file)    {

    vector<bool> orientation;
    ReadASQG(file);
    
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

// add an edge between two nodes
// Note that the parameter "*g" is not necessary. We add it to make the parameter set be consistent with those in the GraphPrune and GraphTraverse.
// GraphPrune and GraphTraverse needs the graph as parameter because they are only method classes and do not define the graph.
// Parameter list:
//    s: the source node
//    t: the target node
//    g: the graph where we should add the edge
// Return: a pair made of the edge descriptor (if success) and whether adding the edge is success
std::pair<BoostEdgeType, bool> GraphEssential::AddEdge(const BoostNodeType &s, const BoostNodeType &t, AssemblyGraphType *g) {
    assert(g == graph_ptr_);    // make sure we are modifying the target object
    return boost::add_edge(s, t, *graph_ptr_);
}

// gets the edge information between two nodes
// Parameter list:
//    s: the source node
//    t: the target node
//    g: the graph where we should add the edge
// Return: a pair made of the edge descriptor (if success) and whether adding the edge is success
std::pair<BoostEdgeType, bool> GraphEssential::GetEdge(const BoostNodeType &s, const BoostNodeType &t, AssemblyGraphType *g)    {
    assert(g == graph_ptr_);    // make sure we are modifying the target object
    return boost::edge(s, t, *graph_ptr_);
}

// reads in the information contained in the ASQG file and returns the number of reads of the dataset
// parameter list:
//    file: the ASQG file
IDType GraphEssential::GetNumReadsASQG(const std::string &file) {
    IDType max_ID = 0;
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
            }   else if(vs[0] == "ED")   {
                break;
            }
        }
        asqg_fh.close();
    }    
    return (max_ID + 1);   // we need to add 1 to convert the ID into the size
}

// reads in the information contained in ASQG file without checking consistency
// parmeter lst:
//    file: the ASQG file
void GraphEssential::ReadASQG(const std::string &file)  {
    IDType num_nodes = GetNumReadsASQG(file);
    vector<bool> is_set(num_nodes, false);              // array indicating whether the orientation of a read is set
    vector<BoostNodeType> node_map(num_nodes);          // mapping from the node ID to the node decriptor
    ifstream asqg_fh (file);    
    if(asqg_fh.is_open())    {
        string line;
        while(getline(asqg_fh, line)) {
            vector<string> vs;
            StringUtils::SplitByDelimiter(line, "\t ", vs);
            assert(vs.size() >= 1);     //  fail assertion is likely due to sequence corruption
            if(vs[0] == "VT")    {
                // record the maximun read ID so far
                assert(vs.size() >= 3);             // fail assertion is likely due to sequence corruption
                IDType id = stoi(vs[1]);   
                GraphNodeType *n = new GraphNodeType(vs[2].c_str());    // copying the sequence to the node; by default orientation = true (plus strand)
                n->id_ = id;                                            // DEBUG
                //cout << "DEBUG: raw sequence:   " << vs[2] << endl; 
                //cout << "DEBUG: node sequence:   " << n.str_ << endl; 
                node_map[id] = boost::add_vertex(*n, *graph_ptr_);  // adding the node
                is_set[id] = true;
            }   else if(vs[0] == "ED")   {
                //cout << "DEBUG: entering edge info." << endl;
                //cout << "DEBUG: " << line << endl;
                assert(vs.size() >= 10);    // fail assertion is likely due to sequence corruption
                IDType a = stoi(vs[1]);     // the source ID
                IDType b = stoi(vs[2]);     // the target ID
                // checking validity of the edge
                if(a == b)  {               // a self-loop
                    cerr << "Warning:   GraphEssential::ReadASQG: A self-loop detected, edge ignored." << endl; 
                    continue;
                }   else if (!is_set[a] || !is_set[b])  {   // one of the nodes does not exist in the graph
                    cerr << "Warning:   GraphEssential::ReadASQG: Trying to add edge to nodes that do not exist, edge ignored." << endl; 
                }
                //cout << "DEBUG: two vertices:   " << a << " " << b << endl;
                // determine which node is the source and which is the target
                IDType s, t;
                bool src_ori;
                if(stoi(vs[3]) == 0)    {
                    // the first read has its prefix overlapped
                    t = a; s = b;
                    src_ori = (bool) stoi(vs[9]) ? false : true;    // a is the target; if the overlap is reverse complementary then the source is negative strand; otherwise positive
                }   else   {
                    // the first read has its suffix overlapped
                    s = a; t = b;
                    src_ori = true;     // always be true, because a is always positive strand
                }
                //cout << "DEBUG: " << line << endl;
                //cout << "DEBUG: source and target:  " << s << " " << t << endl;
                // check if an edge already exists
                pair<BoostEdgeType, bool> e_check = boost::edge(node_map[s], node_map[t], *graph_ptr_);  // note that the target is the first parameter and the source is the second
                if(!e_check.second)    {  
                    pair<BoostEdgeType, bool> e_add = boost::add_edge(node_map[s], node_map[t], *graph_ptr_);     // note that the target is the first parameter and the source is the second
                    if(!e_add.second)    {
                        cerr << "Warning:   GraphEssential::ReadASQG: Failed to add edge, edge ignored." << endl; 
                    }   else    {
                        // if edge is successfully added, incorporate related information
                        (*graph_ptr_)[e_add.first].SetOverlap(stoi(vs[4]) - stoi(vs[3]) + 1);
                        (*graph_ptr_)[e_add.first].SetIsRevComplement((bool) stoi(vs[9]));  // "1" in the ASQG file indicates reverse complement
                        (*graph_ptr_)[e_add.first].SetSrcOrientation(src_ori);
                    }
                }   else    {
                    // check overlap, use the larger overlap between sequences
                    //cout << "DEBUG: duplicate edge overlap info:    " << (*graph_ptr_)[e_check.first].GetOverlap() << " " << stoi(vs[4]) - stoi(vs[3]) + 1 << endl;
                    //cout << "DEBUG: duplicate edge rev_comp info:    " << (*graph_ptr_)[e_check.first].IsRevComplement() << " " << stoi(vs[9]) << endl;
                    if((*graph_ptr_)[e_check.first].GetOverlap() < stoi(vs[4]) - stoi(vs[3]) + 1)    {
                        (*graph_ptr_)[e_check.first].SetOverlap(stoi(vs[4]) - stoi(vs[3]) + 1);
                        (*graph_ptr_)[e_check.first].SetIsRevComplement((bool) stoi(vs[9]));  // "1" in the ASQG file indicates reverse complement
                        (*graph_ptr_)[e_check.first].SetSrcOrientation(src_ori);
                    }
                }
            }
        }
        //cout << "DEBUG: end of while loop." << endl;
        asqg_fh.close();
    }    
    return;
}