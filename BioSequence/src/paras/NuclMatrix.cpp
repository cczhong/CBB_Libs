#include "NuclMatrix.h"


// int NucleotideMatrix::getMatrixScore(char t1,char t2)
// {
//             int x, y;
//             switch (a)
//             {
//                                 case 'A':  x = 0 ;  break;
//                                 case 'C':  x = 1 ;  break;
//                                 case 'G':  x = 2 ;  break;
//                                 case 'T':  x = 3 ;
//             }
//             switch (b)
//             {
//                                 case 'A':  y = 0 ;  break;
//                                 case 'C':  y = 1 ;  break;
//                                 case 'G':  y = 2 ;  break;
//                                 case 'T':  y = 3 ;
//             }

//             return s[x*4 + y];
// }

template <typename T>
int NucleotideMatrix::getNuclMatrixScore(T &t1,T &t2)
{
    int score = 0;
    for (size_t i = 0; i < sizeof(t1); i++)
    {
        char a = (char)Value_traits<_val>::ALPHABET[t1[i]];
        char b = (char)Value_traits<_val>::ALPHABET[t2[i]];
        score = score + getMatrixScore<char>(a,b);
    }

    return score;
}
/*

NucleotideMatrix::NucleotideMatrix(const char* scoringMatrixFileName, float bitFactor, float scoreBias)
        : SubstitutionMatrix(scoringMatrixFileName, bitFactor, scoreBias) {
    setupLetterMapping();
    reverseLookup = new int[alphabetSize];
    // TODO think about making the matrix dynamic
    reverseLookup[aa2num[static_cast<int>('A')]] = aa2num[static_cast<int>('T')];
    reverseLookup[aa2num[static_cast<int>('G')]] = aa2num[static_cast<int>('C')];
    reverseLookup[aa2num[static_cast<int>('C')]] = aa2num[static_cast<int>('G')];
    reverseLookup[aa2num[static_cast<int>('T')]] = aa2num[static_cast<int>('A')];
    reverseLookup[aa2num[static_cast<int>('X')]] = aa2num[static_cast<int>('X')];
}

*/

/*
void NucleotideMatrix::setupLetterMapping(){
    for(int letter = 0; letter < UCHAR_MAX; letter++){
        char upperLetter = toupper(static_cast<char>(letter));
        /*
         * R.................A or G
         * Y.................C or T
         * S.................G or C
         * W.................A or T
         * K.................G or T
         * M.................A or C
         * B.................C or G or T
         * D.................A or G or T
         * H.................A or C or T
         * V.................A or C or G
         */
        /*
        switch(upperLetter){
            case 'A':
            case 'T':
            case 'G':
            case 'C':
                this->aa2num[static_cast<int>(letter)] = this->aa2num[static_cast<int>(upperLetter)];
                break;
            case 'U':
            case 'W':
                this->aa2num[static_cast<int>(letter)] = this->aa2num[static_cast<int>('T')];
                break;
            case 'K':
            case 'B':
            case 'D':
            case 'V':
            case 'R':
            case 'S':
                this->aa2num[static_cast<int>(letter)] = this->aa2num[static_cast<int>('G')];
                break;
            case 'M':
            case 'Y':
            case 'H':
                this->aa2num[static_cast<int>(letter)] = this->aa2num[static_cast<int>('C')];
                break;
            default:
                this->aa2num[static_cast<int>(letter)] = this->aa2num[static_cast<int>('X')];
                break;
        }
    }
}
*/
/*
NucleotideMatrix::~NucleotideMatrix(){
    delete [] reverseLookup;
}
*/