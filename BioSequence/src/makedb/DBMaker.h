#ifndef __DBMAKER_H__
#define __DBMAKER_H__



#include <iostream>
#include "../paras/Options.h"
#include "../sequence/Reference.h"
#include "../paras/VATExceptions.h"
#include "../paras/statistics.h"
#include "../sequence/LoadSeqs.h"
#include "../tool/SeqFileFormat.h"
//#include "../join/RadixCluster.h"
//Create Database
class DBMake{
        public:
        template <class T>
        void DBMakeConroller(T)
        {
            using std::cout;
            using std::endl;
            cout << "Database file name = " <<VATOptions::input_ref_file<<endl;
            InputStreamController dbfile(VATOptions::input_ref_file,true);
            refheader.block_size = VATOptions::chunk_size;
           // cout << "Database block size = " <<refheader.block_size<<endl;
            size_t chunk = 0;
            OutputStreamController os(VATOptions::database);
            os.write(&refheader,1);
            for (;;++chunk)
            {
                SequenceSet<Nucleotide> *ss;
                size_t num_seq = loadingSequence(dbfile,FASTAFormat<Protein>(),(SequenceSet<Protein>**)&ReferenceSeq<Protein>::data_,ReferenceIds::data_,ss,(size_t)(VATOptions::chunk_size * 1e9));

                if(0 == num_seq)
                {
                    break;
                }
                refheader.letters = refheader.letters + ReferenceSeq<T>::data_->letters();
                refheader.sequences += num_seq;
                const bool long_addressing = ReferenceSeq<T>::data_->rawLen() > (size_t)std::numeric_limits<uint32_t>::max();
                refheader.long_addressing = refheader.long_addressing == true ? true : long_addressing;
                //cout << "refheader.long_addressing = " << refheader.long_addressing << endl;
                ReferenceSeq<T>::data_->printStats();
                //cout << "ReferenceSeq<T>::get().get_length() = " <<ReferenceSeq<T>::get().get_length()<<endl;
                RadixCluster *rc = new RadixCluster(*ReferenceSeq<T>::data_, T());
                ReferenceSeq<T>::data_->save(os);
                ReferenceIds::get().save(os);
                rc->save(os);
                delete ReferenceSeq<T>::data_;
                delete ReferenceIds::data_;
                delete ss;
                delete rc;
            }
            refheader.n_blocks = chunk;
            os.seekp(0);
            os.write(&refheader, 1);
            os.close();
        }
};

#endif // __DBMAKER_H__