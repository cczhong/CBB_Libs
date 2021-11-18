#ifndef __BIOSEQUENCES_H__
#define __BIOSEQUENCES_H__

#include <algorithm>
#include <assert.h>
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
    void createSFA(BioAlphabet& ba,std::string db_file, SFABuild& sfab)
    {
        UtilFunc util;
        if (db_file.size() > 0)
        {
            string db_stem = util.GetFileStem(db_file);
            SFABuild db_seq(ba, db_file);
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
        assert(isValidFormat(seq_file));
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
        if("DNA" || "dna" || "Dna" == seq_type)
        {
            return alphabet_DNA = BioAlphabet(DNA);
        }
        if("Protein" || "protein" || "Protein" == seq_type)
        {
            return alphabet_PROT = BioAlphabet(PROT);
        }
        if("RNA" || "rna" || "Rna" == seq_type)
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
 * Load FASTA from 'file_name'
 * @param alphabet BioAlphabet        type of sequence
 * @param file_name const char*       the path you write to
 * @param seq_len int* sequence len
 * @return int size
 */
    int loadingFasta(BioAlphabet &alphabet, string seq_file, char** seq, int* seq_len) 
    {
            const char *file_name = seq_file.c_str();
            // opens the file and read line-by-line
            std::ifstream ifstrm(file_name, std::ios_base::in);
            std::string line, fasta_tag, fasta_seq;
            int count = 0;
            while (std::getline(ifstrm, line)) 
            {
                if (line[0] == '>') 
                {
                    if (fasta_tag != "" && fasta_seq != "") 
                    {
                        recordSequence(seq, fasta_seq, count);
                        seq_len[count] = fasta_seq.length();
                        ++ count;
                    }
                    fasta_tag = line.substr(1, line.length() - 1); fasta_seq = ""; 
                } else fasta_seq += line;
            }
            ifstrm.close();	
            // handle the last sequence		
            if (fasta_tag != "" && fasta_seq != "") 
            {
                recordSequence(seq, fasta_seq, count);
                seq_len[count] = fasta_seq.length();
                ++ count;
            }		
            return count;
    }


    private:

    void recordSequence(char** seq, std::string& single_seq, const int& index) 
    {
        assert(single_seq.length() > 0);
        seq[index] = new char[single_seq.length() + 1];
        strcpy(seq[index], single_seq.c_str());
        return;
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
        BioAlphabet alphabet_DNA;
        BioAlphabet alphabet_PROT;
        BioAlphabet alphabet_RNA;
        BioAlphabet alphabet_OTHER;
};


#endif // __BIOSEQUENCES_H__