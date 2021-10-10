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
#include "../basic/reduction.h"


//ldconfig -p | grep boost
//g++ test_reduction.cpp ../basic/options.o -o reduction -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{
    //template<> const Reduction<AminoAcid> Reduction<AminoAcid>::reduction ("KREDQN C G H M F Y ILV W P STA");
    Reduction<AminoAcid> *r = new Reduction<AminoAcid>("S LKJ  NNB");
    //cout << "r size = " <<r->size()<< endl;
    cout << "" << endl;
    cout << "" << endl;

    //AminoAcid aa;
    char val0 = 'TTT';
    char val1 = 'A';
    char val2 = 'T';
    char val3 = 'G';
    char val4 = 'O';
    char val5 = 'P';
    char val6 = 'c';
    //Value_type<AminoAcid> *a = new Value_type<AminoAcid>(val);
    //typedef Value_type<Letter_prot> AminoAcid;

    AminoAcid a0(val0);
    AminoAcid a1(val1);
    AminoAcid a2(val2);
    AminoAcid a3(val3);
    AminoAcid a4(val4);
    AminoAcid a5(val5);
    AminoAcid a6(val6);
//    AminoAcid a1(val);


    //unsigned r = Reduction<_val>::reduction(l);
    //const char* str = "J";

    //Reduction<AminoAcid>::reduction;
    //Reduction<Nucleotide>::reduction;
    unsigned r0 = Reduction<AminoAcid>::reduction(a0);
    unsigned r1 = Reduction<AminoAcid>::reduction(a1);
    unsigned r2 = Reduction<AminoAcid>::reduction(a2);
    unsigned r3 = Reduction<AminoAcid>::reduction(a3);
    unsigned r4 = Reduction<AminoAcid>::reduction(a4);
    unsigned r5 = Reduction<AminoAcid>::reduction(a5);
    unsigned r6 = Reduction<AminoAcid>::reduction(a6);
    // cout << "Nucleotide size = " << Reduction<Nucleotide>::reduction.size() << endl;


    // cout << "Amino acid size = " <<Reduction<AminoAcid>::reduction.size()<<endl;
    cout << "r 0= " <<r0<< endl;
    cout << "r 1= " <<r1<< endl;
    cout << "r 2= " <<r2<< endl;
    cout << "r 3= " <<r3<< endl;
    cout << "r 4= " <<r4<< endl;
    cout << "r 5= " <<r5<< endl;
    cout << "r 6= " <<r6<< endl;

    cout << "char = " <<a6.operator char()<< endl;
    //delete r;
    return 0;
}