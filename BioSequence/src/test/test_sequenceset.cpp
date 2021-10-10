#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/options.h"
#include "../data/sorted_list.h"
#include "../data/seed_histogram.h"
#include "../util/tinythread.h"
#include "../util/thread.h"
#include <auto_ptr.h>
#include "../data/frequency_masking.h"
#include "../data/reference.h"
#include "../util/binary_file.h"
/**
 * g++ test_sequenceset.cpp ../basic/options.o ../util/tinythread.cpp -o sequenceset -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
 */
//boost::iostreams::filtering_ostream log_stream;

int main()
{
    Input_stream in("./test.fa");

    SequenceSet<char> *ss = new SequenceSet<char>(in);

    //ss->print_stats();

    return 0;
}