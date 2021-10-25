#ifndef __GRAPHEDGETYPE_H_
#define __GRAPHEDGETYPE_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <assert.h>

#include "../CommonDef/dataType.h"   

class GraphEdgeType
{
  public:

    // empty construction function; setting the overlap length to 0
    GraphEdgeType() {
        overlap_ = 0;
    }

    // initialization with overlap length
    explicit GraphEdgeType(const SeqIdxType overlap)    {
        overlap_ = overlap;
        visited_ = false;
    }

    // destructor function
    ~GraphEdgeType()    {
        ;   // do nothing
    }

    // assignment operator
    GraphEdgeType& operator=(const GraphEdgeType &n) {
        this->overlap_ = n.overlap_;
        this->visited_ = n.visited_;
        return *this;
    } 

    // setting the overlap length
    void SetOverlap(const SeqIdxType overlap)   {
        overlap_ = overlap;
    }

    // retrieving the overlap length
    SeqIdxType GetOverlap(void) const {
        return overlap_;
    }

    // returns the coverage of the current node
    CoverageType GetCoverage(void) const    {
        return cov_;
    }

    // returns whether the node has been visited or not
    bool IsVisited(void) const  {
        return visited_;
    }

    // setting the node as visited
    void SetVisited(void) {
        visited_ = true;
        return;
    }

    // setting the node as unvisited
    void SetUnvisited(void) {
        visited_ = false;
        return;
    }

    friend class GraphNodeType;
    friend class AssemblyGraph;

  protected:
    SeqIdxType overlap_;        // the overlap length between the adjacent nodes
    CoverageType cov_;          // the coverage of the edge
                                // TODO: need to define the coverage of an edge using the coverages of the end nodes
    bool visited_;
};


#endif      // __GRAPHEDGETYPE_H_
