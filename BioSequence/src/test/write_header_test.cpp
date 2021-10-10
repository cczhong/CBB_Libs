#include <vector>
#include <iostream>
#include "../data/sequence_set.h"



//gcc test_io.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -lstdc++

//g++ string_set_test.cpp ../basic/options.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{

	Output_stream main("./nr");
    char *c = "C";
    //write 1 into ref_header
    main.write(c, 1);

    return 0;
}

