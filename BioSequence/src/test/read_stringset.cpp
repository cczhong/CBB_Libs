

#include <vector>
#include <iostream>
#include "../data/sequence_set.h"

// //#include ""

//gcc test_io.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -lstdc++

//g++ read_stringset.cpp ../basic/options.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -o read_set

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{


    Input_stream is("./df",false);
    StringSet<char,0> *s2 = new StringSet<char,0>(is);
    cout << "s2 = " << s2->letters()<< endl;
    cout << "s2 = " << s2->ptr(0)<< endl;
    cout << "s2 = " << s2->ptr(1)<< endl;
    cout << "s2 = " << s2->ptr(2)<< endl;
    cout << "s2 getlength ()= " << s2->get_length()<< endl;


    return 0;
}

