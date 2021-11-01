# BioAlphabet 
## Methods
```c++
/**
 *Init BioAlphabet by PROT, DNA, RNA
 */ 
explicit BioAlphabet(enum BioSequence s);
/**
 *Init BioAlphabet by ReducedAlphabet
 */ 
explicit BioAlphabet(ReducedAlphabet &reduced); 
/**
 *@returns Char a random character within the alphabet
 */ 
char RandomChar();
/**
 * Whether a character is valid
 *@param char a character.
 *@return ture or false
 */ 
bool IsValid(const char c);
/**
 *@returns PROT, DNA, RNA Get sequence type
 */ 
enum BioSequence GetSeqType(void);
/** BioAlphabet copy function
 *@param BioAlphabet an alpha_source.
 *@returns BioAlphabe
 */ 
BioAlphabet& operator=(const BioAlphabet &alpha_source);
```
## Paramaters
```c++
enum BioSequence {PROT, DNA, RNA, OTHER};//PROT, DNA, RNA
std::vector<int> char_map_; //char map
std::vector<int> inv_char_map_; //inverse char map
BioSequence seq_type_;// BioSequence type
int alphabet_size_; // number of elements in the alphabet
int num_bits_; // number of bits to represent each letter in the alphabet
```
## Class
```c++

class BioAlphabet
{
    public:
        explicit BioAlphabet();
        explicit BioAlphabet(enum BioSequence s);
        explicit BioAlphabet(ReducedAlphabet &reduced);
        ~BioAlphabet();
        char RandomChar();
        bool IsValid(const char c);
        enum BioSequence GetSeqType(void);
        BioAlphabet& operator=(const BioAlphabet &alpha_source);
    protected:
        void InitProt();
        void InitDNA();
        void InitRNA();
        std::vector<int> char_map_;
        std::vector<int> inv_char_map_;
        BioSequence seq_type_;
        int alphabet_size_; 
        int num_bits_;       

};

```

