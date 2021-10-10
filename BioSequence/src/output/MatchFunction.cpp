
#include "MatchFunction.h"

int blast_frame(unsigned frame)
{ 
    return frame <= 2 ? (int)frame+1 : 2-(int)frame; 
}



