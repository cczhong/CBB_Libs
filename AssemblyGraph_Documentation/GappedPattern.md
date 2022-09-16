# Gapped Pattern

## Methods
```c++
/** 
* Constructor
*/
  GappedPattern(void);

  
 /** Destructor **/
  ~GappedPattern(void);

/** 
* Returns string with no gap
* @param seq string sequence
* @param pid sequence id
* @return string
*/
  std::string GetUngappedStr(const int pid, char *seq)


/** 
* Get pattern length
* @param pid pattern id 
*/  
  int GetPatternLen(const int pid)
  
  
  /** Get pattern weight **/
  int GetPatternWeight(void)  { return pattern_weight; }
  
  /** Get no. of patterns **/
  int GetNumPatterns(void)  { return pattern.size();  }
  
```
## Class
```c++
public:
  explicit GappedPattern(void) {
    InitPattern();
  }
  ~GappedPattern(void) {}
  
  std::string GetUngappedStr(const int pid, char *seq);
  int GetPatternLen(const int pid);
  int GetPatternWeight(void)  { return pattern_weight; }
  int GetNumPatterns(void)  { return pattern.size();  }
};

```

