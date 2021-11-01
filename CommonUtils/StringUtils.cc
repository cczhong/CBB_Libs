#include "StringUtils.h"

using namespace std;

void StringUtils::SplitByDelimiter(const string &s, const char & d, vector<string> &vs)
{
    string md = "";    md += d;
    boost::split(vs, s, boost::is_any_of(md), boost::token_compress_on);
    return;
}