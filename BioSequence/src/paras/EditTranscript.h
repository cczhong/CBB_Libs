#ifndef __EDIT_TRANSCRIPT_H__
#define __EDIT_TRANSCRIPT_H__


#include <vector>
#include <iostream>
#include "PackedTranscript.h"
#include "../tool/TextBuffer.h"
#include "../sequence/Sequence.h"


using std::vector;
using std::endl;
class EditTranscript
{
	public:
	EditTranscript():
		begin_ (0),
		end_ (0)
	{ 

	}

	EditTranscript(const vector<char> &buf):
		begin_ (buf.size()),
		end_ (begin_)
	{ 

	}

	EditTranscript& setEnd(const vector<char> &buf)
	{
		end_ = buf.size();
		return *this;
	}

	vector<char>::const_iterator Begin(const vector<char> &buf) const
	{ 
		return buf.begin() + begin_; 
	}

	vector<char>::const_iterator End(const vector<char> &buf) const
	{ 
		return buf.begin() + end_;
	}

private:

	void Print_Matches(char *&ptr, unsigned &n)
	{
		if(n > 0) {
			ptr += sprintf(ptr, "%u", n);
			n = 0;
		}
	}

	size_t begin_, end_;

};

struct LinkIterator
{
	LinkIterator(const EditTranscript &right,
		const EditTranscript &left,
		const vector<char> &transcript_buf):
			i_ (left.Begin(transcript_buf)),
			left_end_ (left.End(transcript_buf)),
			right_begin_ (right.Begin(transcript_buf)),
			right_end_ (right.End(transcript_buf))
	{
		if(i_ >= left_end_-1)
			i_ = right_end_-1;
	}
	char operator*() const
	{ return *i_; }
	bool Good() const
	{ return i_ != right_begin_-1; }
	LinkIterator& operator++()
	{
		if(i_ >= right_begin_ && i_ < right_end_)
			--i_;
		else {
			++i_;
			if(i_ >= left_end_-1)
				i_ = right_end_-1;
		}
		return *this;
	}
private:
	vector<char>::const_iterator i_;
	const vector<char>::const_iterator left_end_, right_begin_, right_end_;
};
void static printNumber(TextBuffers &buf, unsigned n, EditOperations op)
{
	while(n>0) {
		unsigned m = std::min(n, 63u);
		buf.write(PackedOperations(op, m));
		n -= m;
	}
}

template <typename T>
void static printMatch(TextBuffers &buf, LinkIterator &i, const Sequence<const T> &query, const Sequence<const T> &subject, unsigned &qpos, unsigned &spos)
{
	unsigned n=0;
	for(;i.Good() && *i == op_match && query[qpos] == mask_critical(subject[spos]); ++i) {
		++qpos;
		++spos;
		++n;
	}
	printNumber(buf, n, op_match);
}

template <typename T>
void static printDeletion(TextBuffers &buf, LinkIterator &i, const Sequence<const T> &subject, unsigned &spos)
{
	for(;i.Good() && *i == op_deletion; ++i)
		buf.write(PackedOperations(op_deletion, mask_critical(subject[spos++])));
}

void static printInsertion(TextBuffers &buf, LinkIterator &i, unsigned &qpos)
{
	unsigned n = 0;
	for(;i.Good() && *i == op_insertion; ++i) {
		++n;
		++qpos;
	}
	printNumber(buf, n, op_insertion);
}

template <typename _val>
void static printPacked(const EditTranscript &right,
				  const EditTranscript &left,
				  const vector<char> &transcript_buf,
				  TextBuffers &buf,
				  const Sequence<const _val> &query,
				  const Sequence<const _val> &subject,
				  unsigned qpos,
				  unsigned spos)
{
	LinkIterator i (right, left, transcript_buf);
	while(i.Good())
		switch(*i) {
		case op_match:
			if(query[qpos] == mask_critical(subject[spos]))
				printMatch(buf, i, query, subject, qpos, spos);
			else {
				buf.write(PackedOperations(op_substitution, mask_critical(subject[spos])));
				++qpos;
				++spos;
				++i;
			}
			break;
		case op_insertion:
			printInsertion(buf, i, qpos);
			break;
		case op_deletion:
			printDeletion(buf, i, subject, spos);
		}
	buf.write(PackedOperations::terminator());
}

template <typename _val>
void static Print(LinkIterator i, std::ostream &os, const _val *s, EditOperations gap_op)
{
	unsigned n=0;
	for(; i.Good(); ++i) {
		os << n << ' ';
		if(*i == gap_op)
			os << '-';
		else
			os << ValueFeature<_val>::ALPHABET[*(s++)];
		++n;
	}
}

template <typename _val>
void static Print(std::ostream &os,
		   const _val *query,
		   const _val *subject,
		   const EditTranscript &right,
		   const EditTranscript &left,
		   const vector<char> &transcript_buf)
{
	LinkIterator i (right, left, transcript_buf);
	Print(i, os, query, op_deletion);
	os << endl;
	Print(i, os, subject, op_insertion);
	os << endl;
}

#endif // __EDIT_TRANSCRIPT_H__