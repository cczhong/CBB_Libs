#include <iostream>
#include "SeqRepresentation.h"

int main()
{
    //const SeqRepresentation::invalid = 0xff;
    //const char SeqRepresentation<char>::invalid = 0xff;
    const SeqRepresentation<char> seq(5,"ACGTN",4,"MRWSYKVHDBX");
    char t = seq.operator()('A');
    cout << "seq = " << t<< endl;
    return 1;
}