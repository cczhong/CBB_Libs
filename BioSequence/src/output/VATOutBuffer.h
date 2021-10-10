#ifndef __VATBUFFER_H__
#define __VATBUFFER_H__


#include "../tool/TextBuffer.h"
#include "VATOutputForm.h"
#include "VATWriter.h"
#include "../paras/Match.h"
#include "../sequence/Queries.h"

unsigned getLengthFlag(unsigned x)
{
	if(x <= (unsigned)std::numeric_limits<uint8_t>::max())
		return 0;
	else if(x <= (unsigned)std::numeric_limits<uint16_t>::max())
		return 1;
	else
		return 2;
}

template <typename _val>
unsigned getRevFlag(unsigned frame)
{
	return frame > 0 ? 1 : 0;
}

template<>
unsigned getRevFlag<Protein>(unsigned frame)
{
	return frame > 2 ? 1 : 0;
}

template <typename _val>
uint8_t getSegmentFlag(const Segment<_val> &match)
{
	unsigned rev = getRevFlag<_val>(match.frame_);
	return (uint8_t)(getLengthFlag(match.score_)
				| (getLengthFlag(match.traceback_->query_begin_)<<2)
				| (getLengthFlag(match.traceback_->subject_begin_)<<4)
				| rev<<6);
}

template <typename _val>
void write_intermediate_record(TextBuffers &buf,
							   const Segment<_val> &match,
							   size_t query_source_len,
							   const Sequence<const _val> &query,
							   unsigned query_id,
							   const vector<char> &transcript_buf)
{
	//cout << "query_id" <<query_id<< endl;
	buf.write(query_id)
		.write(ref_map.get<_val>(current_ref_block, match.subject_id_))
		.write(getSegmentFlag(match))
		.write_pack(match.score_)
		.write_pack(match.traceback_->query_begin_)
		.write_pack(match.traceback_->subject_begin_);
	const unsigned qbegin = queryTranslatedBegin<_val>(match.traceback_->query_begin_, match.frame_, query_source_len, false);
	printPacked(match.traceback_->transcript_right_, match.traceback_->transcript_left_, transcript_buf, buf, query, ReferenceSeq<_val>::get()[match.subject_id_], qbegin, match.traceback_->subject_begin_);
}
template<typename _val>
class OutputBuffers : public TextBuffers
{
	public:
	virtual void print_match(const Segment<_val> &match,
		size_t query_source_len,
		const Sequence<const _val> &query,
		unsigned query_id,
		const vector<char> &transcript_buf)
	{
		VATOutput vo;
		vo.write_record(*this, match, query_source_len, query, query_id, transcript_buf);
	}
	virtual void write_query_record(unsigned query_id)
	{
		query_begin_ = this->size();
		VATOutput vo;
		vo.write_query_record(*this, QueryIds::get()[query_id], QuerySeqs<_val>::get()[query_id]);
	}
	virtual void finish_query_record()
	{ 
		*(uint32_t*)(this->data_+query_begin_) = this->size() - query_begin_ - sizeof(uint32_t); 
	}
	virtual ~OutputBuffers()
	{ 

	}
private:
	size_t query_begin_;
};

template<typename _val>
class TempOutputBuffer : public OutputBuffers<_val>
{
	public:
	virtual void print_match(const Segment<_val> &match,
				size_t query_source_len,
				const Sequence<const _val> &query,
				unsigned query_id,
				const vector<char> &transcript_buf)
	{ 
		write_intermediate_record(*this, match, query_source_len, query, query_id, transcript_buf); 
	}
	virtual void write_query_record(unsigned query_id)
	{ 

	}
	virtual void finish_query_record()
	{ 
		
	}
	virtual ~TempOutputBuffer()
	{ 

	}
};

#endif // __VATBUFFER_H__