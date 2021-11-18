#include "BioSequences.h"

//g++ *.cpp -o t -lboost_filesystem -lboost_system

int main()
{
    BioSequences bs;
    string db_fa = "/home/xuan/biosequence/BioSequence/t2.fa";
    int i = bs.guessFormat(db_fa);
    int j = bs.getNumSeqs(db_fa);
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;
    SFABuild sfab;
    BioAlphabet ba = bs.getBioAlphabetInstance("DNA");
    bs.createSFA(ba, db_fa,sfab);


    char **seqs =  new char* [j];
    int *seq_length = new int [j];
    int k = bs.loadingFasta(ba,db_fa,seqs,seq_length);
    cout <<"k = "<<k<< endl;
    delete seqs, seq_length;
    return 1;
}