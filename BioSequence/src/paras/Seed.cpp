#include "ConstParameter.h"
#include "Seed.h"


//divide seed
unsigned seedPartition(seed s)
{
    return s & (ConstParameter::seedp -1);
};

unsigned seedPartitionOffset(seed s)
{
    return s >> ConstParameter::seedp_bits;
};
