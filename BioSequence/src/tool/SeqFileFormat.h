
#ifndef SEQ_FILE_FORMAT_H_
#define SEQ_FILE_FORMAT_H_

#include <utility>
#include <string>


using std::pair;
using std::string;

template<typename _val>
class SequenceFileFormat
{
	public:

	virtual bool getSeq(vector<char> &id, vector<_val> &seq, InputStreamController &s) const = 0;
	virtual ~SequenceFileFormat()
	{ 
		
	}

	protected:

	template<typename _t>
	static void copyLine(InputStreamController &s, vector<_t> &v)
	{
		char a = 0;
		while(s.read(&a, 1) == 1 && a != '\n' && a != '\r')
			v.push_back(ValueFeature<_t>::from_char_(a));
		if(a == '\r')
			if(s.read(&a, 1) != 1 || a != '\n')
				throw FileFormatException ();
	}

	static void skipLine(InputStreamController &s)
	{
		char a = 0;
		while(s.read(&a, 1) == 1 && a != '\n' && a != '\r');
		if(a == '\r')
			if(s.read(&a, 1) != 1 || a != '\n')
				throw FileFormatException ();
	}

	static void copyUntil(InputStreamController &s, int delimiter, vector<_val> &v)
	{
		char a = 0;
		size_t col = 0;
		while(s.read(&a, 1) == 1 && a != delimiter) 
		{
			switch(a) 
			{
				case '\n':
					col = 0;
				case '\r':
					break;
				default:
					v.push_back(ValueFeature<_val>::from_char_(a));
					++col;
			}
		}
		if(a == delimiter) 
		{
			if(col > 0)
				throw FileFormatException ();
			else
				s.putback(a);
		}

	}


	static void skipChar(InputStreamController &s, char c)
	{
		char a;
		if(s.read(&a, 1) != 1 || a != c)
			throw FileFormatException ();
	}

	static bool haveChar(InputStreamController &s, char c)
	{
		char a;
		if(s.read(&a, 1) == 0)
			return false;
		else if (a != c)
			throw FileFormatException ();
		else
			return true;
	}

};

template<typename _val>
class FASTAFormat : public SequenceFileFormat<_val>
{
	public:

	FASTAFormat()
	{ }

	virtual bool getSeq(vector<char> &id, vector<_val> &seq, InputStreamController &s) const
	{
		if(!SequenceFileFormat<_val>::haveChar(s, '>'))
			return false;
		id.clear();
		seq.clear();
		SequenceFileFormat<_val>::copyLine(s, id);
		SequenceFileFormat<_val>::copyUntil(s, '>', seq);
		return true;
	}

	virtual ~FASTAFormat()
	{ 
		
	}

};

template<typename _val>
class FASTQFormat : public SequenceFileFormat<_val>
{
	public:

	FASTQFormat()
	{ 

	}

	virtual bool getSeq(vector<char> &id, vector<_val> &seq, InputStreamController &s) const
	{
		if(!SequenceFileFormat<_val>::haveChar(s, '@'))
			return false;
		id.clear();
		seq.clear();
		SequenceFileFormat<_val>::copyLine(s, id);
		SequenceFileFormat<_val>::copyLine(s, seq);
		SequenceFileFormat<_val>::skipChar(s, '+');
		SequenceFileFormat<_val>::skipLine(s);
		SequenceFileFormat<_val>::skipLine(s);
		return true;
	}

	virtual ~FASTQFormat()
	{ 

	}

};

template<typename _val>
const SequenceFileFormat<_val>* guessFormat(const string &file)
{
	static const FASTAFormat<_val> fasta;
	static const FASTQFormat<_val> fastq;

	InputStreamController f (file, true);
	char c;
	if(f.read(&c, 1) != 1)
		throw FileFormatException ();
	f.close();
	switch(c) {
	case '>': return &fasta;
	case '@': return &fastq;
	default: throw FileFormatException ();
	}
	return 0;
}

#endif /* SEQ_FILE_FORMAT_H_ */
