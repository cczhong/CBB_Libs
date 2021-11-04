#include "StringUtils.h"

using namespace std;

void StringUtils::SplitByDelimiter(const std::string &s, const std::string & d, vector<string> &vs)
{
    boost::split(vs, s, boost::is_any_of(d), boost::token_compress_on);
    return;
}