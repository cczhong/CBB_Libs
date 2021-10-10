#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/const.h"
#include "../basic/value.h"
#include "../basic/options.h"
#include "../basic/shape.h"
#include "../basic/shape_config.h"
#include <stdint.h>
#include "../data/string_set.h"
//ldconfig -p | grep boost
//g++ test_shape.cpp ../basic/options.o -o shape -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{

    char* str = "111101111";
    shape *s = new shape(str,6);

    //uint64_t key = 112312;
    //const SequenceSet<AminoAcid> *seqs;
    //const sequence<AminoAcid>* seq;
    //s->
    //s->set_seed(key, seq);


    delete s;
    //delete s;
    return 0;
}