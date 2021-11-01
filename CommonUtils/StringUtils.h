#ifndef __STRINGUTILS_H_
#define __STRINGUTILS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace StringUtils   
{
    // split a single string into a set of strings using a specific delimiter
    // multiple delimiters are considered as a single one
    void SplitByDelimiter(const std::string &s, const char & d, std::vector<std::string> &vs);
    
}

#endif  //__STRINGUTILS_H_