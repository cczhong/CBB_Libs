#ifndef __VATWRITER_H__
#define __VATWRITER_H__
#include "VATFile.h"
#include "../paras/PackedTranscript.h"
#include "../tool/TextBuffer.h"
#include "../sequence/Sequence.h"
#include "../paras/Options.h"
#include "MatchFunction.h"
#include "../paras/NuclMatrix.h"
#include "../sequence/Reference.h"
#include "../paras/PackedSequence.h"
class IntermediateRecord
{
    public:
	void read(BufferedFile &f)
	{
		f.read(query_id);
		f.read(subject_id);
		f.read(flag);
		f.read_packed(flag & 3, score);
		f.read_packed((flag>>2)&3, query_begin);
		f.read_packed((flag>>4)&3, subject_begin);
		transcript.Read(f);
	}
	uint32_t query_id, subject_id, score, query_begin, subject_begin;
	uint8_t flag;
	PackedTranscript transcript;
};

class VATOutput
{

    public:
	VATOutput():
		f_ (VATOptions::daa_file),
		v2_ (refheader.sequences,
				VATOptions::db_size == 0 ? refheader.letters : VATOptions::db_size,
				nm.getGapOpen(),
				nm.getGapExtend(),
				2,
				-3,
				nm.getK(),
				nm.getLambda(),
				"VAT Matrix",
				(AlignMode)VATOptions::cmd)
	{
		VATHeaderOne h1;
		f_.write(&h1, 1);
		v2_.block_type[0] = VATHeaderTwo::alignments;
		v2_.block_type[1] = VATHeaderTwo::ref_names;
		v2_.block_type[2] = VATHeaderTwo::ref_lengths;
		f_.write(&v2_, 1);
	}

	template<typename _val>
	static void write_query_record(TextBuffers &buf, const Sequence<const char> &query_name, const Sequence<const _val> &query)
	{
		cout << "write_query_record"<<endl;
		cout << "query_name.c_str() = "<<query_name.cStr()<<"query.length() = "<<query.length()<<endl;
		buf.write((uint32_t)0);
		uint32_t l = query.length();
		buf.write(l);
		buf.write_c_str(query_name.cStr(), find_first_of(query_name.cStr(), ConstParameter::id_delimiters));
		PackedSeq s (query);
		//cout << "PackedSeq = "<<s.<<endl;
		uint8_t flags = s.has_n() ? 1 : 0;
		buf.write(flags);
		buf << s.data();
	}

	static void write_record(TextBuffers &buf, const IntermediateRecord &r)
	{
		//cout << "write_record"<<endl;

		//cout << "r.subject_id =" <<r.subject_id<< ", r.flag = "<<r.flag<<endl;
		buf.write(r.subject_id).write(r.flag);
		buf.write_pack(r.score);
		buf.write_pack(r.query_begin);
		buf.write_pack(r.subject_begin);
		buf << r.transcript.data();
	}

	template<typename _val>
	static void write_record(TextBuffers &buf,
			const Segment<_val> &match,
			size_t query_source_len,
			const Sequence<const _val> &query,
			unsigned query_id,
			const vector<char> &transcript_buf)
	{
		NucleotideMatrix nm;
		buf << QueryIds::get()[query_id].cStr() << '\t'
				<< ReferenceIds::get()[match.subject_id_].cStr()<< '\t'
				<< (double)(match.traceback_->identities_)*100/(match.traceback_->len_)<< '\t'
				<< match.traceback_->len_<< '\t'
				<< match.traceback_->mismatches_<< '\t'
				<< match.traceback_->gap_openings_<< '\t'
				<< match.traceback_->query_begin_ + 1<< '\t'
				<< match.traceback_->query_begin_ + match.traceback_->query_len_<< '\t'
				<< match.traceback_->subject_begin_+1 << '\t'
				<< match.traceback_->subject_begin_+match.traceback_->subject_len_<< '\t'
				<< nm.bitscore(match.traceback_->score_)<< '\t'
				<< nm.evalue(nm.bitscore(match.traceback_->score_),refheader.letters,query_source_len)<<'\t'
				<<'\n';
		// int q_len = query.length();
		// match.traceback_
		// cout << "q len = " <<q_len<< ",query_source_len = "<<query_source_len<<endl;
		// size_t len = ReferenceIds::get()[match.subject_id_].
		// cout << "db len" << refheader.letters<< endl;
		// out.print_e(score_matrix::get().evalue(r.score, r.db_letters(), r.query().size()));
		// out << '\t' << score_matrix::get().bitscore(r.score) << '\n';

		// cout << "subject_begin_ = " <<match.traceback_->subject_begin_<< endl;
		// //cout << "subject_begin_ = " <<match->traceback_->subject_begin_<< endl;
		// cout << "query_begin_ = " <<match.traceback_->query_begin_<< endl;
		// cout << "subject_len_ = " <<match.traceback_->subject_len_<< endl;
		// cout << "query = " <<query_ids::get()[query_id].c_str()<< endl;
		// //cout << "current_ref_block = " <<current_ref_block<<",match.subject_id_ = "<<match.subject_id_<<endl;
		// cout << "ref id =" <<ref_ids::get()[match.subject_id_].c_str()<< endl;

		// cout << "current_ref_block = "<<current_ref_block<<", subject_id_= "<<match.subject_id_<<", match.traceback_->query_begin_ = "<<match.traceback_->query_begin_<<",match.traceback_->subject_begin = "<<match.traceback_->subject_begin_<<",match.traceback_->subject_begin_ = "<<match.traceback_->subject_begin_<<endl;
		buf.write(ref_map.get<_val>(current_ref_block, match.subject_id_));
		buf.write(getSegmentFlag(match));
		//cout << "match.score_ = " <<match.score_<<endl;
		buf.write_pack(match.score_);
		buf.write_pack(match.traceback_->query_begin_);
		buf.write_pack(match.traceback_->subject_begin_);
		const unsigned qbegin = queryTranslatedBegin<_val>(match.traceback_->query_begin_, match.frame_, query_source_len, false);
		//cout << "qbegin = " <<qbegin<<endl;
		printPacked(match.traceback_->transcript_right_, match.traceback_->transcript_left_, transcript_buf, buf, query, ReferenceSeq<_val>::get()[match.subject_id_], qbegin, match.traceback_->subject_begin_);
	}

	void finish()
	{
		uint32_t size = 0;
		f_.write(&size, 1);
		v2_.block_size[0] = f_.tell() - sizeof(VATHeaderOne) - sizeof(VATHeaderTwo);
		v2_.db_seqs_used = ref_map.next_;
		v2_.query_records = statistics.get(Statistics::ALIGNED);
		size_t s = 0;
		for(ptr_vector<string>::const_iterator i = ref_map.name_.begin(); i != ref_map.name_.end(); ++i) 
		{
			f_.write_c_str(*i);
			s += i->length()+1;
		}
		v2_.block_size[1] = s;

		f_.write(ref_map.len_, false);
		v2_.block_size[2] = ref_map.len_.size() * sizeof(uint32_t);
		f_.seek(sizeof(VATHeaderOne));
		f_.write(&v2_, 1);
		f_.close();
	}

	
	OutputStreamController& stream()
	{ 
        return f_; 
    }
	NucleotideMatrix nm;

private:
	OutputStreamController f_;
	VATHeaderTwo v2_;

};

#endif // __VATWRITER_H__