#ifndef __VATOUTPUTFORM_H__
#define __VATOUTPUTFORM_H__

#include "../paras/Match.h"
#include "MatchFunction.h"
#include "VATFile.h"
#include "VATRecord.h"

template<typename _val>
class OutputFormat
{
    public:
	virtual void print_match(const DAA_match_record<_val> &r, TextBuffers &out) const = 0;

	virtual void print_header(OutputStreamController &f) const
	{
		//cout << "print_header.." << endl;
	}
	virtual ~OutputFormat()
	{ 

    }
};

template<typename _val>
class BLASTTabFormat : public OutputFormat<_val>
{
	public:

	BLASTTabFormat()
	{
		//cout << "BLASTTabFormat" << endl;
	}

	virtual void print_match(const DAA_match_record<_val> &r, TextBuffers &out) const
	{
		out << r.query_name() << '\t'
				<< r.subject_name << '\t'
				<< (double)r.identities*100/r.len << '\t'
				<< r.len << '\t'
				<< r.mismatches << '\t'
				<< r.gap_openings << '\t'
				<< r.query_begin+1 << '\t'
				<< r.query_end()+1 << '\t'
				<< r.subject_begin+1 << '\t'
				<< r.subject_begin+r.subject_len << '\t';

		cout << r.query_name() << '\t'
			 << r.subject_name << '\t'
			 << (double)r.identities * 100 / r.len << '\t'
			 << r.len << '\t'
			 << r.mismatches << '\t'
			 << r.gap_openings << '\t'
			 << r.query_begin + 1 << '\t'
			 << r.query_end() + 1 << '\t'
			 << r.subject_begin + 1 << '\t'
			 << r.subject_begin + r.subject_len << '\t' << endl;
		//out.print_e(score_matrix::get().evalue(r.score, r.db_letters(), r.query().size()));
		//out << '\t' << score_matrix::get().bitscore(r.score) << '\n';
	}

	virtual ~BLASTTabFormat()
	{ 

    }

	static size_t print_salltitles(TextBuffers &buf, const char *id)
	{
		size_t n = 0;
		const vector<string> t (tokenize(id, "\1"));
		vector<string>::const_iterator i=t.begin();
		for(;i<t.end()-1;++i) {
			buf << *i << "<>";
			n += i->length() + 2;
		}
		buf << *i;
		n += i->length();
		return n;
	}

};

template<typename _val>
const OutputFormat<_val>& getOutputFormat()
{
	static const BLASTTabFormat<_val> tab;
	if(VATOptions::output_format == "tab")
		return tab;
	else
		throw std::runtime_error("Invalid output format.");
}


#endif // __VATOUTPUTFORM_H__