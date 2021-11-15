//  Author: Cuncong Zhong
//  Last modification: 11/10/2021

#include "GraphPrune.h"

using namespace std;

//==========================================================================================
//      Public methods implementations
//==========================================================================================

// remove orphant vertices
// parameter:
//    g: the pointer to the graph where the function operate on
void GraphPrune::RemoveOrphantVertices(AssemblyGraphType *g)    {
    // remove orphant vertices
    auto it_v = boost::vertices(*g).first;
    while(it_v != boost::vertices(*g).second) {
        BoostNodeType v = *it_v; ++ it_v;
        if(degree(v, *g) <= 0) boost::remove_vertex(v, *g);
    }
    return;
}

// Resolve read orientation; for each read only one direction is retained in the assembly graph
// Algorithm: Start with an unresolved node with the highest degree and set it to positive strand
// Repeat:  set its neigbhors' orientation according to the edge;
//          if the neighbor has already been set and is conflicting with the current edge, remove the current edge
//          mark the neighbors as resolved
//          return to step 1
// Internal data structure used: priority_queue
void GraphPrune::ResolveOrientation(AssemblyGraphType *g)   {
    // defining the comparison class for priority queue
    // each element is a pair <BoostNodeType, int>;
    // the first field is the node, the second field is the degree of the node
    class cmp_node_degree   {
      public:  
        // comparison operator
        bool operator() (const std::pair<BoostNodeType, int> &lhs, const std::pair<BoostNodeType,int> &rhs) const {
            // implement a MAX queue; assign higher priority to nodes with higher degree
            return lhs.second < rhs.second ? true : false;
        } 
    };
    // insert all nodes into priority queue
    std::priority_queue<std::pair<BoostNodeType, int>, std::vector<std::pair<BoostNodeType, int> >,  cmp_node_degree> degree_rank;
    auto it_v = boost::vertices(*g).first;
    while(it_v != boost::vertices(*g).second) {
        degree_rank.push(std::make_pair(*it_v, boost::degree(*it_v, *g)));
        ++ it_v;
    }
    // process the nodes one-by-one 
    while(!degree_rank.empty()) {
        std::pair<BoostNodeType, int> node_info = degree_rank.top();
        degree_rank.pop();
        //cout << "DEBUG: node degree:    " << boost::degree(node_info.first, *g) << endl;
        if((*g)[node_info.first].IsResolved()) continue;    // skip the source if it is resolved
        // set positive orientation for the source
        (*g)[node_info.first].SetOrientation(true);
        (*g)[node_info.first].SetResolved(true);
        // use BFS style traversal to resolve orientation
        std::queue<BoostNodeType> n_set;    // node set
        n_set.push(node_info.first);
        while(!n_set.empty()) {
            BoostNodeType s = n_set.front();  n_set.pop();
            // making sure each node can be used as a source for only once
            // otherwise may cause infinite loop if cycles exist
            if((*g)[s].IsVisited())    {
                continue;
            }   else    {
                (*g)[s].SetVisited(true); 
            }
            bool s_ori = (*g)[s].GetOrientation();   
            std::vector<BoostEdgeType> to_delete;   // the set of edges to be deleted
            // iterate through all edges
            auto it_e = boost::out_edges(s, *g).first;
            while(it_e != boost::out_edges(s, *g).second) {
                BoostNodeType t = boost::target(*it_e, *g);
                // if target not resolved, assign orientation   
                if(!(*g)[t].IsResolved())   {
                    (*g)[t].SetOrientation((*g)[*it_e].IsRevComplement() ? !s_ori : s_ori);     // setting orientation
                    (*g)[t].SetResolved(true);      // marking the target node as resolved
                    n_set.push(t);      // adding the target node to the queue
                }
                // if target resolved  
                else {
                    bool t_ori = (*g)[t].GetOrientation();
                    if((s_ori == t_ori) == !(*g)[*it_e].IsRevComplement())  {   // nodes orientations are consistent with edge info
                        ;    // if consistent, do nothing
                    }   else    {
                        // if not consistent, mark edge for deletion
                        to_delete.push_back(*it_e);
                    }
                }
                ++ it_e;
            }
            // delete the edges
            for(int i = 0; i < to_delete.size(); ++ i)   {
                boost::remove_edge(to_delete[i], *g);
            }
        }
    }
    return;
}

// Resolve the sequences in the assembly graph.
// Use reverse complementary if the read is assigned negative strand
// Also update all edges as no reverse-completementary
// Parameter:
//    g: the pointer to the graph where the function operates on
void GraphPrune::ResoveSequence(AssemblyGraphType *g)   {
    // check the consisency of the graph
    auto it_e = boost::edges(*g).first;
    while(it_e != boost::edges(*g).second) {
       if(((*g)[*it_e].is_rc_ && (*g)[boost::source(*it_e, *g)].orientation_ != (*g)[boost::target(*it_e, *g)].orientation_) ||
         !((*g)[*it_e].is_rc_ && (*g)[boost::source(*it_e, *g)].orientation_ == (*g)[boost::target(*it_e, *g)].orientation_)) {
            // consistent edge and nodes
            (*g)[*it_e].is_rc_ = false;
       }    else    {
           // inconsistency found
            cerr << "Error:  GraphPrune::ResolveSequence: Inconsistent edge/vertex orientation found. Call ResolveOrientation() before calling ResolveSequence(). Exit program." << endl;
            exit(1);        
       }
        ++ it_e;
    }
    // convert the sequence for each vertex 
    auto it_v = boost::vertices(*g).first;
    while(it_v != boost::vertices(*g).second) {
        if(!(*g)[*it_v].orientation_)    {  // if reverse complementary
            (*g)[*it_v].orientation_ = true;
            StringUtils::InplaceRevComp((*g)[*it_v].str_, (*g)[*it_v].str_len_);
        }
        ++ it_v;
    }
    return;
}