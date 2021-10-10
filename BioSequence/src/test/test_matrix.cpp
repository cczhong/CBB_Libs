#include <iostream>
#include "../commons/NuclMatrix.h"
#include "../commons/value_type.h"

using std::cout;
using std::endl;
int main()
{
    //NucleotideMatrix nm;
    Nucleotide n1('A');
    Nucleotide n2('G');
    Nucleotide n3('C');
    Nucleotide n4('T');
    Nucleotide n5('A');

    if (n1 == n2)
    {
        cout << "true" << endl;
    }else
    {
        cout << "false" << endl;
    }

    return 1;
}