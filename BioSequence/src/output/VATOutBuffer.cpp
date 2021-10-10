\
#include "VATOutBuffer.h"
#include "../tool/TextBuffer.h"
#include "VATOutputForm.h"
#include "VATWriter.h"
#include "../commons/Match.h"
#include "../database/Queries.h"

unsigned get_length_flag(unsigned x)
{
	if(x <= (unsigned)std::numeric_limits<uint8_t>::max())
		return 0;
	else if(x <= (unsigned)std::numeric_limits<uint16_t>::max())
		return 1;
	else
		return 2;
}

// template<typename _val>
// unsigned get_rev_flag(unsigned frame)
// { 
// 	return frame > 0 ? 1 : 0; 
// }

// template<>
// unsigned get_rev_flag<Amino_acid>(unsigned frame)
// { 
// 	return frame > 2 ? 1 : 0; 
// }

// template<typename _val>
// uint8_t get_segment_flag(const Segment<_val> &match)
// {
// 	unsigned rev = get_rev_flag<_val>(match.frame_);
// 	return (uint8_t)(get_length_flag(match.score_)
// 				| (get_length_flag(match.traceback_->query_begin_)<<2)
// 				| (get_length_flag(match.traceback_->subject_begin_)<<4)
// 				| rev<<6);
// }

// template<typename _val>
// void write_intermediate_record(TextBuffers &buf,
// 			const Segment<_val> &match,
// 			size_t query_source_len,
// 			const sequence<const _val> &query,
// 			unsigned query_id,
// 			const vector<char> &transcript_buf)
// {
// 	buf.write(query_id)
// 		.write(ref_map.get<_val>(current_ref_block, match.subject_id_))
// 		.write(get_segment_flag(match))
// 		.write_pack(match.score_)
// 		.write_pack(match.traceback_->query_begin_)
// 		.write_pack(match.traceback_->subject_begin_);
// 	const unsigned qbegin = query_translated_begin<_val>(match.traceback_->query_begin_, match.frame_, query_source_len, false);
// 	Print_Packed(match.traceback_->transcript_right_, match.traceback_->transcript_left_, transcript_buf, buf, query, ReferenceSeq<_val>::get()[match.subject_id_], qbegin, match.traceback_->subject_begin_);
// }

