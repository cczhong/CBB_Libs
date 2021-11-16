#include "StringUtils.h"

using namespace std;

int main()  {

    // testing NARevComplementary()
    string test = "ACGTAGCTA";
    string test_rv = StringUtils::NARevComplementary(test);
    cout << "test:  " << test << endl;
    cout << "revc:  " << test_rv << endl;

    // testing InPlaceRevComp()
    char *test_array = new char [100];
    strcpy(test_array,  "ACGTAGCTA");
    cout << "test:  " << test_array << endl;
    StringUtils::InplaceRevComp(test_array, 9);
    cout << "revc:  " << test_array << endl;

    return 0;

}