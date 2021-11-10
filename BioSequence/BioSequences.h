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
    void createSFA(string seq_type,string db_file, SFABuild& sfab)
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

    BioAlphabet& getBioAlphabetInstance(string seq_type)
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
    bool checkValid(char c, string seq_type)
    {
       bool dna = alphabet_DNA.IsValid(c);
       bool prot = alphabet_PROT.IsValid(c);
       bool rna = alphabet_RNA.IsValid(c);
       if(seq_type == "DNA")
       {
           return dna;
       }else if (seq_type == "RNA")
       {
           return rna;
       }else if(seq_type == "PROT")
       {
           return prot;
       }else
       {
           return false;
       }
    }

    private:
        BioAlphabet alphabet_DNA;
        BioAlphabet alphabet_PROT;
        BioAlphabet alphabet_RNA;
        BioAlphabet alphabet_OTHER;
};


#endif // __BIOSEQUENCES_H__