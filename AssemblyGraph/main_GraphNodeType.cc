#include "GraphNodeType.h"

#include <iostream>
#include <string>

using namespace std;

int main()  {

    GraphNodeType a;
    char *s = new char [100];
    strcpy(s, "hello world");
    a.SetSequence(s);

    GraphNodeType b;
    b = a;

    //char *sshort = new char [5];  // tested, assertion failed
    //a.CopySeq(sshort);
    
    char *slong = new char [100];
    strcpy(slong, b.GetStrPtr());

    cout << "A sequence length:   " << a.GetSeqLen() << endl;
    cout << "B sequence length:   " << b.GetSeqLen() << endl;

    cout << "sequence:  " << string(slong) << endl;
    
    delete [] s;
    delete [] slong;
    return 0;
}

