//#include "../tool/util.h"
#include <iostream>

using std::cout;
using std::endl;


//ldconfig -p | grep boost
// g++ test_util.cpp ../basic/options.o -o util -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{
    // int str = 129;
    // int str1 = 128;
    // int str2 = 127;
    // int l = mask_critical<int>(str);
    // int l1 = mask_critical<int>(str1);
    // int l2 = mask_critical<int>(str2);

    // int str3 = 12912;
    // int str4 = 12812;
    // int str5 = 12712;
    // int str6 = 129455;
    // int l3 = mask_critical<int>(str3);
    // int l4 = mask_critical<int>(str4);
    // int l5 = mask_critical<int>(str5);
    // int l6 = mask_critical<int>(str6);
    // cout << "l 129= " << l<<std::endl;
    // cout << "l 128 = " << l1<<std::endl;
    // cout << "l 12712 = " << l2<<std::endl;

    // cout << "l 12912= " << l3<<std::endl;
    // cout << "l 12812 = " << l4<<std::endl;
    // cout << "l 12712 = " << l5<<std::endl;
    // cout << "l 12712 = " << l6<<std::endl;

    // char *str_c = "ABC.CD.FD";
    // char *de = ".";
    // vector<string> v;
    // v = tokenize(str_c, de);
    // for (size_t i = 0; i < v.size(); i++)
    // {
    //     cout << "" <<v[i]<< std::endl;
    // }

    char v = 'A';
    cout << "A = " << (static_cast<unsigned>(v) & 0x7F) << endl;
    cout << "A = " << (static_cast<unsigned>(v) | 0x80) << endl;
    cout << "A = " << (static_cast<unsigned>(v) | 0x80) << endl;

    cout << "A = " <<int('A') << 5<<endl;

    return 0;
}