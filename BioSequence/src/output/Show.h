#ifndef __SHOW_H__
#define __SHOW_H__
#include "../tool/BinaryFile.h"
#include "../tool/TextBuffer.h"
#include "./VATFile.h"
#include "../tool/BinaryBuffer.h"
#include "../paras/SequenceType.h"

const unsigned showbufsize = 32;

class ShowWriter
{
    public:
	ShowWriter():
		f_ (VATOptions::output_file + (VATOptions::compression==1?".gz":""),
				VATOptions::compression==1,
				VATOptions::output_file.length()==0 ? OutputStreamController::stdout_sink : OutputStreamController::file_sink)
	{
		//cout << "ShowWriter" << endl;
	}
	void operator()(TextBuffers &buf)
	{
		f_.write(buf.get_begin(), buf.size());
		buf.clear();
	}
	OutputStreamController f_;
};


class ShowGetter
{
    public:
	ShowGetter(VATFile &daa):
		daa (daa)
	{
		cout << "ShowGetter" << endl;
	}
	bool operator()()
	{
		n = 0;
		for(unsigned i=0; i<showbufsize; ++i)
			if(!daa.ReadQueryBuffer(buf[i]))
				return false;
			else
				++n;
		cout << "n = " <<n<<endl;
		return true;
		//cout << "n = " <<n<<endl;

	}

	BinaryBuffers buf[showbufsize];
	unsigned n;
	VATFile &daa;
};


template<typename _val>
class ShowEnvironment
{
    public:
	ShowEnvironment(VATFile &daa, ShowWriter &writer, const OutputFormat<_val> &format):
		daa (daa),
		writer (writer),
		queue (2*1, writer),
		format (format)
	{

	}
	void operator()(unsigned thread_id)
	{
		try 
        {
			size_t n;
			ShowGetter query_buf (daa);
			TextBuffers *buffer = 0;
			 while(queue.get(n, buffer, query_buf)) 
             {
			// 	cout << "-----------" << endl;
			// cout << "query_buf = " <<query_buf.n <<""<<endl;
				for (unsigned j = 0; j < query_buf.n; ++j)
				{
					//cout << "-----------" << endl;
					DAA_query_record<_val> r(daa, query_buf.buf[j]);
					//cout << "-----------" << endl;
					for (typename DAA_query_record<_val>::Match_iterator i = r.begin(); i.good(); ++i)
					{
						if (i->frame > 2 && 1)
							continue;
						format.print_match(*i, *buffer);
					}
					}
					queue.push(n);
				}
		} 
        catch(std::exception &e) 
        {
			exception_state.set(e);
			//queue.wake_all();
		}
	}
	VATFile &daa;
	ShowWriter &writer;
	Task_queue<TextBuffers,ShowWriter> queue;
	const OutputFormat<_val> &format;
};


template<typename _val>
void Show(VATFile &daa)
{
	//              daa.score_matrix(),
	// 				daa.gap_open_penalty(),
	// 				daa.gap_extension_penalty(),
	// 				daa.match_reward(),
	// 				daa.mismatch_penalty(),
	// 				_val ()));

	cout << "Build version = " << daa.VATBuild()<< endl;
	cout << "DB sequences = " << daa.DBSeqs()<< endl;
	cout << "DB sequences used = " << daa.DBSeqsUsed() << endl;
	cout << "DB letters = " << daa.DBletters() << endl;
	
	ShowWriter writer;
	const OutputFormat<_val>& format (getOutputFormat<_val>());
	format.print_header(writer.f_);
	ShowEnvironment<_val> context(daa, writer, format);

	launch_thread_pool(context, 1);

}

void ShowResults()
{
	//VATOptions::daa_file = "match.vout";
	//cout << "ShowResults = " <<VATOptions::daa_file<< endl;
	VATFile daa(VATOptions::daa_file);
	//cout << "DBletters= " <<daa.DBletters()<< endl;
	//cout << "VATOptions::daa_file" << endl;
	Show<Protein>(daa);
	cout << "End" << endl;

}



#endif // __SHOW_H__