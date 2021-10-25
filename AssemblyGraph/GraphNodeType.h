#ifndef __GRAPHNODETYPE_H_
#define __GRAPHNODETYPE_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <assert.h>

#include "../CommonDef/dataType.h"   

// The assembly graph node class
class GraphNodeType {
  public:
    
    // empty constructor function
    GraphNodeType(void) {
        str_ = nullptr;
        str_len_ = 0;
        visited_ = false;
    }

    // constructor function from a char array
    explicit GraphNodeType(char *s)  {
        str_len_ = strlen(s);
        str_ = new char [str_len_ + 1];
        strcpy(str_, s);
        visited_ = false;
        return;
    }

    // destructor
    ~GraphNodeType(void)    {
        if(str_len_ > 0)    delete []  str_;
    }

    // assignment operator
    GraphNodeType& operator=(const GraphNodeType &n) {
        this->str_len_ = n.GetSeqLen();
        this->str_ = new char [this->str_len_ + 1];
        strcpy(this->str_, n.str_);
        this->visited_ = n.visited_;
        return *this;
    } 

    // updating the sequence of a node
    void SetSequence(char *s)    {
        str_len_ = strlen(s);
        str_ = new char [str_len_ + 1];
        strcpy(str_, s);
        return;
    }

    // access the length of the sequence contained in the node
    SeqIdxType GetSeqLen(void) const {
        return str_len_;
    } 

    // TODO: to be updated with BioSequence class data access
    const char* GetStrPtr(void) const {
        return str_;
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

    friend class GraphEdgeType;
    friend class AssemblyGraph;

  protected:
    char *str_;                 // the char array that holds the sequence
    SeqIdxType str_len_;        // the length of the sequence
    CoverageType cov_;          // the coverage of the node
                                // TODO: to define profile type, waiting for BioSequence definition
                                // Or, can we compute but not actually store the frequency informaiton?
    bool visited_;              // the tag indicating whether the node has been visited

};

#endif  // __GRAPHNODETYPE_H_