#ifndef __GRAPHNODETYPE_H_
#define __GRAPHNODETYPE_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <assert.h>

typedef uint32_t SeqIdxType;

class GraphNodeType {
  public:
    
    GraphNodeType(void) {
        str_ = nullptr;
        str_len_ = 0;
        visited_ = false;
    }

    ~GraphNodeType(void)    {
        if(str_len_ > 0)    delete []  str_;
    }

    void SetSequence(char *s)    {
        str_len_ = strlen(s);
        str_ = new char [str_len_ + 1];
        strcpy(str_, s);
        return;
    }

    GraphNodeType& operator=(const GraphNodeType &n) {
        this->str_len_ = n.GetSeqLen();
        this->str_ = new char [this->str_len_ + 1];
        strcpy(this->str_, n.str_);
        this->visited_ = n.visited_;
        return *this;
    } 

    SeqIdxType GetSeqLen(void) const {
        return str_len_;
    } 

    char* GetStrPtr(void) const {
        return str_;
    }

    bool isVisited(void) const  {
        return visited_;
    }

    void setVisited(void) {
        visited_ = true;
        return;
    }

    friend class GraphEdgeType;
    friend class AssemblyGraph;

  protected:
    char *str_;
    SeqIdxType str_len_;
    bool visited_;
};

#endif  // __GRAPHNODETYPE_H_