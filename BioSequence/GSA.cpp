using namespace std;

#include "GSA.h"

GSA::GSA()
{
    init(NULL, 0);
}

GSA::GSA( char **s, RIDTYPE n )
{
    init(s, n);
    GSA::buildGSA();
}

GSA::GSA( char **s, RIDTYPE n, bool gen_lcp )
{
    init(s, n);
    GSA::buildGSA();
    GSA::buildLCPs();
}

GSA::GSA( char **s, 
          RIDTYPE n, 
          bool gen_lcp, 
          const char *gsa_file,
          const char *lcp_file,
          const char *mcp_file)
{
    init(s,n);
    GSA::buildGSA( gsa_file );
    GSA::buildLCPs( lcp_file, mcp_file );
}

GSA::~GSA()
{
    clear();
}

void GSA::init(char **s, RIDTYPE n)
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
    //double t0 = mytime();
    SFA::buildSFA();
    //if ( verbose ) std::cout << "SFA created:" << mytime()-t0 << " sec\n";

    //t0 = mytime();
    //__convertWithBinarySearch();
    __convertWithArrays();
   // if ( verbose ) std::cout << "GSA created:" << mytime()-t0 << " sec\n";
}

void GSA::buildGSA(const char *gsa_file)
{
    //double t0 = mytime();
    SFA::buildSFA();
   // if ( verbose ) std::cout << "SFA created:" << mytime()-t0 << " sec\n";

    //t0 = mytime();
    //__convertWithBinarySearch();
    __convertWithArrays();
    //if ( verbose ) std::cout << "GSA created:" << mytime()-t0 << " sec\n";

    //t0 = mytime();
    GSA::writeGSA( gsa_file );
    GSA::clear(); 
   // if ( verbose ) std::cout << "GSA written & purged:" << mytime()-t0 << " sec\n";
}

void GSA::buildLCPs()
{
    // double t0 = mytime();
    SFA::buildLCP();
    // if ( verbose ) std::cout << "LCP created:" << mytime()-t0 << " sec\n";

    // t0 = mytime();
    SFA::buildMLCP();
    // if ( verbose ) std::cout << "lLCP/rLCP created:" << mytime()-t0 << " sec\n";
}

void GSA::buildLCPs( const char *lcp_file, const char *mcp_file )
{
    // double t0 = mytime();
    SFA::buildLCP();
    // if ( verbose ) std::cout << "LCP created:" << mytime()-t0 << " sec\n";

    // t0 = mytime();
    SFA::writeLCP( lcp_file );
    // if ( verbose ) std::cout << "LCP written & purged:" << mytime()-t0 << " sec\n";
    // t0 = mytime();
    SFA::purgeDoc();
    SFA::purgeSA();
 //   if ( verbose ) std::cout << "Concatenated string & Suffix array written/purged:" << mytime()-t0 << " sec\n";

  //  t0 = mytime();
    SFA::buildMLCP();
    // if ( verbose ) std::cout << "lLCP/rLCP created:" << mytime()-t0 << " sec\n";
    // t0 = mytime();
    SFA::writeMLCP( mcp_file );
    SFA::purgeMLCP();
    // if ( verbose ) std::cout << "lLCP/rLCP written & purged:" << mytime()-t0 << " sec\n";
}

void GSA::__convertWithBinarySearch()
{    
    std::vector<SFAIDXTYPE> poss(nreads,0);
    SFAIDXTYPE doc = 0;
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        if ( concat[i] == '$' ) {
            poss[doc] = i; doc++; 
        }
    }
 
    Ids = new RIDTYPE[size];
    Pos = new POSTYPE[size];
    RIDTYPE pos = 0;
    std::vector<SFAIDXTYPE>::iterator up;
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        pos = SA[i];
        up = std::lower_bound( poss.begin(), poss.end(), pos );
        doc = up-poss.begin();
        if ( doc > 0 ) 
            pos -= (poss[doc-1]+1);
        Ids[i] = doc;
        Pos[i] = pos;
    }
}

