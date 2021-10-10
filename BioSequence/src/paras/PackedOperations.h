#ifndef __PACKED_OPERATIONS_H__
#define __PACKED_OPERATIONS_H__



#include <stdint.h>
#include <vector>


typedef enum
{
    op_match = 0,
    op_insertion = 1,
    op_deletion = 2,
    op_substitution = 3
} EditOperations;

struct PackedOperations
{
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

	unsigned Count() const
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

template<typename _T>
struct CombinedOperations
{
    /* data */
    EditOperations op;
    unsigned count;
    _T letter;
};


#endif // __PACKED_OPERATIONS_H__