#include "sfa.h"

using namespace std;

SFA::SFA()
{
    init();
}

SFA::SFA( char **s, int n )
{
    init();
    seqs = s;
    nreads = n;
}

SFA::~SFA()
{
    clear();
}

void SFA::init()
{
	concat  = NULL;
	SA      = NULL;
	LCP     = NULL;
	mLCP    = NULL;
	seqs    = NULL;
	nreads  = 0;
  verbose = false;
}

void SFA::clear()
{
    purgeSA();
    purgeDoc();
    purgeLCP();
    purgeMLCP();
}

void SFA::purgeSA()
{
    if ( SA != NULL ) delete[] SA;
    SA = NULL;
}

void SFA::purgeDoc()
{
    if ( concat != NULL ) delete[] concat;
    concat = NULL;
}

void SFA::purgeLCP()
{
    if ( LCP != NULL ) delete[] LCP;
    LCP = NULL;
}

void SFA::purgeMLCP()
{
    mLCP = NULL;
}


void  SFA::setStorageSize()
{
    size = 0;
    for ( int i = 0; i < nreads; i++ ) {
        int len = strlen(seqs[i]);
        size += len;
        size++;
    }
}


void SFA::concatenateAllSeqs()
{
    SFAIDXTYPE cpos = 0;
    for ( int i = 0; i < nreads; i++ ) {
        int len = strlen(seqs[i]);
        memcpy( &concat[cpos], (SFACHARTYPE *)seqs[i], len );
        cpos += len;
        concat[cpos] = DELIMITER;
        cpos++;
    }

    assert(size == cpos);
    concat[size] = '\0';
}

void SFA::buildSFA()
{
    setStorageSize( );

    concat = new SFACHARTYPE[size+1];
    if ( concat == NULL ) {
        std::cerr << "SFA::BuildSFA::Memory allocation error\n";
        exit(EXIT_FAILURE);
    }
    concatenateAllSeqs( );

    assert(strlen((char*) concat) > 0);

    SA = new SFAIDXTYPE[size];
    if ( concat == NULL ) {
        std::cerr << "Memory allocation error\n";
        exit(EXIT_FAILURE);
    }

    divsufsort( concat, SA, size );
}


void SFA::buildLCP()
{
    assert(SA != NULL);
    SFAIDXTYPE *rank = new SFAIDXTYPE[size];
    for (SFAIDXTYPE i = 0; i < size; i++ )
        rank[SA[i]] = i;

    LCP = new SeqIdxType[size];
    LCP[0] = 0;
    SFAIDXTYPE h = 0;
    for (SFAIDXTYPE i = 0 ; i < size; i++ ) {
        if ( rank[i] > 0 ) {
            SFAIDXTYPE j = SA[rank[i]-1];
            while( concat[i+h] != DELIMITER && concat[i+h] == concat[j+h] ) {
                h++;
                assert(i+h<=size && j+h<=size);
            }
            LCP[rank[i]] = h;
            if ( h > 0 ) h--;
        }
    }
    delete[] rank;
}


void SFA::buildMLCP()
{
    ia.build( LCP, size );

    SFAIDXTYPE n = ia.getSize();
    assert( n == size-1 );

    mLCP = ia.getIntervals();
}
