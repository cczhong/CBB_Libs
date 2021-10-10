#ifndef __VATRECORD_H__
#define __VATRECORD_H__
#include "VATFile.h"
#include "../paras/PackedSequence.h"
#include <stdint.h>

//#include "../commons/Translate.h"
using std::string;
using std::vector;


template<typename _val>
struct DAA_match_record;

template<typename _val>
void translate_query(const vector<Nucleotide>& query, vector<_val> *context)
{
	context[0] = query;
	context[1] = query;
}

template<>
void translate_query<Protein>(const vector<Nucleotide>& query, vector<Protein> *context)
{
	Translator::translate(query, context);
}

template<typename _val>
struct DAA_query_record
{

	struct Match_iterator
	{
		Match_iterator(const DAA_query_record &parent, BinaryBuffers::Iterator it):
			r_ (parent),
			it_ (it),
			good_ (true)
		{ operator++(); }
		const DAA_match_record<_val>& operator*() const
		{ return r_; }
		const DAA_match_record<_val>* operator->() const
		{ return &r_; }
		bool good() const
		{ return good_; }
		Match_iterator& operator++()
		{ if(it_.good()) it_ >> r_; else good_ = false; return *this; }
	private:
		DAA_match_record<_val> r_;
		BinaryBuffers::Iterator it_;
		bool good_;
	};

	DAA_query_record(const VATFile& file, const BinaryBuffers &buf):
		file_ (file),
		it_ (init(buf))
	{
		cout << "DAA_query_record" << endl;
	}

	Match_iterator begin() const
	{ return Match_iterator (*this, it_); }

	string query_name;
	vector<Nucleotide> source_seq;
	vector<_val> context[6];

private:

	BinaryBuffers::Iterator init(const BinaryBuffers &buf)
	{
		BinaryBuffers::Iterator it (buf.begin());
		uint32_t query_len;
		it >> query_len;
		it >> query_name;
		uint8_t flags;
		it >> flags;
		if(file_.mode() == 3) {
			PackedSeq seq (it, query_len, false, 5);
			seq.unpack(context[0], 5, query_len);
		} else {
			const bool have_n = (flags&1) == 1;
			PackedSeq seq (it, query_len, have_n, have_n ? 3 : 2);
			seq.unpack(source_seq, have_n ? 3 : 2, query_len);
			translate_query<_val>(source_seq, context);
		}
		return it;
	}

	const VATFile& file_;
	const BinaryBuffers::Iterator it_;

	friend struct DAA_match_record<_val>;

	template<typename _val2> friend BinaryBuffers::Iterator& operator>>(BinaryBuffers::Iterator &it, DAA_match_record<_val2> &r);

};

template<typename _val>
struct DAA_match_record
{

	DAA_match_record(const DAA_query_record<_val> &query_record):
		parent_ (query_record)
	{ }

	const string& query_name() const
	{ return parent_.query_name; }

	const vector<_val>& query() const
	{ return parent_.context[frame]; }

	size_t db_letters() const
	{ return parent_.file_.db_letters(); }

	unsigned query_end() const
	{
		// if(parent_.file_.mode() == blastp) {
			return query_begin + translated_query_len - 1;
		// } else if(parent_.file_.mode() == blastx) {
		// 	int len = (int)translated_query_len*3*(frame>2 ? -1 : 1);
		// 	return (int)query_begin + (len > 0 ? -1 : 1) + len;
		// } else if(parent_.file_.mode() == blastn) {
		// 	int len = (int)translated_query_len*(frame>0 ? -1 : 1);
		// 	return (int)query_begin + (len > 0 ? -1 : 1) + len;
		// } else
			// return 0;
	}

	uint32_t total_subject_len, score, query_begin, subject_begin, frame, translated_query_begin, translated_query_len, subject_len, len, identities, mismatches, gap_openings;
	string subject_name;
	PackedTranscript transcript;

private:

	void parse()
	{
		translated_query_len = 0;
		subject_len = 0;
		len = 0;
		identities = 0;
		mismatches = 0;
		gap_openings = 0;
		unsigned d = 0;
		for(PackedTranscript::Const_Iterator<char> i = transcript.template begin<char>(); i.Good(); ++i) {
			len += i->count;
			switch(i->op) {
			case op_match:
				identities += i->count;
				translated_query_len += i->count;
				subject_len += i->count;
				d = 0;
				break;
			case op_substitution:
				mismatches += i->count;
				translated_query_len += i->count;
				subject_len += i->count;
				d = 0;
				break;
			case op_insertion:
				translated_query_len += i->count;
				++gap_openings;
				d = 0;
				break;
			case op_deletion:
				subject_len += i->count;
				if(d == 0)
					++gap_openings;
				d += i->count;
			}
		}
	}

	const DAA_query_record<_val> &parent_;

	template<typename _val2> friend BinaryBuffers::Iterator& operator>>(BinaryBuffers::Iterator &it, DAA_match_record<_val2> &r);

};

template<typename _val>
BinaryBuffers::Iterator& operator>>(BinaryBuffers::Iterator &it, DAA_match_record<_val> &r)
{
	uint32_t subject_id;
	//cout << "r.parent_.file_.ref_len(subject_id) =" <<subject_id<< endl;

	it >> subject_id;
	cout << "r.parent_.file_.ref_len(subject_id) =" <<subject_id<< endl;

	uint8_t flag;
	it >> flag;
	it.read_packed(flag & 3, r.score);
	it.read_packed((flag>>2)&3, r.query_begin);
	it.read_packed((flag>>4)&3, r.subject_begin);
	r.transcript.Read(it);
	//r.subject_name = r.parent_.file_.RefName(subject_id);
	// //cout << "r.parent_.file_.ref_len(subject_id) =" <<subject_id<< ",query_begin = "<<r.query_begin<<",subject_begin = "<<r.subject_begin<<endl;
	//r.total_subject_len = r.parent_.file_.RefLen(subject_id);
	// if(r.parent_.file_.mode() == 3) {
	// 	r.frame = (flag&(1<<6)) == 0 ? r.query_begin % 3 : 3+(r.parent_.source_seq.size() - 1 - r.query_begin)%3;
	// 	r.translated_query_begin = query_translated_begin<_val>(r.query_begin, r.frame, r.parent_.source_seq.size(), true);
	// } else if (r.parent_.file_.mode() == 2) {
		r.frame = 0;
		r.translated_query_begin = r.query_begin;
	// } else {
	// 	r.frame = (flag&(1<<6)) == 0 ? 0 : 1;
	// 	r.translated_query_begin = query_translated_begin<_val>(r.query_begin, r.frame, r.parent_.source_seq.size(), false);
	// }
	r.parse();
	return it;
}


#endif // __VATRECORD_H__