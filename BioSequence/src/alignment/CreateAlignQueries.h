#ifndef __CREATEQUERIES_H__
#define __CREATEQUERIES_H__

#include "AlignQuery.h"
#include "../tool/TaskQueue.h"
#include "../tool/BinaryFile.h"
#include "../tool/TextBuffer.h"
#include "./AlignQueryBufferList.h"

class OutputWriter
{
	public:
	
		OutputWriter(OutputStreamController* f):
			f_ (f)
		{ 

		}
		void operator()(TextBuffers &buf)
		{
			f_->write(buf.get_begin(), buf.size());
			buf.clear();
		}
	private:
			OutputStreamController*  const f_;
};


template<typename _val, typename _locr, typename _locl, typename _buffer>
class CreateAlignQueries
{
	public:
	CreateAlignQueries(TracePtList<_locr,_locl> &trace_pts, OutputStreamController* output_file):
		trace_pts (trace_pts),
		output_file (output_file),
		writer (output_file),
		queue (VATOptions::threads()*8, writer)
	{ 

	}
	void operator()(unsigned thread_id)
	{
		Statistics st;
		size_t i=0;
		typename TracePtList<_locr,_locl>::Query_range query_range (trace_pts.get_range());
		_buffer *buffer = 0;
		while(queue1.get(i, buffer, query_range) && !exception_state()) 
		{
			try
			{
				switch(Query_Contexts())
				{
					case 2:		
						AlignQueriesBufferList<_val, _locr, _locl, 2>(query_range.begin, query_range.end, *buffer, st);
						// AlignQuery aq;
						// aq.AlignQueriesBufferList<_val, _locr, _locl, 2>(query_range.begin, query_range.end, *buffer, st);
				}
				queue1.push(i);
			} 
			catch(std::exception &e) 
			{
				exception_state.set(e);
				queue1.wake_all();
			}
		}
		statistics += st;
	}
	//AlignQuery aq;
	TracePtList<_locr, _locl> &trace_pts;
	OutputStreamController* output_file;
	OutputWriter writer;
	Task_queue<_buffer,OutputWriter> queue1;
};


#endif // __CREATEQUERIES_H__