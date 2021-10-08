#ifndef __GRAPHNODETYPE_H_
#define __GRAPHNODETYPE_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <assert.h>

// TODO: this definition should be included from the SequenceIndex object or some common definition headers
typedef uint32_t SeqIdxType;    


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
    GraphNodeType(char *s)  {
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

    // updating the sequence of a node
    void SetSequence(char *s)    {
        str_len_ = strlen(s);
        str_ = new char [str_len_ + 1];
        strcpy(str_, s);
        return;
    }

    // assignment operator
    GraphNodeType& operator=(const GraphNodeType &n) {
        this->str_len_ = n.GetSeqLen();
        this->str_ = new char [this->str_len_ + 1];
        strcpy(this->str_, n.str_);
        this->visited_ = n.visited_;
        return *this;
    } 

    // access the length of the sequence contained in the node
    SeqIdxType GetSeqLen(void) const {
        return str_len_;
    } 

    // TODO: to be updated with BioSequence class data access
    char* GetStrPtr(void) const {
        return str_;
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
    bool visited_;              // the tag indicating whether the node has been visited
};

#endif  // __GRAPHNODETYPE_H_