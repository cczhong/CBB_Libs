#ifndef __SEED_H__
#define __SEED_H__
#include <stdint.h>

typedef uint64_t seed;

unsigned seedPartition(seed s);
unsigned seedPartitionOffset(seed s);



#endif // __SEED_H__