#ifndef __HASH_FUNCTION_H__
#define __HASH_FUNCTION_H__
#include <stdint.h>

/**
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain. The author hereby disclaims copyright to this source code.
 
  Note - The x86 and x64 versions do _not_ produce the same results, as the
  algorithms are optimized for their respective platforms. You can still
  compile and run any of them on any platform, but your performance with the
  non-native version will be less than optimal.
  
  github : https://github.com/aappleby/smhasher
 * 
*/
struct murmur_hash
{
    /*Init hash*/
    uint64_t operator()(uint64_t i) const
    {
        i ^= i >> 33;
		i *= 0xff51afd7ed558ccdLL;
		i ^= i >> 33;
		i *= 0xc4ceb9fe1a85ec53LL;
		i ^= i >> 33;
        return i;
    }
};


#endif // __HASH_FUNCTION_H__