void GSA::__convertWithArrays()
{
    std::vector<RIDTYPE> docs(size,0);
    std::vector<POSTYPE> poss(size,0);

    RIDTYPE doc = 0;
    POSTYPE pos = 0;
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        docs[i] = doc;
        poss[i] = pos;
        pos++;
        if ( concat[i] == '$' ) {
            doc++; pos = 0;
        }
    }

    Ids = new RIDTYPE[size];
    Pos = new POSTYPE[size];
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        RIDTYPE doc = docs[SA[i]];
        POSTYPE pos = poss[SA[i]];
        Ids[i] = doc;
        Pos[i] = pos;
    }
}

void GSA::printSFA() 
{
    for ( SFAIDXTYPE i = 0; i < size; i++ ) 
        std::cout << Ids[i] << ":" << (int)Pos[i] << " ";
    std::cout << "\n";
}

void GSA::printSuffix()
{
    // no reference sequence is given
    if ( seqs == NULL ) 
        return;

    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        RIDTYPE doc = Ids[i];
        POSTYPE pos = Pos[i];
        POSTYPE rlen = (POSTYPE) strlen(seqs[doc]);
        assert(pos <= rlen);

        std::cout << i << "\t" << doc << "\t" << pos << "\t" << seqs[doc]+pos << "\n";
    }
}

void GSA::readGSA( const char *filename )
{
    std::fstream in;
    fio::openFile( in, filename, std::ios::in | std::ios::binary );

    in.read((char*)&size, sizeof(SFAIDXTYPE));
    assert(size>0);

    Ids = new RIDTYPE[size];
    Pos = new POSTYPE[size];
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        in.read((char*)&(Ids[i]), sizeof(RIDTYPE));
        in.read((char*)&(Pos[i]), sizeof(POSTYPE));
    }
    in.close();
}

void GSA::load(const char *idx_file)    {
    std::ifstream in_fh(idx_file, std::ios::in | std::ios::binary);
    if(!in_fh.good())  {
        cout << "MANA::GSA::load: Cannot read suffix index file " << idx_file << endl;
        exit(1);
    }
    in_fh.read((char*) &size, sizeof(SFAIDXTYPE));
    assert(size > 0);
    Ids = new RIDTYPE[size];
    Pos = new POSTYPE[size];
    LCP = new POSTYPE[size];
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        in_fh.read((char*) &(Ids[i]), sizeof(RIDTYPE));
        in_fh.read((char*) &(Pos[i]), sizeof(POSTYPE));
        in_fh.read((char*) &(LCP[i]), sizeof(POSTYPE));
        // DEBUG
        //cout << "SFA:   " << Ids[i] << "    " << Pos[i] << "    " << LCP[i] << "  " << seqs[Ids[i]] + Pos[i] << endl;
    }
    return;
}

void GSA::load( const char *lcp_file,
                const char *mcp_file,
                const char *gsa_file )
{
    //double t0 = mytime();
    GSA::readGSA( gsa_file );
    // if ( verbose ) std::cout << "Suffix array loaded:" << mytime()-t0 << "\n";
    // t0 = mytime();
    SFA::readLCP( lcp_file );
    // if ( verbose ) std::cout << "LCP array loaded:" << mytime()-t0 << "\n";
    // t0 = mytime();
    SFA::readMLCP( mcp_file );
    // if ( verbose ) std::cout << "lLCP/rLCP array loaded:" << mytime()-t0 << "\n";
}

void GSA::load( const char *sfa_file, 
                const char *doc_file,
                const char *lcp_file,
                const char *mcp_file,
                const char *gsa_file )
{
    SFA::load( sfa_file, doc_file, lcp_file, mcp_file );
    GSA::readGSA( gsa_file );
}

void GSA::writeGSA( const char *gsa_file )
{
    std::fstream out;
    fio::openFile( out, gsa_file, std::ios::out | std::ios::binary );
    
    out.write((char*)&size, sizeof(SFAIDXTYPE));
    for ( SFAIDXTYPE i = 0; i < size; i++ ) {
        out.write((char*)&Ids[i], sizeof(RIDTYPE));
        out.write((char*)&Pos[i], sizeof(POSTYPE));
    }
    out.close();
}

