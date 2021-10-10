#include <iostream>
//#include "../commons/options.h"
//#include "../database/sequenceset.h"
#include "../commons/score_matrix.h"
#include "../tool/util.h"

//g++ test_xdrop.cpp ../commons/options.cpp -o xdrop -I/usr/include/boost/  -L/usr/lib/x86_64-linux-gnu/ -I../algo/blast/core/ -L../algo/blast/core/ -lblast_core -lz -lpthread -lboost_iostreams -lboost_system
int main()
{
    int score = 0, st = 0;
    int n = 0;
    int delta = 0;
    int seed_len = 4;
    int len = 0;

    const char *query = "ATGC_";
    const char *subject = "ATGC_";

    
    const char *str = "_";

    const char *q = query + 1;
    const char *s = subject + 1;

    cout << "q = " <<*q<<", query = "<<query<<", query 2 = "<<query[2]<<endl;
    cout << "s = " <<*s<<", subject = "<<subject<<", subject 2 = "<<subject[2]<<endl;

    //to left
    // while (*q != *str && *s != *str)
    // {
    //     st = st + score_matrix::get().letter_score<char>(*q, mask_critical<char>(*s));
    //     score = std::max(score,st);
    //     --q;
    //     --s;
    //     ++delta;
    // }

    // q = query + seed_len;
    // s = subject + seed_len;
    // st = score;
    // to right
    while (*q != *str && *s != *str)
    {
        //st = st + score_matrix::get().letter_score<char>(*q, mask_critical<char>(*s));
        
        score = std::max(score,st);
        ++q;
        ++s;
        ++n;
    }

    // for(int i=0;i<seed_len;++i)
    // {
    //     score += score_matrix::get().letter_score<char>(query[i], mask_critical<char>(subject[i]));
    // }
	// len = delta + n + seed_len;

    cout << "score = " <<score<< endl;
    // delete query, subject, str, q, s;
    //return score;
    return 0;
}