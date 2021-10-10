#ifndef __SEEDCLUSTER_H__
#define __SEEDCLUSTER_H__

#include "../paras/Seed.h"
#include "../sequence/SequenceSet.h"
#include "../paras/ShapeConfigure.h"
#include "../tool/Thread.h"
#include "../tool/BinaryFile.h"
#include <stdlib.h>

using std::vector;

void static encode0Rle(const int32_t *data, size_t len, OutputStreamController &out)
{
	const int32_t *p = data, *end = data + len;
	int32_t n = 0;
	while(p < end) {
		while(p < end && *p == 0) 
		{
			//cout << "n= " <<n<<", p = "<<p<< endl;
			--n;
			++p;
		}
		if(n < 0) {
			//cout << "n= " <<n<< endl;
			out.write(&n, 1);
			n = 0;
		}
		if(p < end) 
		{
			//cout << "p= " <<p<< endl;
			out.write(p, 1);
			++p;
		}
	}
	//cout << "after encode zero p = " <<*p<<endl;
}

void static decode0Rle(int32_t *data, size_t len, InputStreamController &in)
{
	size_t n = 0;
	while(n < len) {
		int32_t x;
		in.read(&x, 1);
		if(x >= 0) {
			*(data++) = x;
			++n;
		} else {
			for(int32_t i=0;i>x;--i) {
				*(data++) = 0;
				++n;
			}
		}
	}
		//cout << "after decode zero data = " <<*data<< endl;

}


typedef int32_t ShapeCluster[ConstParameter::seqp][ConstParameter::seedp];

class SeedPartitionRange
{
	public:
	SeedPartitionRange():
		begin (0),
		end (0)
	{ }
	SeedPartitionRange(unsigned begin, unsigned end):
		begin (begin),
		end (end)
	{ }
	bool Contains(unsigned i) const
	{ return i >= begin && i < end; }
	unsigned getBegin() const
	{ return begin; }
	unsigned getEnd() const
	{ return end; }
	bool Lower(unsigned i) const
	{ return i < begin; }
	bool LowerOrEqual(unsigned i) const
	{ return i < end; }
private:
	unsigned begin, end;
} ;
static SeedPartitionRange Current_Range;

size_t static PartitionSize(const ShapeCluster &hst, unsigned p)
{
	size_t s (0);
	for(unsigned i=0;i<ConstParameter::seqp;++i)
		s += hst[i][p];
	return s;
}

size_t static ShapeClusterSize(const ShapeCluster &hst, const SeedPartitionRange &range)
{

	size_t s (0);
	for(unsigned i=range.getBegin();i<range.getEnd();++i)
		s += PartitionSize(hst, i);
	//cout << "s = " <<s<< endl;
	return s;
}

class RadixCluster
{
	public:

	RadixCluster()
	{ 
		
	}

	template<typename _val>
	RadixCluster(const SequenceSet<_val> &seqs, const _val&)
	{
		//cout << "seqs.letters() = " <<seqs.letters()<< endl;
		memset(data_, 0, sizeof(data_));
		Build_context<_val> context (seqs, *this);
		ThreadPool(context, ConstParameter::seqp, VATOptions::threads());
	}

	const ShapeCluster& get(unsigned index_mode, unsigned sid) const
	{
		//cout << "index = " <<index_mode<<endl;
		return data_[index_mode - 1][sid];
	}

	size_t MaxChunkSize() const
	{
		size_t max (0);
		::partition p (ConstParameter::seedp, VATOptions::lowmem = 4);
		//cout << "p = " <<p.size<<",part = "<<p.parts<<",VATOptions::index_mode ="<<VATOptions::index_mode<<endl;
		for(unsigned shape=0;shape < ShapeConfigure::get().count();++shape)
			for(unsigned chunk=0;chunk < p.parts; ++chunk)
				max = std::max(max, ShapeClusterSize(data_[VATOptions::index_mode-1][shape], SeedPartitionRange(p.getMin(chunk), p.getMax(chunk))));
		//cout << "max = " <<max<< endl;
		return max;
	}

	void save(OutputStreamController &out) const
	{
		encode0Rle(reinterpret_cast<const int32_t *>(data_), sizeof(data_) / sizeof(int32_t), out);
	}

	void load(InputStreamController &in)
	{
		decode0Rle(reinterpret_cast<int32_t*>(data_), sizeof(data_)/sizeof(int32_t), in);
	}

private:

	template<typename _val>
	struct Build_context
	{
		Build_context(const SequenceSet<_val> &seqs, RadixCluster &hst):
			seqs (seqs),
			cfgs (ShapeConfigs<_val>()),
			seq_partition (seqs.Partition()),
			hst (hst)
		{ }
		void operator()(unsigned thread_id, unsigned seqp) const
		{ hst.buildSeqPartition(seqs, seqp, seq_partition[seqp], seq_partition[seqp+1], cfgs); }
		const SequenceSet<_val> &seqs;
		const vector<ShapeConfigure> cfgs;
		const vector<size_t> seq_partition;
		RadixCluster &hst;
	};

	template<typename _val>
	void buildSeqPartition(const SequenceSet<_val> &seqs,
			const unsigned seqp,
			const size_t begin,
			const size_t end,
			const vector<ShapeConfigure> &cfgs)
	{
		//int id = 0;
		assert(seqp < ConstParameter::seqp);
		uint64_t key;
		for(size_t i=begin;i<end;++i) {

			assert(i < seqs.get_length());
			const Sequence<const _val> seq = seqs[i];
			//cout << "seq.length() = " <<seq.length()<< endl;
			if (seq.length() < ConstParameter::min_shape_len)
				continue;
			for(unsigned j=0;j<seq.length()+1-ConstParameter::min_shape_len; ++j)
				for(vector<ShapeConfigure>::const_iterator cfg = cfgs.begin(); cfg != cfgs.end(); ++cfg) {
					assert(cfg->mode() < ConstParameter::index_modes);
					assert(cfg->count() <= ConstParameter::max_shapes);
					for(unsigned k=0;k<cfg->count(); ++k)
					{
						if(j+cfg->getShape(k).length_ < seq.length()+1 && cfg->getShape(k).setSeed(key, &seq[j]))
						{
							//cout<<"id = "<<id++<< ", cfg->mode()= " <<cfg->mode()<<",k = "<<k<< ", seqp = "<<seqp<<",SeedPartition(key) = "<<SeedPartition(key)<<endl;
							++data_[cfg->mode()][k][seqp][seedPartition(key)];
						}
					}
				}

		}
	}

	template<typename _val>
	static vector<ShapeConfigure> ShapeConfigs()
	{
		vector<ShapeConfigure> v;
		if(VATOptions::cmd == VATOptions::createdb) {
			for(unsigned i=1;i<=ConstParameter::index_modes;++i)
				v.push_back(ShapeConfigure (i, _val()));
		} else
			v.push_back(ShapeConfigure (VATOptions::index_mode, _val()));
		return v;
	}
	ShapeCluster data_[ConstParameter::index_modes][ConstParameter::max_shapes];
};


#endif // __SEEDCLUSTER_H__