void GSA::dump( const char *sfa_file, 
                const char *doc_file,
                const char *lcp_file,
                const char *mcp_file,
                const char *gsa_file )
{
    SFA::dump( sfa_file, doc_file, lcp_file, mcp_file );
    GSA::writeGSA( gsa_file );
}

void GSA::DumpAllBlock(const char *idx_file, const RIDTYPE &pivot) {
    std::ofstream out_fh(idx_file, ios_base::out | ios_base::binary);
    if(!out_fh.good())  {
        cout << "MANA::GSA::DumpAll: Cannot write suffix array index: " << idx_file << endl;
        exit(1);
    }
    out_fh.write((char*) &size, sizeof(SFAIDXTYPE));
    for(SFAIDXTYPE i = 0; i < size; ++ i)   {
        Ids[i] += pivot;
        out_fh.write((char*) &Ids[i], sizeof(RIDTYPE));
        out_fh.write((char*) &Pos[i], sizeof(POSTYPE));
        out_fh.write((char*) &LCP[i], sizeof(POSTYPE));
    }
    return;
}

char* GSA::getSuffix( SFAIDXTYPE p )
{
    assert( p <= size );
    if ( seqs == NULL ) return NULL;
    RIDTYPE rid = Ids[p];
    POSTYPE pos = Pos[p];
    return seqs[rid]+pos;
}

char* GSA::getEntireSuffix( SFAIDXTYPE p )
{
    assert( p <= size );
    if ( seqs == NULL ) return NULL;
    return SFA::getSuffix(p);
}


GSATYPE GSA::getAt( SFAIDXTYPE p )
{
    assert( p <= size );
    return GSATYPE( Ids[p], Pos[p] );
}

BOUNDTYPE GSA::search( const SFACHARTYPE *srch, POSTYPE len )
{
    assert(seqs!=NULL);
    return ( LCP != NULL && mLCP != NULL ) ?
        this->searchWithLCPs( srch, len ) :
        this->searchOnGSA( srch, len ) ;
}

BOUNDTYPE GSA::searchWithLCPs( const SFACHARTYPE *srch, POSTYPE len )
{
    assert(seqs!=NULL);
    SFAIDXTYPE left  = this->getLeftBoundWithLCPs( srch, len );
    SFAIDXTYPE right = this->getRightBoundWithLCPs( srch, len );

    return BOUNDTYPE(left-1, right-1);
}

BOUNDTYPE GSA::searchWithLCPs_bounded(
    const SFACHARTYPE *srch, POSTYPE len, 
    SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
) {
  assert(seqs!=NULL);
  SFAIDXTYPE left  = this->getLeftBoundWithLCPs_bounded(srch, len, left_bound, right_bound);
  SFAIDXTYPE right = this->getRightBoundWithLCPs_bounded(srch, len, left_bound, right_bound);
  return BOUNDTYPE(left-1, right-1);
}

BOUNDTYPE GSA::searchOnGSA( const SFACHARTYPE *srch, POSTYPE len )
{
    assert(seqs!=NULL);
    SFAIDXTYPE left  = this->getLeftBoundOnGSA( srch, len );
    SFAIDXTYPE right = this->getRightBoundOnGSA( srch, len );

    return BOUNDTYPE(left-1, right-1);
}

char* GSA::getSeq(SFAIDXTYPE p )
{
    assert(p < size);                                                  

    RIDTYPE doc = Ids[p];
    POSTYPE pos = Pos[p];
    POSTYPE rlen = (POSTYPE) strlen(seqs[doc]);
    assert(pos <= rlen);
    
    return seqs[doc]+pos;
}

