#ifndef __ALIGNFLOWS_H__
#define __ALIGNFLOWS_H__

#include "../sequence/Queries.h"
#include "../paras/statistics.h"
#include "../sequence/Reference.h"
#include "../sequence/LoadSeqs.h"
#include "../tool/VATSetup.h"
//#include "../search_db/VATSearch.h"
#include "../sequence/frequency_mask.h"
#include "../alignment/AlignQueries.h"
#include "../output/VATJoinBlock.h"
#include "../tool/Thread.h"
#include "../alignment/JoinRange.h"
#include "./CreateSearchContext.h"

class WorkFlows
{

		public:
		template<typename T, typename _locr, typename _locq, typename _locl> 
		void handleShapes(unsigned shapeid,unsigned query_chunk,char *query_buffer,char *ref_buffer)
		{
			using std::vector;
			::partition p (ConstParameter::seedp, VATOptions::lowmem);
			for(unsigned chunk=0;chunk < p.parts; ++chunk) 
			{

				//cout << "Processing query chunk " << query_chunk << ", reference chunk " << current_ref_block << ", shape " << shapeid << ", index chunk " << chunk << '.' << endl;
				const SeedPartitionRange range (p.getMin(chunk), p.getMax(chunk));
				Current_Range = range;
				typename SortedLists<_locr>::Type ref_idx (ref_buffer,
						*ReferenceSeq<T>::data_,
						ShapeConfigure::instance.getShape(shapeid),
						ref_hst.get(VATOptions::index_mode = 1, shapeid),
						range);
				//ReferenceSeq<T>::get_nc().template createMask<_locr>(shapeid, range, ref_idx);

				typename SortedLists<_locq>::Type query_idx (query_buffer,
						*QuerySeqs<T>::data_,
						ShapeConfigure::instance.getShape(shapeid),
						query_hst->get(VATOptions::index_mode = 1, shapeid),
						range);

				CreateSearchContext<T,_locr,_locq,_locl> context (shapeid, ref_idx, query_idx);
				ThreadPool(context, ConstParameter::seedp, VATOptions::threads());
			}
			return;
		}

/**
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
		void handleShape(unsigned shapeid,unsigned query_chunk,char *query_buffer,char *ref_buffer)
		{
			using std::vector;
			::partition p (ConstParameter::seedp, VATOptions::lowmem);
			for(unsigned chunk=0;chunk < p.parts; ++chunk) 
			{

				//cout << "Processing query chunk " << query_chunk << ", reference chunk " << current_ref_block << ", shape " << shapeid << ", index chunk " << chunk << '.' << endl;
				const SeedPartitionRange range (p.getMin(chunk), p.getMax(chunk));
				Current_Range = range;
				typename SortedLists<uint64_t>::Type ref_idx (ref_buffer,
						*ReferenceSeq<Protein>::data_,
						ShapeConfigure::instance.getShape(shapeid),
						ref_hst.get(VATOptions::index_mode = 1, shapeid),
						range);
				//ReferenceSeq<T>::get_nc().template createMask<_locr>(shapeid, range, ref_idx);

				typename SortedLists<uint64_t>::Type query_idx (query_buffer,
						*QuerySeqs<Protein>::data_,
						ShapeConfigure::instance.getShape(shapeid),
						query_hst->get(VATOptions::index_mode = 1, shapeid),
						range);

				CreateSearchContext<Protein,uint64_t,uint64_t,uint32_t> context (shapeid, ref_idx, query_idx);
				ThreadPool(context, ConstParameter::seedp, VATOptions::threads());
			}
			return;
		}

		template <typename T, typename locr, typename locq, typename locl> 
		void handleRefBlk(DatabaseController<T> &dbfile,unsigned query_chunk,pair<size_t,size_t> query_len_bounds,
			char *query_buffer,VATOutput &vout,vector<TempFile> &tmp_file)
		{
			//cout << "handling reference block" << endl;
			//cout << "	dbfile.file_name = " <<dbfile.file_name<<endl;
			//dbfile.file_name;
			ReferenceSeq<T>::data_ = new MaskSequenceSet<T>(dbfile);
			//ReferenceSeq<T>::data_
			//cout << "handling 1.........." << endl;
			ReferenceIds::data_ = new BaseStringSet<char,0> (dbfile);
			//cout << "handling 2.........." << endl;
			ref_hst.load(dbfile);

			SetSearchParams(query_len_bounds, ReferenceSeq<T>::data_->letters());
			//cout << "ReferenceSeq<T>::get().get_length() = " <<ReferenceSeq<T>::get().get_length()<<endl;
			//cout << "ReferenceSeq<T>::data_->letters() = " <<ReferenceSeq<T>::data_->letters()<<endl;

			ref_map.init(ReferenceSeq<T>::get().getLength());

			//cout << "VATOptions::tmpdir = " <<VATOptions::tmpdir<<", VATOptions::Mem_Buffered() = "<<VATOptions::Mem_Buffered()<<endl;

			char *ref_buffer = SortedLists<locr>::Type::AllocateBuffer(ref_hst);

			//cout << "5.1" << endl;

			TracePtBuffer<locr,locl>::instance = new TracePtBuffer<locr,locl> (QuerySeqs<T>::data_->getLength(),
					VATOptions::tmpdir = "/dev/shm",
					VATOptions::memBuffered());

			//cout << "handling 6.........." << endl;

			for(unsigned i=0;i<ShapeConfigure::instance.count();++i)
				handleShapes<T, locr, locq, locl>(i, query_chunk, query_buffer, ref_buffer);

			TracePtBuffer<locr,locl>::instance->close();
			exception_state.sync();
			delete[] ref_buffer;


			OutputStreamController* out;
			if(refheader.n_blocks > 1) {
				tmp_file.push_back(TempFile ());
				out = new OutputStreamController (tmp_file.back());
			} else
				out = &vout.stream();
			//cout << "handling 7.........." << endl;
			alignQueries<T,locr,locl>(*TracePtBuffer<locr,locl>::instance, out);
			//cout << "handling 8.........." << endl;

			delete TracePtBuffer<locr,locl>::instance;

			if(refheader.n_blocks > 1)
			{
				out->close();
				delete out;
			}

			delete ReferenceSeq<T>::data_;
			delete ReferenceIds::data_;

			return;
		}
	

		template <typename T, typename locr, typename locq, typename locl> 
		void handleQueryBlk(DatabaseController<T> &dbfile,unsigned queryblk, pair<size_t,size_t> querylenbounds,VATOutput &vo)
		{
			char *query_buffer = SortedLists<locq>::Type::AllocateBuffer(*query_hst);
			vector<TempFile> tmp_file;

			dbfile.rewind();
			for(current_ref_block=0;current_ref_block < refheader.n_blocks;++current_ref_block)
				handleRefBlk<T,locr,locq,locl>(dbfile, queryblk, querylenbounds, query_buffer, vo, tmp_file);

			delete[] query_buffer;

			if (refheader.n_blocks > 1)
			{
				JoinBlocks<T>(refheader.n_blocks, vo, tmp_file);
			}

			delete QuerySeqs<T>::data_;
			delete QueryIds::data_;
			delete QuerySourceSeqs::data_;
			return;
		}
/**
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
		//template <typename T, typename locr, typename locq, typename locl> 
		void handleQueryBlock(DatabaseController<Protein> &dbfile,unsigned queryblk, pair<size_t,size_t> querylenbounds,VATOutput &vo)
		{

			char *query_buffer = SortedLists<uint64_t>::Type::AllocateBuffer(*query_hst);
			vector<TempFile> tmp_file;

			dbfile.rewind();
			for(current_ref_block=0;current_ref_block < refheader.n_blocks;++current_ref_block)
				handleRefBlock(dbfile, queryblk, querylenbounds, query_buffer, vo, tmp_file);

			delete[] query_buffer;

			if (refheader.n_blocks > 1)
			{
				JoinBlocks<Protein>(refheader.n_blocks, vo, tmp_file);
			}

			delete QuerySeqs<Protein>::data_;
			delete QueryIds::data_;
			delete QuerySourceSeqs::data_;
			return;
		}
/**
 * 
 * locr = uint64_t
 * locq = uint64_t
 * locl = uint32_t
 */
 
