#ifndef __SORTEDLIST_H__
#define __SORTEDLIST_H__



#include "../tool/Utils.h"
#include "SeedCluster.h"
#include "../paras/PackedLocation.h"

template<typename _pos>
class SortedLists
{

	public:

	typedef SortedLists<typename PackedSequenceLocation<_pos>::type> Type;
	class entry
	{
		public:
		entry():
			key (),
			value ()
		{ 

		}
		entry(unsigned key, _pos value):
			key (key),
			value (value)
		{ 

		}
		bool operator<(const entry &rhs) const
		{ 
			return key < rhs.key; 
		}
		unsigned	key;
		_pos		value;
	} __attribute__((packed));

	static char* AllocateBuffer(const RadixCluster &rc)
	{
		//size_t l = rc.MaxChunkSize();
		//cout << "sizeof(entry) = " <<sizeof(entry)<<", rc.MaxChunkSize() = "<<rc.MaxChunkSize()<< endl;
		return new char[sizeof(entry) * rc.MaxChunkSize()];
	}

	template<typename _val>
	SortedLists(char *buffer, const SequenceSet<_val> &seqs, const Shape &sh, const ShapeCluster &hst, const SeedPartitionRange &range):
		limits_ (hst, range),
		data_ (reinterpret_cast<entry*>(buffer))
	{
		
		CreateContext<_val> build_context (seqs, sh, range, CreateIterators(hst));
		ThreadPool(build_context, ConstParameter::seqp, VATOptions::threads());
		SortContext sort_context (*this);
		ThreadPool(sort_context, ConstParameter::seedp, VATOptions::threads());
	}

	template<typename _t>
	class IterateBase
	{
		public:
		IterateBase(_t *i, _t *end):
			i (i),
			end (end),
			n (count())
		{ 

		}

		//equal key
		size_t count() const
		{
			_t *k (i);
			size_t n (0);
			while(k < end && (k++)->key == i->key)
				++n;
			return n;
		}
		void operator++()
		{ 
			i += n; 
			n = count(); 
		}
		_pos operator[](unsigned k) const
		{ 
			return (i+k)->value; 
		}
		_t* get(unsigned k)
		{ 
			return i+k; 
		}
		bool at_end() const
		{ 
			return i >= end; 
		}
		//get key value
		unsigned key() const
		{ 
			return i->key; 
		}
		_t *i, *end;
		size_t n;
	};

	typedef IterateBase<entry> iterator;
	typedef IterateBase<const entry> const_iterator;

	const_iterator getPartitionConstBegin(unsigned p) const
	{ 
		return const_iterator (getConstPtrbegin(p), getConstPtrend(p)); 
	}

	iterator getPartitionBegin(unsigned p) const
	{ 
		return iterator (getPtrbegin(p), getPtrend(p));
	}

 private:

	typedef StaticMatrix<entry*,ConstParameter::seqp,ConstParameter::seedp> Ptr_set;

	class BufferIterator
	{
		public:
		static const unsigned BUFFER_SIZE = 16;
		BufferIterator(entry **ptr)
		{
			memset(n, 0, sizeof(n));
			memcpy(this->ptr, ptr, sizeof(this->ptr));
		}
		//push seed and positions into a entry
		void push(seed key, _pos value, const SeedPartitionRange &range)
		{
			const unsigned p (seedPartition(key));
			if(range.Contains(p)) {
				assert(n[p] < BUFFER_SIZE);
				buf[p][n[p]++] = entry (seedPartitionOffset(key), value);
				if(n[p] == BUFFER_SIZE)
					flush(p);
			}
		}
		void flush(unsigned p)
		{
			memcpy(ptr[p], buf[p], n[p] * sizeof(entry));
			ptr[p] += n[p];
			n[p] = 0;
		}
		void flush()
		{
			for(unsigned p=0;p<ConstParameter::seedp;++p)
				if(n[p] > 0)
					flush(p);
		}
		entry* ptr[ConstParameter::seedp];
		entry  	 buf[ConstParameter::seedp][BUFFER_SIZE];
		uint8_t  n[ConstParameter::seedp];
	};

	entry* getPtrbegin(unsigned i) const
	{ 
		return &data_[limits_[i]]; 
	}

	entry* getPtrend(unsigned i) const
	{ 
		return &data_[limits_[i+1]]; 
	}

	const entry* getConstPtrbegin(unsigned i) const
	{ 
		return &data_[limits_[i]]; 
	}

	const entry* getConstPtrend(unsigned i) const
	{ 
		return &data_[limits_[i+1]]; 
	}

	template<typename _val>
	class CreateContext
	{
		public:
		CreateContext(const SequenceSet<_val> &seqs, const Shape &sh, const SeedPartitionRange &range, Ptr_set *iterators):
			seqs (seqs),
			sh (sh),
			range (range),
			iterators (iterators),
			seq_partition (seqs.Partition())
		{ 
			
		}

		CreateContext(const SequenceSet<_val> &seqs, const SeedPartitionRange &range, Ptr_set *iterators):
			seqs (seqs),
			range (range),
			iterators (iterators),
			seq_partition (seqs.partition())
		{ }

		void operator()(unsigned thread_id, unsigned seqp) const
		{
			CreateSeqPartition<_val>(seqs,
					seq_partition[seqp],
					seq_partition[seqp+1],
					(*iterators)[seqp],
					sh,
					range);
		}
		const SequenceSet<_val> &seqs;
		const Shape &sh;
		const SeedPartitionRange &range;
		const auto_ptr<Ptr_set> iterators;
		const vector<size_t> seq_partition;
	};

	template<typename _val>
	static void CreateSeqPartition(const SequenceSet<_val> &seqs, unsigned begin, unsigned end, entry **ptr, const Shape &sh, const SeedPartitionRange &range)
	{
		uint64_t key;
		auto_ptr<BufferIterator> it (new BufferIterator(ptr));
		for(size_t i=begin;i<end;++i) {
			const Sequence<const _val> seq = seqs[i];
			if(seq.length()<sh.length_) continue;
			for(unsigned j=0;j<seq.length()-sh.length_+1; ++j) 
			{
				if(sh.setSeed(key, &seq[j]))
					it->push(key, seqs.position(i, j), range);
			}
		}
		it->flush();
	}

	Ptr_set* CreateIterators(const ShapeCluster &hst) const
	{
		Ptr_set *iterators = new Ptr_set;
		for(unsigned i=0;i<ConstParameter::seedp;++i)
			(*iterators)[0][i] = getPtrbegin(i);

		for(unsigned i=1;i<ConstParameter::seqp;++i) {
			for(unsigned j=0;j<ConstParameter::seedp;++j)
				(*iterators)[i][j] = (*iterators)[i-1][j] + hst[i-1][j];
		}
		return iterators;
	}
//sort seeds
	class SortContext
	{
		public:
		SortContext(SortedLists &sl):
			sl (sl)
		{ 

		}
		void operator()(unsigned thread_id ,unsigned seedp) const
		{ 
			std::sort(sl.getPtrbegin(seedp), sl.getPtrend(seedp)); 
		}
		SortedLists &sl;
		
	};

	class Limits : public vector<size_t>
	{
		public:
		Limits(const ShapeCluster &hst, const SeedPartitionRange &range)
		{
			this->push_back(0);
			for(unsigned i=0;i<ConstParameter::seedp;++i) {

				this->push_back(this->operator[](i) + (range.Contains(i) ? PartitionSize(hst, i) : 0));
			}
		}
	};

	const Limits limits_;
	entry *data_;

};



#endif // __SORTEDLIST_H__