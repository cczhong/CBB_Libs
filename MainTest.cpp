
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "myBST.h"

using namespace std;
using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
      if (argc != 2)
      {
            std::cout << "You have to provide one test data file and output file name to run the MainTest!\n";
            return 0;
      }
      ifstream inFile;
      ifstream inFile_1;
      ifstream inFile_2;
      inFile.open(argv[1]);
      inFile_1.open(argv[1]);
      inFile_2.open(argv[1]);
      myBST<int> mbst_;
      myBST<int> mbst_1;
      int count = 0;
      int tmp;
      if (inFile.is_open())
      {
            int str;
            while(!inFile.eof())
            {
                inFile >> str;
                mbst_.insert(str);
                mbst_1.insert(str);
            }
            
            cout<<mbst_.isEmpty()<<endl;
            cout<<mbst_.findMin()<<endl;
            cout<<mbst_.findMax()<<endl;
            mbst_.printPostOrder();
            mbst_.printPreOrder();


            count=0;
            str=0;
            while(!inFile_1.eof() and (count !=3))
            {   
                  inFile_1 >> str;
                  mbst_.remove(str);
                  count=count+1;
            }
            count=0;
            str=0;
            while(!inFile_2.eof() and (count !=3))
            {   
              inFile_2 >> str;
              if(mbst_.contains(str))
                  cout <<"1"<<endl;
                else
                  cout <<"0"<<endl;
              count=count+1;
            }
            mbst_.makeEmpty();
            cout<<mbst_.isEmpty()<<endl;

            // while (mbst_1.root != nullptr)
            // {
            //     if(mbst_1.lowestCommonAncestor(mbst_1.root->right,mbst_1.root->left) != nullptr)
            //     {
            //       cout<<(mbst_1.lowestCommonAncestor(mbst_1.root->right,mbst_1.root->left)->element)<<endl;      
            //     }
            //     mbst_1.root= mbst_1.root->right;
            // }

      }
      else
      {
            cout << "The data file cannot be opened";
      }
      inFile.close();
      inFile_1.close();
      inFile_2.close();  

    return 1;
}




