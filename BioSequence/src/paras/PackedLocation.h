#ifndef __PACKED_LOCATION_H__
#define __PACKED_LOCATION_H__
#include <stdint.h>

class PackedUint40_t
{
	public:
	uint8_t		high;
	uint32_t	low;

	PackedUint40_t():
		high (),
		low ()
	{ 

	}

	PackedUint40_t(uint64_t v):
		high (v>>32),
		low (v&0xffffffffu)
	{ 

	}

	operator const uint64_t() const
	{ 
		return (uint64_t(high) << 32) | low;
	}

	bool operator<(const PackedUint40_t &rhs) const
	{ 
		return high < rhs.high || (high == rhs.high && low < rhs.low);
	}

	friend uint64_t operator-(const PackedUint40_t &x, const PackedUint40_t &y)
	{ 
		return (const uint64_t)(x) - (const uint64_t)(y);
	}

} __attribute__((packed));

template<class _loc>
class PackedSequenceLocation
{
	public:
	typedef _loc type;
};

template<>
class PackedSequenceLocation<uint64_t>
{
	public:
	typedef PackedUint40_t type;
};

#endif // __PACKED_LOCATION_H__