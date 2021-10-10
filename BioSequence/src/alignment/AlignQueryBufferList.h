#ifndef __ALIGNQUERYBUFFERLIST_H__
#define __ALIGNQUERYBUFFERLIST_H__


#include "VATSearch.h"
#include "../paras/statistics.h"
#include "../output/VATOutBuffer.h"

template<typename _val, typename _locr, typename _locl,typename _d>
void alignQueriesBufferList(typename TracePtList<_locr, _locl>::iterator begin, 
                            typename TracePtList<_locr, _locl>::iterator end,
                                            OutputBuffers<_val> &buffer,
                                            Statistics &st);





#endif // __ALIGNQUERYBUFFERLIST_H__