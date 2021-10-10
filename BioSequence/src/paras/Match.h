#ifndef MATCH_H_
#define MATCH_H_

#include "../sequence/Sequence.h"
#include "../tool/AsyncBuffer.h"
#include "EditTranscript.h"
#include "../tool/Utils.h"
#include "PackedLocation.h"

enum Strand { FORWARD, REVERSE };

interval normalizedRange(unsigned pos, int len, Strand strand);

template<typename _locr, typename _locl>
class Hit
{
	public:
	typedef typename PackedSequenceLocation<_locr>::type packed_loc;
	unsigned	query_;
	packed_loc	subject_;
	_locl		seed_offset_;
	Hit():
		query_ (),
		subject_ (),
		seed_offset_ ()
	{ 

	}
	Hit(unsigned query, _locr subject, _locl seed_offset):
		query_ (query),
		subject_ (subject),
		seed_offset_ (seed_offset)
	{ 

	}
	bool operator<(const Hit &rhs) const
	{ return query_ < rhs.query_; }
	bool blank() const
	{ return subject_ == 0; }
	unsigned operator%(unsigned i) const
	{ return (query_/6) % i; }
	unsigned operator/(unsigned i) const
	{ return (query_/6)/i; }
	int64_t global_diagonal() const
	{ return (int64_t)subject_ - (int64_t)seed_offset_; }
	template<unsigned _d>
	static unsigned query_id(const Hit& x)
	{ return x.query_/_d; }
	template<unsigned _d>
	class QueryId
	{
		public:
		unsigned operator()(const Hit& x) const
		{ return query_id<_d>(x); }
	};
	static bool cmpSubject(const Hit &lhs, const Hit &rhs)
	{ return lhs.subject_ < rhs.subject_; }
	static bool cmpNormalizedSubject(const Hit &lhs, const Hit &rhs)
	{
		const uint64_t x = (uint64_t)lhs.subject_ + (uint64_t)rhs.seed_offset_, y = (uint64_t)rhs.subject_ + (uint64_t)lhs.seed_offset_;
		return x < y || (x == y && lhs.seed_offset_ < rhs.seed_offset_);
	}
	friend std::ostream& operator<<(std::ostream &s, const Hit &me)
	{
		s << me.query_ << '\t' << me.subject_ << '\t' << me.seed_offset_ << '\n';
		return s;
	}
} __attribute__((packed));

template<typename _val>
class LocalMatch
{
	public:
	typedef typename vector<LocalMatch>::iterator iterator;
	LocalMatch():
		len_ (),
		query_begin_ (),
		subject_len_ (),
		gap_openings_ (),
		identities_ (),
		mismatches_ (),
		subject_begin_ (),
		score_ (),
		query_len_ (),
		query_anchor_ (0),
		subject_ (0)
	{ 

	}
	LocalMatch(int score):
		len_ (0),
		query_begin_ (0),
		subject_len_ (0),
		gap_openings_ (0),
		identities_ (0),
		mismatches_ (0),
		subject_begin_ (0),
		score_ (score),
		query_len_ (0),
		query_anchor_ (0),
		subject_ (0)
	{ 

	}
	LocalMatch(int query_anchor, const _val *subject):
		len_ (0),
		query_begin_ (0),
		subject_len_ (0),
		gap_openings_ (0),
		identities_ (0),
		mismatches_ (0),
		subject_begin_ (0),
		score_ (0),
		query_len_ (0),
		query_anchor_ (query_anchor),
		subject_ (subject)
	{ 
		
	}
	LocalMatch(unsigned len, unsigned query_begin, unsigned query_len, unsigned subject_len, unsigned gap_openings, unsigned identities, unsigned mismatches, signed subject_begin, signed score):
		len_ (len),
		query_begin_ (query_begin),
		subject_len_ (subject_len),
		gap_openings_ (gap_openings),
		identities_ (identities),
		mismatches_ (mismatches),
		subject_begin_ (subject_begin),
		score_ (score),
		query_len_ (query_len),
		query_anchor_ (0),
		subject_ (0)
	{ }
	LocalMatch& operator+=(const LocalMatch& rhs)
	{
		add(rhs);
		transcript_right_ = rhs.transcript_right_;
		return *this;
	}
	LocalMatch& operator-=(const LocalMatch& rhs)
	{
		add(rhs);
		query_begin_ = rhs.query_len_;
		subject_begin_ = rhs.subject_len_;
		transcript_left_ = rhs.transcript_right_;
		return *this;
	}
	void add(const LocalMatch &rhs)
	{
		len_ += rhs.len_;
		subject_len_ += rhs.subject_len_;
		gap_openings_ += rhs.gap_openings_;
		identities_ += rhs.identities_;
		mismatches_ += rhs.mismatches_;
		score_ += rhs.score_;
		query_len_ += rhs.query_len_;
	}
	interval queryRange(Strand strand) const
	{ return normalizedRange(query_begin_, query_len_, strand); }
	interval subjectRange() const
	{ return normalizedRange(subject_begin_, subject_len_, FORWARD); }
	void print(const Sequence<const _val> &query, const Sequence<const _val> &subject, const vector<char> &buf) const
	{
		cout << "Score = " << score_ << endl;
		::print(cout, &query[query_begin_], &subject[subject_begin_], transcript_right_, transcript_left_, buf);
	}
	unsigned len_, query_begin_, subject_len_, gap_openings_, identities_, mismatches_;
	signed subject_begin_, score_, query_len_, query_anchor_;
	const _val *subject_;
	EditTranscript transcript_right_, transcript_left_;
};

template<typename _val>
class Segment
{
	public:
	Segment(int score,
			unsigned frame,
			LocalMatch<_val> *traceback = 0,
			unsigned subject_id = std::numeric_limits<unsigned>::max()):
		score_ (score),
		frame_ (frame),
		traceback_ (traceback),
		subject_id_ (subject_id),
		next_ (0),
		top_score_ (0)
	{ 

	}
	Strand strand() const
	{ 
		return frame_ < 3 ? FORWARD : REVERSE; 
	}
	interval query_range() const
	{ 
		return traceback_->query_range(strand()); 
	}
	interval subject_range() const
	{ 
		return traceback_->subject_range(); 
	}
	bool operator<(const Segment &rhs) const
	{ 
		return top_score_ > rhs.top_score_
			|| (top_score_ == rhs.top_score_
			&& (subject_id_ < rhs.subject_id_ || (subject_id_ == rhs.subject_id_ && (score_ > rhs.score_ || (score_ == rhs.score_ && traceback_->score_ > rhs.traceback_->score_))))); }
	static bool compSubject(const Segment& lhs, const Segment &rhs)
	{ 
		return lhs.subject_id_ < rhs.subject_id_ || (lhs.subject_id_ == rhs.subject_id_ && lhs.score_ > rhs.score_); 
	}
	class Subject
	{
		public:
		unsigned operator()(const Segment& x) const
		{ 
			return x.subject_id_; 
		}
	};
	int						score_;
	unsigned				frame_;
	LocalMatch<_val>	   *traceback_;
	unsigned				subject_id_;
	Segment				   *next_;
	int						top_score_;
};

#endif /* MATCH_H_ */
