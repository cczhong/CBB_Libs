#ifndef __PACKED_TRANSCRIPT_H__
#define __PACKED_TRANSCRIPT_H__

#include <stdint.h>
#include <vector>
//#include "packed_operations.h"
#include "../tool/BinaryBuffer.h"
#include "../tool/BinaryFile.h"
using std::vector;

typedef enum
{
    op_match = 0,
    op_insertion = 1,
    op_deletion = 2,
    op_substitution = 3
} EditOperations;

class PackedOperations
{
    public:
    PackedOperations(uint8_t code_)
    {
        code = code_;
    }

    PackedOperations(EditOperations op, unsigned count)
    {   //
        code = (op << 6) | count;
    }

    template<typename _t>
    PackedOperations(EditOperations op, _t v)
    {
        code = ((op << 6) | (int)v);
    }

    operator uint8_t()const
    {
        return code;
    }

	EditOperations Op() const
	{ 
        return (EditOperations)(code>>6);
    }

	unsigned count() const
	{ 
        return code&63; 
    }

	template<typename _T>
	_T letter() const
	{ 
        return code&63;
    }

	static PackedOperations terminator()
	{ 
        return PackedOperations(op_match, 0);
    }


    /* data */
    uint8_t code;
};

template<typename _t>
struct CombinedOperations
{
    public:
    /* data */
    EditOperations op;
    unsigned count;
    _t letter;
};

class PackedTranscript
{
    public:
    template<typename _t>
    class Const_Iterator
    {
        public:
        Const_Iterator(const PackedOperations *op)
        {
            
        }

        bool Good() const
		{ 
            return *ptr_ != PackedOperations::terminator(); 
        }

		Const_Iterator& operator++()
		{ 
            ++ptr_; 
            Gather(); 
            return *this;
        }

		const CombinedOperations<_t>& operator*() const
		{ 
            return op_; 
        }

		const CombinedOperations<_t>* operator->() const
		{ 
            return &op_;
        }
        private:
        void Gather()
        {
            if(!Good())
                return;
            op_.op = ptr_->Op();
            if (op_.op == op_deletion|| op_.op == op_substitution)
            {
                op_.letter = ptr_->letter<_t>();
                op_.count = 1;
            }
                else
            {
                op_.count = 0;
                do
                {
                    /* code */
                    op_.count += ptr_->count();
                    ++ptr_;
                } while (Good() && ptr_->Op() == op_.op);
                --ptr_;
            }
        }
       
        const PackedOperations *ptr_;
        CombinedOperations<_t> op_;
    };


     void Read(BufferedFile &f)
        {
            data_.clear();
            uint8_t code;
            do
            {
                /* code */
                f.read(code);
                data_.push_back(code);
            } while (code != PackedOperations::terminator());
        }

        void Read(BinaryBuffers::Iterator &it)
        {
            data_.clear();
            uint8_t code;
            do
            {
                it >> code;
                //code << it;
                data_.push_back(code);
            } while (code != PackedOperations::terminator());
        }
	template<typename T>
	Const_Iterator<T> begin() const
	{ 
        return Const_Iterator<T> (data_.data()); 
    }

	const vector<PackedOperations>& data() const
	{ 
        return data_; 
    }
    //private:
        vector<PackedOperations> data_;
};

#endif // __PACKED_TRANSCRIPT_H__