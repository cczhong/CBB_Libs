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
    //  parameters:
    //      s:  the input string to be decomposed
    //      d:  the string for concatenated delimiters; e.g., to use either space ' ' or tab '\t' as delimter set d = " \t"
    //      vs: the vector that holds the output decomposed strings
    void SplitByDelimiter(const std::string &s, const std::string &d, std::vector<std::string> &vs);
    
}

#endif  //__STRINGUTILS_H_