		//template <typename T, typename locr, typename locq, typename locl> 
		void handleRefBlock(DatabaseController<Protein> &dbfile,unsigned query_chunk,pair<size_t,size_t> query_len_bounds,
			char *query_buffer,VATOutput &vout,vector<TempFile> &tmp_file)
		{
			//cout << "handling reference block" << endl;
			//cout << "	dbfile.file_name = " <<dbfile.file_name<<endl;
			//dbfile.file_name;
			ReferenceSeq<Protein>::data_ = new MaskSequenceSet<Protein>(dbfile);
			//ReferenceSeq<T>::data_
			//cout << "handling 1.........." << endl;
			ReferenceIds::data_ = new BaseStringSet<char,0> (dbfile);
			//cout << "handling 2.........." << endl;
			ref_hst.load(dbfile);
			//cout << "ReferenceSeq<T>::get().get_length() = " <<ReferenceSeq<T>::get().get_length()<<endl;
			//cout << "ReferenceSeq<T>::data_->letters() = " <<ReferenceSeq<T>::data_->letters()<<endl;
			//cout << "query_len_bounds = " <<query_len_bounds.first<<endl;
			//cout << "ReferenceSeq<T>::data_->letters() = " <<ReferenceSeq<T>::data_->letters()<<endl;
			SetSearchParams(query_len_bounds, ReferenceSeq<Protein>::data_->letters());
			//cout << "ReferenceSeq<T>::get().get_length() = " <<ReferenceSeq<T>::get().get_length()<<endl;
			//cout << "ReferenceSeq<T>::data_->letters() = " <<ReferenceSeq<T>::data_->letters()<<endl;

			ref_map.init(ReferenceSeq<Protein>::get().getLength());

			//cout << "VATOptions::tmpdir = " <<VATOptions::tmpdir<<", VATOptions::Mem_Buffered() = "<<VATOptions::Mem_Buffered()<<endl;

			char *ref_buffer = SortedLists<uint64_t>::Type::AllocateBuffer(ref_hst);

			//cout << "5.1" << endl;

			TracePtBuffer<uint64_t,uint32_t>::instance = new TracePtBuffer<uint64_t,uint32_t> (QuerySeqs<Protein>::data_->getLength(),
					VATOptions::tmpdir = "/dev/shm",
					VATOptions::memBuffered());

			//cout << "handling 6.........." << endl;

			for(unsigned i=0;i<ShapeConfigure::instance.count();++i)
				handleShape(i, query_chunk, query_buffer, ref_buffer);

			TracePtBuffer<uint64_t,uint32_t>::instance->close();
			exception_state.sync();
			delete[] ref_buffer;


			OutputStreamController* out;
			if(refheader.n_blocks > 1) {
				tmp_file.push_back(TempFile ());
				out = new OutputStreamController (tmp_file.back());
			} else
				out = &vout.stream();

			alignQueries<Protein,uint64_t,uint32_t>(*TracePtBuffer<uint64_t,uint32_t>::instance, out);


			delete TracePtBuffer<uint64_t,uint32_t>::instance;

			if(refheader.n_blocks > 1)
			{
				out->close();
				delete out;
			}

			delete ReferenceSeq<Protein>::data_;
			delete ReferenceIds::data_;

			return;
		}

