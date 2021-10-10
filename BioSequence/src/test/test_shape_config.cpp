#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/const.h"
#include "../basic/value.h"
#include "../basic/options.h"
#include "../basic/shape.h"
#include "../basic/shape_config.h"

//ldconfig -p | grep boost
//g++ test_shape_config.cpp ../basic/options.o -o shape -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{
    shape_config *sc = new shape_config(2,AminoAcid());
    shape_config *sc1 = new shape_config(1,Nucleotide());
    //count() = num of shapes
    shape s1 = sc->get_shape(1);
    uint64_t key = 112312;
    //const SequenceSet<AminoAcid> *seqs;
    sequence<const AminoAcid> seq;

    s1.set_seed(key,&seq);
    cout << "s1 length = " << s1.length_ << ", count = " << sc->count() << endl;

    //shape s2 = sc1->get_shape(1);
    //cout << "s2 length = " << s2.length_<<", count = "<<sc1->count()<<endl;


    delete sc;
    delete sc1;
    return 0;
}