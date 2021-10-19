#include "Loader.h"
#include "BioAlphabet.h"
#include "ReducedAlphabet.h"
#include "SFABuild.h"
#include "UtilFunc.h"


//g++ *.cpp -o t -lboost_filesystem -lboost_system




int main()
{

    BioAlphabet alphabet;
    string db_file = "./";
    UtilFunc util;
    string db_stem = util.GetFileStem(db_file);
    SFABuild db_seq(alphabet, db_file);
    return 1;
}