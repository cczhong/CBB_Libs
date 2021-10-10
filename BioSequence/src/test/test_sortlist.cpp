#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/options.h"
#include "../data/sorted_list.h"
#include "../data/seed_histogram.h"
#include "../util/tinythread.h"
#include "../util/thread.h"
#include <auto_ptr.h>
using std::cout;
using std::cerr;
using std::endl;
//g++ test_sortlist.cpp ../basic/options.o ../util/tinythread.cpp -o sortlist -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system

int main()
{
    char str = 'A';
    vector<Nucleotide> v;
    for (size_t i = 0; i < 20; i++)
    {
        /* code */
        v.push_back(str);
    }
    
    
    SequenceSet<Nucleotide> *ss = new SequenceSet<Nucleotide>();
    ss->push_back(v);
    //sorted_list<char>::entry e(1,'2'); 	char *query_buffer = sorted_list<_locq>::Type::alloc_buffer(*query_hst);
    seed_histogram *hst = new seed_histogram(*ss,Nucleotide());
    char *buf = sorted_list<Nucleotide>::Type::alloc_buffer(*hst);
    // int tem = sizeof(buf);
    // cout << "size  = " <<tem<< endl;
    // cout << "char = " <<*buf<<endl;
    //char* bf = "123";
    //sorted_list<char> *sl = new sorted_list<char>();
    // sorted_list<char>
    //e.operator<
    delete ss;
  //  delete buf;
    //delete hst;
    return 0;
}