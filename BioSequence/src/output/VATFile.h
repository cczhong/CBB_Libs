#ifndef __VATFILE_H__
#define __VATFILE_H__

#include <iostream>
#include <string.h>
#include <vector>
#include "../paras/ConstParameter.h"
#include "../tool/BinaryBuffer.h"
#include "../tool/BinaryFile.h"
#include <boost/ptr_container/ptr_vector.hpp>

using std::string;
using std::vector;
class VATHeaderOne
{
    public:
	VATHeaderOne():
		magic_number (0x3c0e53476d3ee36bllu),
		version (0)
	{ 

    }
	uint64_t magic_number, version;
};

typedef enum { search=4 } AlignMode;

class VATHeaderTwo
{
    public:
	VATHeaderTwo()
	{ 
        
    }
	VATHeaderTwo(uint64_t db_seqs,
			uint64_t db_letters,
			int32_t gap_open,
			int32_t gap_extend,
			int32_t reward,
			int32_t penalty,
			double k,
			double lambda,
			const string &score_matrix,
			AlignMode mode):
		vat_build (ConstParameter::build_version),
		db_seqs (db_seqs),
		db_seqs_used (0),
		db_letters (db_letters),
		flags (0),
		query_records (0),
		mode (mode),
		gap_open (gap_open),
		gap_extend (gap_extend),
		reward (reward),
		penalty (penalty),
		reserved1 (0),
		reserved2 (0),
		reserved3 (0),
		k (k),
		lambda (lambda),
		reserved4 (0),
		reserved5 (0)
	{
		memset(block_type, 0, sizeof(block_type));
		memset(block_size, 0, sizeof(block_size));
		strcpy(this->score_matrix, score_matrix.c_str());
	}
	typedef enum { empty = 0, alignments = 1, ref_names = 2, ref_lengths = 3 } BlockType;
	uint64_t vat_build, db_seqs, db_seqs_used, db_letters, flags, query_records;
	int32_t mode, gap_open, gap_extend, reward, penalty, reserved1, reserved2, reserved3;
	double k, lambda, reserved4, reserved5;
	char score_matrix[16];
	uint64_t block_size[256];
	char block_type[256];
};

class VATFile
{
    public:
	VATFile(const string& file_name):
		f_ (file_name)
	{
		//cout << "1..........." << endl;
		f_.read(&v1_, 1);
		if(v1_.magic_number != VATHeaderOne().magic_number)
			throw std::runtime_error("Input file is not a VAT file.");
		if(v1_.version > ConstParameter::vat_version)
			throw std::runtime_error("VAT version requires later version of VAT.");
		f_.read(&v2_, 1);
		//cout << "2.v2_.block_size[0] = " <<v2_.block_size[0]<< endl;

		if(v2_.block_size[0] == 0)
			throw std::runtime_error("Invalid VAT file. VAT run probably has not completed successfully.");
		cout << "3..........." << endl;

		f_.seek(sizeof(VATHeaderOne) + sizeof(VATHeaderTwo) + v2_.block_size[0]);
		string s;
		//cout << "v2_.db_seqs_used= " <<v2_.db_seqs_used<< endl;
		ref_name_.reserve(v2_.db_seqs_used);
		for(uint64_t i=0;i<v2_.db_seqs_used;++i) {
			f_.read_c_str(s);
			//cout << "s = " <<s<<endl;
			ref_name_.push_back(new string(s));
		}
		ref_len_.resize(v2_.db_seqs_used);
		f_.read(ref_len_.data(), v2_.db_seqs_used);
		f_.seek(sizeof(VATHeaderOne) + sizeof(VATHeaderTwo));
		//cout << "4..........." << endl;

	}

	uint64_t VATBuild() const
	{ 
        return v2_.vat_build; 
    }

	uint64_t DBSeqs() const
	{ 
        return v2_.db_seqs; 
    }

	uint64_t DBSeqsUsed() const
	{ 
        return v2_.db_seqs_used; 
    }

	uint64_t DBletters() const
	{ 
        return v2_.db_letters; 
    }

	const char* Score_Matrix() const
	{ 
        return v2_.score_matrix; 
    }

	int32_t GapOpenPenalty() const
	{ 
        return v2_.gap_open; 
    }

	int32_t GapExtensionPenalty() const
	{ 
        return v2_.gap_extend; 
    }

	int32_t MatchReward() const
	{ 
        return v2_.reward; 
    }

	int32_t MismatchPenalty() const
	{ 
        return v2_.penalty; 
    }

	uint64_t QueryRecords() const
	{ 
        return v2_.query_records; 
    }

	AlignMode mode() const
	{ 
        return (AlignMode)v2_.mode; 
    }

	const string& RefName(size_t i) const
	{
		//cout << "RefName" <<ref_name_[i]<< endl;
		return ref_name_[i];
	}

	const uint32_t RefLen(size_t i) const
	{ 
        return ref_len_[i]; 
    }

	bool ReadQueryBuffer(BinaryBuffers &buf)
	{
		uint32_t size;
		f_.read(&size, 1);
		if(size == 0)
			return false;
		buf.resize(size);
		f_.read(buf.data(), size);
		return true;
	}

private:

	InputStreamController f_;
	VATHeaderOne v1_;
	VATHeaderTwo v2_;
	ptr_vector<string> ref_name_;
	vector<uint32_t> ref_len_;

};

#endif // __VATFILE_H__