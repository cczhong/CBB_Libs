#include "BioSequences.h"

//g++ *.cpp -o t -lboost_filesystem -lboost_system


int main()
{
    BioSequences bs;
    string db_fa = "/home/xuan/biosequence/t2.fa";
    int i = bs.guessFormat(db_fa);
    int j = bs.getNumSeqs(db_fa);
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;
    SFABuild sfab;
    bs.createSFA("DNA", db_fa,sfab);
    cout << sfab.GetNumSeqs() << endl;

    int g = 5;
    int *s = &g;
    int t = 1;
    const char *path = "s.txt";
    cout<<bs.writeData<int, int>(s, t, path)<<endl;
    return 1;
}