SFAIDXTYPE GSA::getLeftBoundWithLCPs(const SFACHARTYPE* pat, POSTYPE len)
{
    if ( verbose ) std::cout << "Left bound:\n";
	  POSTYPE lLcp = lcp( (SFACHARTYPE*)getSeq(0), pat );
	  POSTYPE rLcp = lcp( (SFACHARTYPE*)getSeq(size-1), pat );
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp >= len || pat[lLcp] <= getSeq(0)[lLcp]) return 1;
    if (rLcp <  len && pat[rLcp] >= getSeq(size-1)[rLcp]) return size+1;

	SFAIDXTYPE left  = 1;
	SFAIDXTYPE right = size;
    POSTYPE mLcp, lMcp, rMcp;
	while (right-left > 1)
	{
        lMcp = rMcp = 0;
        SFAIDXTYPE mid = (left+right)/2;

        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d lMcp:%d rMcp:%d\n", left, right, mid, lLcp, rLcp, lMcp, rMcp);

        if ( lLcp >= rLcp ) {
            lMcp = getMcp(left, mid);
            if ( lMcp >= lLcp ) 
                mLcp = lLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+lLcp, pat+lLcp );
            else {
                mLcp = lMcp;
            }
        } else {
            rMcp = getMcp(mid,right);
            if ( rMcp >= rLcp ) 
                mLcp = rLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+rLcp, pat+rLcp );
            else {
                mLcp = rMcp;
            }
        }

		if (mLcp >= len || pat[mLcp] <= getSeq(mid-1)[mLcp]) { 
            right = mid; rLcp = mLcp; 
        }
		else { 
            left  = mid; lLcp = mLcp; 
        }
        if (verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);
	}

    if ( verbose ) std::cout << "left bound:" << right << "\n";
	return right;
}

SFAIDXTYPE GSA::getLeftBoundWithLCPs_bounded(
    const SFACHARTYPE* pat, POSTYPE len, 
	  SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
) {
    if ( verbose ) std::cout << "Left bound:\n";
	POSTYPE lLcp = lcp( (SFACHARTYPE*)getSeq(0), pat );
	POSTYPE rLcp = lcp( (SFACHARTYPE*)getSeq(size-1), pat );
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp >= len || pat[lLcp] <= getSeq(0)[lLcp]) return 1;
    if (rLcp <  len && pat[rLcp] >= getSeq(size-1)[rLcp]) return size+1;

	SFAIDXTYPE left  = left_bound + 1;
	SFAIDXTYPE right = right_bound + size;
    POSTYPE mLcp, lMcp, rMcp;
	while (right-left > 1)
	{
        lMcp = rMcp = 0;
        SFAIDXTYPE mid = (left+right)/2;

        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d lMcp:%d rMcp:%d\n", left, right, mid, lLcp, rLcp, lMcp, rMcp);

        if ( lLcp >= rLcp ) {
            lMcp = getMcp(left, mid);
            if ( lMcp >= lLcp ) 
                mLcp = lLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+lLcp, pat+lLcp );
            else {
                mLcp = lMcp;
            }
        } else {
            rMcp = getMcp(mid,right);
            if ( rMcp >= rLcp ) 
                mLcp = rLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+rLcp, pat+rLcp );
            else {
                mLcp = rMcp;
            }
        }

		if (mLcp >= len || pat[mLcp] <= getSeq(mid-1)[mLcp]) { 
            right = mid; rLcp = mLcp; 
        }
		else { 
            left  = mid; lLcp = mLcp; 
        }
        if (verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);
	}

    if ( verbose ) std::cout << "left bound:" << right << "\n";
	return right;
}

