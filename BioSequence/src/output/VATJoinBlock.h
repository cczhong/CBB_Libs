#ifndef __VATJOINBLOCK_H__
#define __VATJOINBLOCK_H__

#include <algorithm>
#include <vector>
#include "BlockOutput.h"


using std::endl;
using std::cout;
using std::vector;

template<typename _val>
void JoinBlocks(unsigned ref_blocks, VATOutput &master_out, const vector<TempFile> &tmp_file)
{
	//	cout << "JoinBlocks" << endl;

	vector<BlockOutput*> files;
	vector<BlockOutput::Iterator> records;
	BlockOutput::Iterator r;
	for(unsigned i=0;i<ref_blocks;++i) 
    {
		files.push_back(new BlockOutput(i, tmp_file[i]));
		if(files.back()->next(r, std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max()))
			records.push_back(r);
	}

	std::make_heap(records.begin(), records.end());
	unsigned query, block, subject, n_target_seq = 0;
	query = block = subject = std::numeric_limits<unsigned>::max();
	int top_score=0;
	OutputBuffers<_val> buf;
	//cout << "JoinBlocks" << endl;
	while (!records.empty())
	{
		const BlockOutput::Iterator &next = records.front();
		const unsigned b = next.block_;

		if(next.info_.query_id != query) 
        {
			if(query != std::numeric_limits<unsigned>::max()) 
            {
				buf.finish_query_record();
				master_out.stream().write(buf.get_begin(), buf.size());
				buf.clear();
			}
			query = next.info_.query_id;
			n_target_seq = 0;
			top_score = next.info_.score;
			statistics.inc(Statistics::ALIGNED);
			buf.write_query_record(query);
		}
		const bool same_subject = n_target_seq > 0 && b == block && next.info_.subject_id == subject;
		if(VATOptions::outputRange(n_target_seq, next.info_.score, top_score) || same_subject) 
        {
			//printf("q=%u s=%u n=%u ss=%u\n",query, next.info_.subject_id, n_target_seq, same_subject, next.info_.score);
			VATOutput::write_record(buf, next.info_);
			statistics.inc(Statistics::MATCHES);
			if(!same_subject) {
				block = b;
				subject = next.info_.subject_id;
				++n_target_seq;
			}
		} else
			;

		std::pop_heap(records.begin(), records.end());
		records.pop_back();
		if(files[b]->next(r, subject, query)) 
        {
			records.push_back(r);
			std::push_heap(records.begin(), records.end());
		}
	}
	if(query != std::numeric_limits<unsigned>::max()) 
    {
		buf.finish_query_record();
		master_out.stream().write(buf.get_begin(), buf.size());
	}
	for(unsigned i=0;i<ref_blocks;++i) 
    {
		files[i]->close();
		delete files[i];
	}
}



#endif // __VATJOINBLOCK_H__