

#include <vector>
#include <iostream>
#include "../data/sequence_set.h"

// //#include ""

//gcc test_io.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -lstdc++

//g++ string_set_test.cpp ../basic/options.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -o string_set

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{

    string str = "1";
    string str1 = "2";
    string str2 = "3";
    string str3 = "4";
    string str4 = "5";
    string str5 = "6";

    string str6 = "A";
    vector<string> v1;
    vector<char> v2;
    vector<char> v3;
    vector<char> v4;
    char c1 = 'a';
    char c2 = 'b';
    char c3 = 'c';
    char c4 = 'd';

     v3.push_back(c1);
    // v3.push_back(c2);
    // v3.push_back(c3);
    // v3.push_back(c4);
    // v2.push_back(c4);
    // v2.push_back(c3);
    v1.push_back(str);
    v1.push_back(str1); 
    v1.push_back(str2);
    v1.push_back(str3); 
    v1.push_back(str4);
    //v1.insert(v1.begin()+1,3,str5);
    //v2.insert(v2.end(),v1.begin(),v1.end());
    //v2.insert(v2.end(),0xff,1UL);
    vector<string>::iterator it;
    //SequenceSet<char> ss;
    //cout << "v3 size  = " <<v3.size()<< "    v3 no.2 = "<<v3[2]<< endl;
    Output_stream main("./df");
    // StringSet<char> s;
    // s.push_back(v3);
    // s.push_back(v2);
    // //s.finish_reserve();
    // //s.data();
    // //s.length();
    // //cout << "PADDING_CHAR = " << (char32_t)s.PADDING_CHAR << endl;
    // cout << "string set get length = " << s.get_length() << endl;
    // cout << "string set ptr = " <<*s.ptr(0)<< endl;
    // cout << "raw len = " <<s.raw_len()<< endl;
    // cout << "oxff = " <<0xff<< endl;
    // cout << "v3 no.0 = " <<v3[1]<<endl;
    //StringSet<char,0>* s1 = new StringSet<char,0>();
    StringSet<char> *s2 = new StringSet<char>();
    //s2->push_back(v3);
    s2->push_back(v2);
    //cout << "position = " << s2->position(1, 1) << endl;
    // s1->push_back(v3);
    //s1->push_back(v2);
    //s1->finish_reserve();
    //s1->save(main);
    delete s2;
    //delete s1;
    return 0;
}

