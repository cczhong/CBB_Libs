#ifndef __ALIGNQUERIES_H__
#define __ALIGNQUERIES_H__



#include "../tool/MergeSort.h"
#include "VATSearch.h"
#include "../tool/Map.h"
#include "AlignRead.h"
#include "../tool/TaskQueue.h"
#include "../tool/Thread.h"

using std::vector;

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

template<typename _val, typename _locr, typename _locl, unsigned _d>
void alignQueries(typename TracePtList<_locr,_locl>::iterator begin,
		typename TracePtList<_locr,_locl>::iterator end,
		OutputBuffers<_val> &buffer,
		Statistics &st)
{
	typedef Map<typename vector<Hit<_locr,_locl> >::iterator,typename Hit<_locr,_locl>::template QueryId<_d> > Map_t;
	Map_t hits (begin, end);
	//cout << "align_queries...map t." << endl;
	typename Map_t::Iterator i = hits.begin();
	//cout << "align_queries....valid() = " <<i.valid()<< ",exception_state()= "<<exception_state()<<endl;
	while(i.valid() && !exception_state()) 
	{
		//cout << "AlignReads...." << endl;
		//AlignRead<_val, _locr, _locl> ar;
		alignReads<_val, _locr, _locl>(buffer, st, i.begin(), i.end());
		//align_read<_val,_locr,_locl>(buffer, st, i.begin(), i.end());
		++i;
	}
}

template<typename _val, typename _locr, typename _locl, typename _buffer>
class CreateAlignQueries
{
	public:
	CreateAlignQueries(TracePtList<_locr,_locl> &trace_pts, OutputStreamController* output_file):
		trace_pts (trace_pts),
		output_file (output_file),
		writer (output_file),
		queue (VATOptions::threads()*1, writer)
	{ 

	}
	void operator()(unsigned thread_id)
	{
		Statistics st;
		size_t i=0;
		typename TracePtList<_locr,_locl>::QueryRange query_range (trace_pts.getRange());
		_buffer *buffer = 0;
		while(queue.get(i, buffer, query_range) && !exception_state()) 
		{
			try
			{
				alignQueries<_val, _locr, _locl, 1>(query_range.begin, query_range.end, *buffer, st);
				queue.push(i);
			} 
			catch(std::exception &e) 
			{
				exception_state.set(e);
				queue.wake_all();
			}
		}
		statistics += st;
	}
	TracePtList<_locr, _locl> &trace_pts;
	OutputStreamController* output_file;
	OutputWriter writer;
	Task_queue<_buffer,OutputWriter> queue;
};


template<typename _val, typename _locr, typename _locl>
void alignQueries(const TracePtBuffer<_locr,_locl> &trace_pts, OutputStreamController* output_file)
{
	TracePtList<_locr,_locl> v;
	for(unsigned bin=0;bin<trace_pts.bins();++bin) {
		//cout << "Processing query bin " << bin+1 << '/' << trace_pts.bins() << '\n';
		trace_pts.load(v, bin);

		merge_sort(v.begin(), v.end(), VATOptions::threads());
		v.init();
		if(refheader.n_blocks > 1) 
		{
			CreateAlignQueries<_val,_locr,_locl,TempOutputBuffer<_val> > context (v, output_file);
			launch_thread_pool(context, VATOptions::threads());
		} else 
		{
			CreateAlignQueries<_val,_locr,_locl,OutputBuffers<_val> > context (v, output_file);
			launch_thread_pool(context, VATOptions::threads());
		}
	}
}



#endif // __ALIGNQUERIES_H__