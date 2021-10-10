#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/const.h"
#include "../basic/seed.h"
#include <stdint.h>

using namespace std;
//ldconfig -p | grep boost
//g++ test_seed.cpp -o seed
int main()
{

    //uint64_t seed1 = 1024;


    for (size_t i = 0; i <= 65535*1024; i++)
    {
        /* code */
        uint64_t t1 = Seed_Partition(i);
        cout <<"i = "<<i<< ",seed partition = " <<t1<< endl;
        uint64_t t2 = Seed_Partition_Offset(i);
        cout <<"i = "<<i<< ",seed paritition offset = " <<t2<< endl;
        cout << "low i >> 32 = " <<(i >> 32)<< endl;
        cout << "high v & 0xffffffffu = " <<(i & 0xffffffffu)<<endl;
        //low(v & 0xffffffffu)
    }

    return 0;
}