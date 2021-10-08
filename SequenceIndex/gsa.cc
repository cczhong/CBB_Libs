using namespace std;

#include "gsa.h"


GSA::GSA()
{
    init(NULL, 0);
}


GSA::GSA( char **s, IDType n, bool gen_lcp )
{
    init(s, n);
    GSA::buildGSA();
    GSA::buildLCPs();
}


GSA::~GSA()
{
    clear();
}

void GSA::init(char **s, IDType n)
{
    Ids    = NULL;
    Pos    = NULL;
    seqs   = s;
    nreads = n;
}

void GSA::clear()
{
    if ( Ids != NULL ) delete[] Ids;
    if ( Pos != NULL ) delete[] Pos;
    Ids = NULL;
    Pos = NULL;
}

void GSA::buildGSA()
{
    double t0 = mytime();
    SFA::buildSFA();
    if ( verbose ) std::cout << "Suffix Array created:" << mytime()-t0 << " sec\n";

    t0 = mytime();
    __convertWithArrays();
    if ( verbose ) std::cout << "Generalized Suffix Array created:" << mytime()-t0 << " sec\n";
}

void GSA::buildLCPs()
{
    double t0 = mytime();
    SFA::buildLCP();
    if ( verbose ) std::cout << "LCP created:" << mytime()-t0 << " sec\n";

    t0 = mytime();
    SFA::buildMLCP();
    if ( verbose ) std::cout << "lLCP/rLCP created:" << mytime()-t0 << " sec\n";
}


void GSA::__convertWithArrays()
{
    std::vector<IDType> docs(size,0);
    std::vector<IDType> poss(size,0);

    IDType doc = 0;
    IDType pos = 0;
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        docs[i] = doc;
        poss[i] = pos;
        pos++;
        if ( concat[i] == '$' ) {
            doc++; pos = 0;
        }
    }

    Ids = new IDType[size];
    Pos = new IDType[size];
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        IDType doc = docs[SA[i]];
        IDType pos = poss[SA[i]];
        Ids[i] = doc;
        Pos[i] = pos;
    }
}

void GSA::DumpAllBlock(const char *idx_file, const IDType &pivot) {
    std::ofstream out_fh(idx_file, ios_base::out | ios_base::binary);
    if(!out_fh.good())  {
        cout << "GSA::DumpAll: Cannot write suffix array index: " << idx_file << endl;
        exit(1);
    }
    out_fh.write((char*) &size, sizeof(SFAIDXTYPE));
    for(SFAIDXTYPE i = 0; i < size; ++ i)   {
        Ids[i] += pivot;
        out_fh.write((char*) &Ids[i], sizeof(IDType));
        out_fh.write((char*) &Pos[i], sizeof(IDType));
        out_fh.write((char*) &LCP[i], sizeof(IDType));
    }
    return;
}