SFAIDXTYPE GSA::getRightBoundWithLCPs(const SFACHARTYPE* pat, LCPTYPE len) 
{
    if ( verbose ) std::cout << "Right bound:\n";
	POSTYPE lLcp = lcp((SFACHARTYPE*)getSeq(0), pat);
	POSTYPE rLcp = lcp((SFACHARTYPE*)getSeq(size-1), pat);
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp <  len && pat[lLcp] <= getSeq(0)[lLcp]) return 0;
	if (rLcp >= len || pat[rLcp] >= getSeq(size-1)[rLcp]) return size;

	size_t left = 1;
	size_t right = size;
    POSTYPE mLcp, lMcp, rMcp;
	while (right-left > 1)
	{
        lMcp = rMcp = 0;
        SFAIDXTYPE mid = (left+right)/2;

        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d lMcp:%d rMcp:%d\n", left, right, mid, lLcp, rLcp, lMcp, rMcp);
        if ( rLcp >= lLcp ) {
            rMcp = getMcp(mid,right);
            if ( rMcp >= rLcp ) 
                mLcp = rLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+rLcp, pat+rLcp );
            else {
                mLcp = rMcp;
            }
        }
        else {
            lMcp = getMcp(left, mid);
            if ( lMcp >= lLcp ) 
                mLcp = lLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+lLcp, pat+lLcp );
            else {
                mLcp = lMcp;
            }
        } 
		if (mLcp >= len || pat[mLcp] >= getSeq(mid-1)[mLcp]) { 
            left  = mid; lLcp = mLcp; 
        }
		else { 
            right = mid; rLcp = mLcp; 
        }
        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);

	}

    if ( verbose ) std::cout << "right bound:" << left << "\n";
	return left;
}

SFAIDXTYPE GSA::getRightBoundWithLCPs_bounded(
    const SFACHARTYPE* pat, POSTYPE len, 
	  SFAIDXTYPE left_bound, SFAIDXTYPE right_bound
) {
    if ( verbose ) std::cout << "Right bound:\n";
	POSTYPE lLcp = lcp((SFACHARTYPE*)getSeq(0), pat);
	POSTYPE rLcp = lcp((SFACHARTYPE*)getSeq(size-1), pat);
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp <  len && pat[lLcp] <= getSeq(0)[lLcp]) return 0;
	if (rLcp >= len || pat[rLcp] >= getSeq(size-1)[rLcp]) return size;

	SFAIDXTYPE left = left_bound + 1;
	SFAIDXTYPE right = right_bound + 1;
    POSTYPE mLcp, lMcp, rMcp;
	while (right-left > 1)
	{
        lMcp = rMcp = 0;
        SFAIDXTYPE mid = (left+right)/2;

        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d lMcp:%d rMcp:%d\n", left, right, mid, lLcp, rLcp, lMcp, rMcp);
        if ( rLcp >= lLcp ) {
            rMcp = getMcp(mid,right);
            if ( rMcp >= rLcp ) 
                mLcp = rLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+rLcp, pat+rLcp );
            else {
                mLcp = rMcp;
            }
        }
        else {
            lMcp = getMcp(left, mid);
            if ( lMcp >= lLcp ) 
                mLcp = lLcp + lcp( (SFACHARTYPE*)getSeq(mid-1)+lLcp, pat+lLcp );
            else {
                mLcp = lMcp;
            }
        } 
		if (mLcp >= len || pat[mLcp] >= getSeq(mid-1)[mLcp]) { 
            left  = mid; lLcp = mLcp; 
        }
		else { 
            right = mid; rLcp = mLcp; 
        }
        if ( verbose ) 
            printf("left:%lu right:%lu mid:%lu lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);

	}

    if ( verbose ) std::cout << "right bound:" << left << "\n";
	return left;
}

SFAIDXTYPE GSA::getLeftBoundOnGSA(const SFACHARTYPE* pat, POSTYPE len)
{
    if ( verbose ) std::cout << "Left bound:\n";
	POSTYPE lLcp = lcp( (SFACHARTYPE*)getSeq(0), pat );
	POSTYPE rLcp = lcp( (SFACHARTYPE*)getSeq(size-1), pat );
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp >= len || pat[lLcp] <= getSeq(0)[lLcp]) return 1;
    if (rLcp <  len && pat[rLcp] >= getSeq(size-1)[rLcp]) return size+1;

	SFAIDXTYPE left = 1;
	SFAIDXTYPE right = size;
	while (right-left > 1)
	{
		SFAIDXTYPE mid = left + floor((right-left)/2.0);
		POSTYPE mLcp = lLcp <= rLcp? lLcp : rLcp;
		mLcp += lcp( (SFACHARTYPE*)getSeq(mid-1)+mLcp, pat+mLcp);
        if ( verbose ) 
            printf("left:%d right:%d mid:%d lLcp:%d rLcp:%d\n", left, right, mid, lLcp, rLcp);
		if (mLcp >= len || pat[mLcp] <= getSeq(mid-1)[mLcp]) { 
            right = mid; rLcp = mLcp; 
        }
		else { 
            left  = mid; lLcp = mLcp; 
        }
        if (verbose ) 
            printf("left:%d right:%d mid:%d lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);
	}

    if ( verbose ) std::cout << "left bound:" << right << "\n";
	return right;
}

