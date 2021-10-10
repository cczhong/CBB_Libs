#ifndef __BLOCKOUTPUT_H__
#define __BLOCKOUTPUT_H__

#include <string>
#include "VATOutBuffer.h"

using std::string;

class BlockOutput : public BufferedFile
{
    public:
	struct Iterator
	{
		unsigned block_;
		bool same_subject_;
		IntermediateRecord info_;
		bool operator<(const Iterator &rhs) const
		{ 
            return info_.query_id > rhs.info_.query_id ||
				(info_.query_id == rhs.info_.query_id && (rhs.same_subject_ ||
						(!rhs.same_subject_ && info_.score < rhs.info_.score))); 
        }
	};

	bool next(Iterator &it, unsigned subject, unsigned query)
	{
		if(this->eof())
			return false;
		it.info_.read(*this);
		it.block_ = block_;
		it.same_subject_ = it.info_.subject_id == subject && it.info_.query_id == query;
		return true;
	}

	BlockOutput(unsigned ref_block, const TempFile &tmp_file):
		BufferedFile (tmp_file),
		block_ (ref_block)
	{ 

    }

private:

	const unsigned block_;

};


#endif // __BLOCKOUTPUT_H__