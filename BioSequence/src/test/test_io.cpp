#ifndef __TEST_IO_H__
#define __TEST_IO_H__


#include <iostream>
#include "../basic/options.h"
#include "../util/binary_file.h"
/**
 * gcc test_io.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -lstdc++
 */
//boost::iostreams::filtering_ostream log_stream;

int main()
{

    //Input_stream db_file("test.fa",true);
    Input_stream db_file_1("test.fa",false);


    //Output_stream os("./test.fa");
    //os.write("test",4);

    return 0;
}

#endif // __TEST_IO_H__