		template <typename T, typename locr> 
		void runAlignment(DatabaseController<T> &dbfile)
		{
			ShapeConfigure::instance = ShapeConfigure (VATOptions::index_mode, T());
			const SequenceFileFormat<Protein> *format_n (guessFormat<Protein>(VATOptions::query_file));

			InputStreamController query_file (VATOptions::query_file, true);
			unsigned current_query_chunk=0;
			VATOutput vo;

			for(;;++current_query_chunk) 
			{
				size_t n_query_seqs;
				n_query_seqs = loadingSequence(query_file, *format_n, &QuerySeqs<Protein>::data_, QueryIds::data_, QuerySourceSeqs::data_, (size_t)(VATOptions::chunk_size * 1e9));

				if(n_query_seqs == 0)
					break;
				QuerySeqs<T>::data_->printStats();
				query_hst = auto_ptr<RadixCluster> (new RadixCluster (*QuerySeqs<T>::data_, T()));
				const pair<size_t,size_t> query_len_bounds = QuerySeqs<T>::data_->lenBounds(ShapeConfigure::get().getShape(0).length_);
				const bool long_addressing_query = QuerySeqs<T>::data_->rawLen() > (size_t)std::numeric_limits<uint32_t>::max();

				// if(query_len_bounds.second <= (size_t)std::numeric_limits<uint8_t>::max()) 
				// {
				// 	//if(long_addressing_query)
				// //handleQueryBlk<T,locr,uint64_t,uint8_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// 	// else
				// 	// 	handleQueryBlk<T,locr,uint32_t,uint8_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// // } else if(query_len_bounds.second <= (size_t)std::numeric_limits<uint16_t>::max()) 
				// // {
				// // 	//if(long_addressing_query)
				//  		handleQueryBlk<T,locr,uint64_t,uint16_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// // 	// else
				// // 	// 	handleQueryBlk<T,locr,uint32_t,uint16_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// // } else 
				// // {
				// // 	//if(long_addressing_query)
				// // 		handleQueryBlk<T,locr,uint64_t,uint32_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// // 	// else
				// // 	// 	handleQueryBlk<T,locr,uint32_t,uint32_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				// }
				handleQueryBlk<T,locr,uint64_t,uint32_t>(dbfile, current_query_chunk, query_len_bounds, vo);
				
			}
			dbfile.close();
			statistics.print();
			//vo.finish();
			return;
		}

		template<typename T>
		void run(T)
		{
			DatabaseController<T> db_file;
			VATOptions::setOptions<T>(refheader.block_size);
			cout << "Reference = " << VATOptions::database << endl;
			cout << "Sequences = " << refheader.sequences << endl;
			cout << "Letters = " << refheader.letters << endl;
			cout << "Block size = " << (size_t)(refheader.block_size * 1e9) << endl;

			if(refheader.long_addressing)
			{
				runAlignment<T,uint64_t>(db_file);
			}
			else
			{
				runAlignment<T,uint32_t>(db_file);	
			}
			return;
		}
};


#endif // __ALIGNFLOWS_H__