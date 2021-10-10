#include <iostream>
using std::cout;
using std::endl;
using std::max;
using std::pair;
using std::string;

/**
 * gcc test_io.cpp -I /usr/include/boost/ -L /usr/local/lib/ -lboost_system -lboost_thread -lboost_iostreams -lstdc++
 */
//boost::iostreams::filtering_ostream log_stream;
// class Matrix
// {
//     Matrix(int r,int col);

// private:
//     int rows;
//     int colum;
// };

// class SmithWaterman
// {
//     public:
//         explicit SmithWaterman(string a, string b, double p, int score);
//         pair<string, string> Compute();

//     private:
//         string q;
//         string r;
//         double penalty;
//         int score;
// };



// SmithWaterman::SmithWaterman(string a, string b, double p, int s)
// {
//     q = a;
//     r = b;
//     penalty = p;
//     score = s;
// };

// pair<string,string> SmithWaterman::Compute()
// {
//     int l1 = q.size();
//     int l2 = r.size();
//     int matrix[l1][l2];
//     int trackback[4];

//     for (int i = 0; i < l2; i++)
//     {
//         for (int j = 0; j < l1; j++)
//         {
//             if (q[j] = r[i])
//             {
//                 score = 4;
//             }
//             else
//             {
//                 score = -1;
//             }

//             trackback[0] = matrix[i-1][j-1]+score;
//             trackback[1] = matrix[i - 1][j] + penalty;
//             trackback[2] = matrix[i][j - 1] + penalty;
//             trackback[3] = 0;

//         }
        
//     }
    


// }

int main()
{
    string q = "TCGTCC";
    string r = "ACCTGC";
    int score = 4, penalty = -1, mismatch = -3;
    int l1 = q.size();
    int l2 = r.size();
    int matrix[l1+1][l2+1];
    int trackback[4];
    
    
    for (size_t i = 0; i < l1+1; i++)
    {
        matrix[0][i] = 0;
    }
    for (size_t i = 0; i < l2+1; i++)
    {
        matrix[i][0] = 0;
    }
    for (int i = 1; i < l2+1; i++)
    {
        for (int j = 1; j < l1+1; j++)
        {
            int seqscore;
            if (q[j - 1] == r[i - 1])
            {
                seqscore = score;
            }
            else
            {
                seqscore = mismatch;
            }

            int match = matrix[i - 1][j - 1] + seqscore;
            int hgap = matrix[i - 1][j] + penalty;
            int vgap = matrix[i][j - 1] + penalty;

            int finalscore = max(max(match,hgap),vgap);

            if (finalscore > 0)
            {
                matrix[i][j] = finalscore;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }
    //print matrix
    for (size_t i = 0; i < l1+1; i++)
    {
        for (size_t j = 0; j < l2+1; j++)
        {
            cout << "  " << matrix[i][j];
        }
        cout << "\n";
    }
    //find max score in the matrix

    double max_score = 0;
    int i_max = 0, j_max = 0;

    for (size_t i = 1; i < l1+1; i++)
    {
        for (size_t j = 1; j < l2+1; j++)
        {
            if (matrix[i][j] > max_score)
            {
                max_score = matrix[i][j];
                i_max = i;
                j_max = j;
            }
        }
        
    }
    cout << "max score = " <<max_score<<", i max = "<<i_max<<", j max = "<<j_max<< endl;

    string q_result;
    string r_result;
    // for (size_t i = i_max; i > 1;)
    // {
    //     for (size_t j = j_max; j > 1;)
    //     {
    //        // cout << "matrix[i_max][j_max]" << matrix[i_max-1][j_max-1];
    //         if (matrix[i][j] == matrix[i - 1][j - 1] + score)
    //         {
    //             q_result.push_back(q[i - 1]);
    //             r_result.push_back(r[j - 1]);
    //             i--;
    //             j--;
    //         }
    //         else if (matrix[i][j] == matrix[i-1][j]+ score)
    //         {
    //                 q_result.push_back(q[i - 1]);
    //                 r_result.push_back(-1);
    //                 i--;
    //         }
    //          else if (matrix[i][j] == matrix[i][j-1]+ score)
    //         {

    //                 q_result.push_back(-1);
    //                 r_result.push_back(r[j-1]);
    //                 j--;
    //         }
    //         if (matrix[i][j] == matrix[i - 1][j- 1] + penalty)
    //         {
    //             q_result.push_back(q[i - 1]);
    //             r_result.push_back(r[j - 1]);
    //             i--;
    //             j--;
    //         }
    //         else if (matrix[i][j] == matrix[i-1][j]+ penalty)
    //         {
    //                 q_result.push_back(q[i - 1]);
    //                 r_result.push_back(-1);
    //                 i--;
    //         }
    //          else if (matrix[i][j] == matrix[i][j-1]+ penalty)
    //         {

    //                 q_result.push_back(-1);
    //                 r_result.push_back(r[j-1]);
    //                 j--;
    //         }
    //     }
        
    // }

    while (0 !=matrix[i_max][j_max])
    {
        if (i_max > 1 && j_max > 1 && (matrix[i_max][j_max] == matrix[i_max-1][j_max-1]+score))
        {
            q_result.push_back(q[i_max-1]);
            r_result.push_back(r[j_max-1]);
            i_max--;
            j_max--;
        }else if (i_max > 1 && j_max > 1 && (matrix[i_max][j_max] == matrix[i_max-1][j_max]+penalty))
        {
            q_result.push_back(q[i_max-1]);
            r_result.push_back(-1);
            i_max--;
        }else if (i_max > 1 && j_max > 1 && (matrix[i_max][j_max] == matrix[i_max][j_max-1]+penalty))
        {
            q_result.push_back(-1);
            r_result.push_back(r[j_max-1]);
            j_max--;
        }
    }
    

    
    for (size_t i = 0; i < q_result.size(); i++)
    {   
        cout << " " << q_result[i];
    }

    cout << "\n";

    for (size_t i = 0; i < r_result.size(); i++)
    {
        cout << " " << r_result[i];
    }
    

    return 0;
}