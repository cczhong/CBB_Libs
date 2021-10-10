#include <iostream>
//#include "../commons/options.h"
//#include "../database/sequenceset.h"
#include "../dp/ScalarDPMatrix.h"

using std::cout;
//g++ test_xdrop.cpp ../commons/options.cpp -o xdrop -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{
    ScalarDPMatrix<int,TraceBack> mtx (2);
    int j = 0, i_max = 0;
    typename ScalarDPMatrix<int, TraceBack>::Column_iterator it = mtx.column(j, i_max);
}