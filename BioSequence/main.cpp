#include "BioSequences.h"

//g++ *.cpp -o t -lboost_filesystem -lboost_system


int main()
{
    BioSequences bs;
    string db_fa = "/home/xuan/biosequence/t2.fa";
    bs.LoadSequence(db_fa);
    int i = bs.guessFormat(db_fa);
    int j = bs.getNumSeqs(db_fa);
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;
    bs.createSFA(bs.getBioSequenceInstance("DNA"), db_fa);
    return 1;
}