SFAIDXTYPE GSA::getRightBoundOnGSA(const SFACHARTYPE* pat, POSTYPE len) 
{
    if ( verbose ) std::cout << "Right bound:\n";
	POSTYPE lLcp = lcp((SFACHARTYPE*)getSeq(0), pat);
	POSTYPE rLcp = lcp((SFACHARTYPE*)getSeq(size-1), pat);
    if ( verbose ) printf("lLcp:%d\trLcp:%d\n", lLcp, rLcp);

	if (lLcp <  len && pat[lLcp] <= getSeq(0)[lLcp]) return 0;
	if (rLcp >= len || pat[rLcp] >= getSeq(size-1)[rLcp]) return size;

	SFAIDXTYPE left = 1;
	SFAIDXTYPE right = size;
	while (right-left > 1)
	{
		SFAIDXTYPE mid = left + floor((right-left)/2.0);
		POSTYPE mLcp = lLcp <= rLcp? lLcp : rLcp;
		mLcp += lcp((SFACHARTYPE*)getSeq(mid-1)+mLcp, pat+mLcp);
        if ( verbose ) 
            printf("left:%d right:%d mid:%d lLcp:%d rLcp:%d\n", left, right, mid, lLcp, rLcp);
		if (mLcp >= len || pat[mLcp] >= getSeq(mid-1)[mLcp]) { 
            left  = mid; lLcp = mLcp; 
        }
		else { 
            right = mid; rLcp = mLcp; 
        }
        if ( verbose ) 
            printf("left:%d right:%d mid:%d lLcp:%d rLcp:%d mLcp:%d\n", left, right, mid, lLcp, rLcp, mLcp);

	}

    if ( verbose ) std::cout << "right bound:" << left << "\n";
	return left;
}

//  ############################################################################
//  extended by Cuncong Zhong

POSTYPE  GSA::getSuffixLength( SFAIDXTYPE p )
{
    assert( p <= size );
    if ( seqs == NULL ) return 0;
    RIDTYPE rid = Ids[p];//GSFA[p].doc;
    POSTYPE pos = Pos[p];//GSFA[p].pos;
    return strlen(seqs[rid]+pos);
}

char* GSA::getSuffix_explicit(RIDTYPE rid, POSTYPE pos) {
  assert(rid >= 0 && rid < nreads);
  assert(pos >= 0);
  if ( seqs == NULL ) return NULL;
  return seqs[rid]+pos;
}

RIDTYPE GSA::getId(SFAIDXTYPE p)  {
  return Ids[p];
}

POSTYPE GSA::getPos(SFAIDXTYPE p) {
  return Pos[p];
}

POSTYPE GSA::getFullSequenceLength(SFAIDXTYPE p)  {
  assert(p >= 0 && p < size);
  return (POSTYPE) strlen(seqs[Ids[p]]);
}

char* GSA::getSequence_explicit(RIDTYPE rid) {
  assert(rid >= 0 && rid < nreads);
  if ( seqs == NULL ) return NULL;
  return seqs[rid];
}

POSTYPE GSA::getLcp(SFAIDXTYPE p)
{
    assert( p <= size );
    return LCP[p];
}

POSTYPE GSA::getSeqLength_RID(RIDTYPE rid) {
  return strlen(seqs[rid]);
}


