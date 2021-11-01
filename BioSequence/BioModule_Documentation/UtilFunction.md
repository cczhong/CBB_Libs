# UtilFunction
## Methods
```c
/**
* Get string from the file path
* @param  path std::string file path		
* @return string text
*/
std::string GetFileStem(const std::string& path);
/**
* Compare String With LCP
* @param  a char* a set of character 
* @param  b char* a set of character 
* @param  begin 	
* @return pair<int, int> Longest Common Prefix length of the two strings
*/
std::pair<int, int>	CmpStrWithLCP(const char* a, const char* b, const int& begin);
/**
* Compare String With LCP in a reversed direction
* @param  a char* a set of character 
* @param  b char* a set of character 
* @param  begin int& begin position
* @return pair<int, int> Longest Common Prefix length of the two strings
*/
std::pair<int, int> CmpWithLCPRev(const char *a, const char *b, const int& begin);
/**
* Get time
*/
double MyTime (void);
/**
* Print Elapsed time
*/
void PrintElapsed( double s, double e, const char *task );
```
## Class
```c++
class UtilFunc
{
public:
    UtilFunc() ;
    ~UtilFunc() ;
    std::string GetFileStem(const std::string& path);
	std::pair<int, int>	CmpStrWithLCP(const char* a, const char* b, const int& begin);
	std::pair<int, int> CmpWithLCPRev(const char *a, const char *b, const int& begin);
	double MyTime (void);
	void PrintElapsed( double s, double e, const char *task );
};

```