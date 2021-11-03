#ifndef __BIOSEQUENCE_H__
#define __BIOSEQUENCE_H__

#include "Loader.h"
#include "BioAlphabet.h"
#include "ReducedAlphabet.h"
#include "SFABuild.h"
#include "UtilFunc.h"

class BioSequences
{

public:
    BioSequences(){}
    ~BioSequences(){};
    void createSFA(BioAlphabet alphabet,std::string db_file)
    {
        UtilFunc util;
        if (db_file.size() > 0)
        {
            string db_stem = util.GetFileStem(db_file);
            SFABuild db_seq(alphabet, db_file);
        }else
        {
            std::cout << "Invalid path" << "\n";
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
 * @param seq_type including "DNA", "Protein" and "RNA"
 * @return BioAlphabet 
 */

    BioAlphabet& getBioSequenceInstance(string seq_type)
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
            cout<<"Invalid Type"<<endl;
            return alphabet_OTHER = BioAlphabet(OTHER);
        }
    }

  void LoadSequence(string filename) 
  {
      int* seq_len;
      const char *file_name = filename.c_str();
      std::ifstream ifstrm(file_name, std::ios_base::in);
      std::string line, fasta_tag, fasta_seq;
      int count = 0;
      while (std::getline(ifstrm, line))
      {
        //   if (line[0] == '>')
        //   {
        //       if (fasta_tag != "" && fasta_seq != "")
        //       {
        //           seq_len[count] = fasta_seq.length();
        //           ++count;
        //       }
        //       fasta_tag = line.substr(1, line.length() - 1);
        //       fasta_seq = "";
        //   } else fasta_seq += line;
        cout << "" <<line<< endl;
    }
    ifstrm.close();
  }	
    private:
        BioAlphabet alphabet_DNA;
        BioAlphabet alphabet_PROT;
        BioAlphabet alphabet_RNA;
        BioAlphabet alphabet_OTHER;
};


#endif // __BIOSEQUENCE_H__