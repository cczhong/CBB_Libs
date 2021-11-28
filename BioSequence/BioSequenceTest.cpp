#include "BioSequences.h"
//how to use the module
//g++ BioSequenceTest.cpp -L. -lbiosequences_core -lboost_filesystem -lboost_system

int main()
{
    BioSequences bs;
    string db_fa = "/home/xuan/biosequence/BioSequence/t2.fa";
    int i = bs.guessFormat(db_fa);
    int j = bs.getNumSeqs(db_fa);
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;

    BioAlphabet ba_DNA = bs.getBioAlphabetInstance("DNA");
    BioAlphabet ba_Dna = bs.getBioAlphabetInstance("Dna");
    BioAlphabet ba_protein = bs.getBioAlphabetInstance("protein");
    BioAlphabet ba_Protein = bs.getBioAlphabetInstance("Protein");

    char **seqs =  new char* [j];
    int *seq_length = new int [j];
    int k = bs.loadingFasta(ba_Dna,db_fa,seqs,seq_length);
    int p = bs.loadingFasta(ba_protein,db_fa,seqs,seq_length);
    cout <<"k = "<<k<<"\t"<<" p = "<<p<<endl;
    delete seqs, seq_length;
    return 1;
}