#ifndef __ALIGNQUERY_H__
#define __ALIGNQUERY_H__

//#include "../tool/merge_sort.h"
#include "VATSearch.h"
//#include "../tool/map.h"
//#include "AlignRead.h"
#include "../tool/TaskQueue.h"
#include "../tool/BinaryFile.h"
#include "../output/VATOutBuffer.h"

using std::vector;

class AlignQuery
{
private:
public:
	template<typename _val, typename _locr, typename _locl>
	void AlignQueryBuffer(const TracePtBuffer<_locr, _locl> &trace_pts, OutputStreamController *output_file);

};




#endif // __ALIGNQUERY_H__