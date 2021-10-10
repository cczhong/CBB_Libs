
#ifndef LOAD_SEQS_H_
#define LOAD_SEQS_H_

#include <iostream>
#include "SequenceSet.h"
#include "../paras/Translator.h"
#include "../tool/SeqFileFormat.h"
//#include "stringset.h"

class SingleStrand { };
class DoubleStrand { };

//get Seq size and return to StringSet<_val> ss
/*
template <typename _ival, typename _val, typename _strand>
size_t pushSeq(BaseStringSet<_val> &ss, BaseStringSet<Nucleotide> &source_seqs, const vector<_ival> &seq)
{
	//cout << "pushSeq" << endl;
	ss.push_back(seq);
	return seq.size();
}
*/

size_t pushSeq(BaseStringSet<Protein> &ss, const vector<Protein> &seq)
{
	ss.push_back(seq);
	return seq.size();
}

// template<>
// size_t pushSeq<Protein,Nucleotide,SingleStrand>(BaseStringSet<Nucleotide> &ss, BaseStringSet<Nucleotide>& source_seqs, const vector<Protein> &seq)
// { 
// 	cout << "pushSeq<Protein,Nucleotide,SingleStrand>" << endl;
// 	ss.push_back(seq);
// 	return seq.size();
// 	//return 0; 
// }

// template<>
// size_t pushSeq<Nucleotide,Protein,DoubleStrand>(BaseStringSet<Protein> &ss, BaseStringSet<Nucleotide>& source_seqs, const vector<Nucleotide> &seq)
// {
// 	source_seqs.push_back(seq);
// 	if(seq.size() < 2) 
// 	{
// 		for(unsigned j=0;j<6;++j)
// 		{
// 			ss.fill(0, ValueFeature<Protein>::mask_char);
// 		}
// 		return 0;
// 	}

// 	vector<Protein> proteins[6];
// 	size_t n = Translator::translate(seq, proteins);

// 	unsigned optimize_frames (Translator::getFrames(proteins, VATOptions::getRunLen(seq.size()/3)));
// 	for(unsigned j = 0; j < 6; ++j) 
// 	{
// 		if(optimize_frames & (1 << j))
// 			ss.push_back(proteins[j]);
// 		else
// 			ss.fill(proteins[j].size(), ValueFeature<Protein>::mask_char);
// 	}
// 	return n;
// }

// template<>
// size_t pushSeq<Nucleotide,Nucleotide,DoubleStrand>(BaseStringSet<Nucleotide> &ss, BaseStringSet<Nucleotide>& source_seqs, const vector<Nucleotide> &seq)
// {
// 	ss.push_back(seq);
// 	ss.push_back(Translator::reverse(seq));
// 	return seq.size()*2;
// }
/*
//get num of sequence, return ids and seqs
template <typename _ival, typename _val, typename _strand>
size_t loadingSequence(InputStreamController &file,
					   const SequenceFileFormat<_ival> &format,
					   SequenceSet<_val> **seqs,
					   BaseStringSet<char, 0> *&ids,
					   SequenceSet<Nucleotide> *&source_seqs,
					   size_t max_letters)
{
	*seqs = new SequenceSet<_val> ();
	ids = new BaseStringSet<char,0> ();
	source_seqs = new SequenceSet<Nucleotide>();
	size_t letters = 0, n = 0;
	vector<_ival> seq;
	vector<char> id;
	try {
		while(letters < max_letters && format.getSeq(id, seq, file)) 
		{
			ids->push_back(id);
			letters += pushSeq<_ival,_val,_strand>(**seqs, *source_seqs, seq);
			++n;
		}
	} catch(InvalidSeqCharException &e) 
	{
		std::cerr << n << endl;
		throw e;
	}
	ids->finishReserve();
	(*seqs)->finishReserve();
	source_seqs->finishReserve();
	if(n == 0) {
		delete *seqs;
		delete ids;
		delete source_seqs;
	}
	return n;
};
*/

size_t loadingSequence(InputStreamController &file,
					   const SequenceFileFormat<Protein> &format,
					   SequenceSet<Protein> **seqs,
					   BaseStringSet<char, 0> *&ids,
					   SequenceSet<Nucleotide> *&source_seqs,
					   size_t max_letters)
{
	*seqs = new SequenceSet<Protein> ();
	ids = new BaseStringSet<char,0> ();
	source_seqs = new SequenceSet<Nucleotide>();
	size_t letters = 0, n = 0;
	vector<Protein> seq;
	vector<char> id;
	try 
	{
			while(letters < max_letters && format.getSeq(id, seq, file)) 
			{
				ids->push_back(id);
				letters += pushSeq(**seqs, seq);
				++n;
			}
	} 
	catch(InvalidSeqCharException &e) 
	{
		std::cerr << n << endl;
		throw e;
	}
	ids->finishReserve();
	(*seqs)->finishReserve();
	source_seqs->finishReserve();
	if(n == 0) 
	{
		delete *seqs;
		delete ids;
		delete source_seqs;
	}
	return n;
};

#endif /* LOAD_SEQS_H_ */
