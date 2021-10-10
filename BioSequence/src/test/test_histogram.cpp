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
//g++ test_histogram.cpp ../basic/options.o ../util/tinythread.cpp -o sortlist -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system

int main()
{
    char str = 'A';
    vector<AminoAcid> v1;
    vector<AminoAcid> v2;
    vector<AminoAcid> v3;
    for (size_t i = 0; i < 20; i++)
    {
        /* code */
        v1.push_back(str);
        v2.push_back(str);
        v3.push_back(str);

    }
    
    SequenceSet<AminoAcid> *ss = new SequenceSet<AminoAcid>();
    ss->push_back(v1);
    ss->push_back(v2);
    ss->push_back(v3);

    ss->print_stats();
    size_t chunk = 0;
    for (;; chunk++)
    {
      /* code */
      seed_histogram *hst = new seed_histogram(*ss,AminoAcid());
      delete hst;
    }
    
    
    //Output_stream main("./test");
  //  hst->save(main);
  //  delete hst;
    return 0;
}