#ifndef __BIOSEQUENCES_H__
#define __BIOSEQUENCES_H__

#include <algorithm>
#include "Loader.h"
#include "BioAlphabet.h"
#include "ReducedAlphabet.h"
#include "SFABuild.h"
#include "UtilFunc.h"
#include "File.h"
using namespace std;
class BioSequences
{

public:
    BioSequences(){}
    ~BioSequences(){};

/**
 * Create SFA
 * @param seq_type including "DNA", "Protein" and "RNA"
 * @param db_file sequence file path
 * @param sfab SFABuild get SFABuild object
 * @return void
 */
    void createSFA(std::string seq_type,std::string db_file, SFABuild& sfab)
    {
        UtilFunc util;
        if (db_file.size() > 0 && seq_type.size() > 0)
        {
            string db_stem = util.GetFileStem(db_file);
            SFABuild db_seq(getBioAlphabetInstance(seq_type), db_file);
            swap(sfab,db_seq);
        }
        else
        {
            std::cout << "Invalid path"<< "\n";
        }
    }
/**
 * Guess input file format
 * @param seq_file file path 
 * @return int 1 = Fasta format; 2 = Fastq format; 0 = invalid format 
 */
    int guessFormat(std::string& seq_file) 
    {
        const char *file_name = seq_file.c_str();
        std::ifstream ifstrm(file_name, std::ios_base::in);
        std::string line;
        while (std::getline(ifstrm, line)) 
        {
            if (line[0] == '>') 
            {
                return 1;
            }
            if (line[0] == '@') 
            {
                return 2;
            }
            else
            {
                cout << "Invalid file" << endl;
                return 0;
            }
        }
        ifstrm.close();	
    }
/**
 * Check input file format valid
 * @param seq_file file path 
 * @return bool true = a valid format; false = Non-valid format file for input
 */
    bool isValidFormat(std::string& seq_file) 
    {
        const char *file_name = seq_file.c_str();
        std::ifstream ifstrm(file_name, std::ios_base::in);
        std::string line;
        while (std::getline(ifstrm, line)) 
        {
            if (line[0] == '>' || line[0] == '@') 
            {
                return true;
            }
            else
            {
                cout << "Invalid file path" << endl;
                return false;
            }
        }
        ifstrm.close();	
    }   
/**
 * Get number of sequences in sequence file
 * @param seq_file file path 
 * @return int number
 */
    int getNumSeqs(std::string& seq_file)  
    {
        const char *file_name = seq_file.c_str();
        std::ifstream ifstrm(file_name, std::ios_base::in);
        if (!ifstrm) 
        {
            std::cerr << "Cannot open file: " << file_name << std::endl;
            exit (1);
        }
        int count = 0;
        std::string line;
        while (std::getline(ifstrm, line)) 
        {
            if(line[0] == '>'|| line[0] == '@') count++;
        }
            
        ifstrm.close();
        return count;
    }
/**
 * Get BioSequence Instance
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return BioAlphabet  object
 */

    BioAlphabet& getBioAlphabetInstance(std::string seq_type)
    {
        if("DNA" == seq_type)
        {
            return alphabet_DNA = BioAlphabet(DNA);
        }
        if("Protein" == seq_type)
        {
            return alphabet_PROT = BioAlphabet(PROT);
        }
        if("RNA" == seq_type)
        {
            return alphabet_RNA = BioAlphabet(RNA);
        }else
        {
            cout<<"Invalid BioSequence Instance"<<endl;
            return alphabet_OTHER = BioAlphabet(OTHER);
        }
    }
/**
 * Write the 'T' type data with 's' size to 'filename' path
 * @param t T*          the object you write
 * @param st SizeType   the size of the object you write
 * @param filename const char       the path you write to
 * @return bool 
 */
    template<typename T, typename SizeType>    
    bool writeData(T* t, SizeType& st, const char *filename)
    {
        if(sizeof(filename) > 0)
        {
            fio::write<T,SizeType>( t, st, filename );
            return true;
        }
        else
        {
            cout << "Invalid write data path" << endl;
            return false;
        }
    }
/**
 * Read the 'T' type data from 'filename' path
 * @param t T*          the object you write
 * @param filename const char       the path you write to
 * @return size
 */
    template<typename T, typename SizeType>    
    SizeType readData(T* t, const char *filename)
    {
        SizeType s;
        if (sizeof(filename) > 0)
        {
            s= fio::read<T>(t,filename,0);
            return s;
        }
        else
        {
            cout << "Invalid write data path" << endl;
            return -1;
        }
    }
/**
 * Check letters if valid
 * @param c char Sequence letter
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @return bool
 */
    bool checkValid(char c, std::string seq_type)
    {
       bool flag = getBioAlphabetInstance(seq_type).IsValid(c);
       return flag;
    }
/**
 * Check if the frequency of a specific kmer is greater than 1/n of the total length
 * @param seq string Sequence letters
 * @param n a specific kmer is greater than 1/n of the total length
 * @return bool
 */
    bool checkLowComplexity(std::string &seq, const int n)  
    {
        int mer_len = 2;
        unordered_map<string, int> mer_hash;
        for(int i = 0; i <= seq.length() - mer_len; ++ i) {
            mer_hash[seq.substr(i, mer_len)] ++;
        }
        // check if the frequency of a specific kmer is greater than 1/n of the total length
        for(auto it = mer_hash.begin(); it != mer_hash.end(); ++ it) 
        {
            if(it->second > (int) (seq.length() - mer_len) / n) return true;
        }
        return false;
    }
/**
 * Check if the special letter 
 * @param seq_type  including "DNA", "Protein" and "RNA"
 * @param sseq string Sequence letters
 * @param freq_cutoff float frequency cutoff
 * @return bool
 */
    bool checkSpecialLetter(std::string &seq_type, std::string &sseq, float freq_cutoff) 
    {
        BioAlphabet alphabet = getBioAlphabetInstance(seq_type);
        int num_special = 0;
        for(int i = 0; i < sseq.length(); ++ i)  
        {
            if(!alphabet.IsValid(sseq[i]))  
            {
                if(alphabet.IsValid(toupper(sseq[i])))  
                {
                    sseq[i] = toupper(sseq[i]);
                } else  
                {
                    ++ num_special;
                    sseq[i] = alphabet.RandomChar();
                }
            }
        }
        if(num_special / sseq.length() > 1 - freq_cutoff) return false;
        return true;  
    }


    private:
        BioAlphabet alphabet_DNA;
        BioAlphabet alphabet_PROT;
        BioAlphabet alphabet_RNA;
        BioAlphabet alphabet_OTHER;
};


#endif // __BIOSEQUENCES_H__