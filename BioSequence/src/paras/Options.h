#ifndef __OPTIONS_H__
#define __OPTIONS_H__


#include <stdint.h>
#include <string>

using std::string;

namespace VATOptions
{
    extern string input_ref_file;
    extern uint32_t	threads_;
	extern string	database;
	extern string	query_file;
	extern uint32_t	merge_seq_treshold;
	extern uint32_t	block_size;
	extern uint32_t	hit_cap;
	extern int		min_ungapped_raw_score;
	extern uint32_t shapes;
	extern uint32_t	index_mode;
	extern uint64_t	max_alignments;
	extern string	match_file1;
	extern string	match_file2;
	extern int		padding;
	extern uint32_t	output_threads;
	extern uint32_t compression;
	extern uint32_t lowmem;
	extern double	chunk_size;
	extern unsigned min_identities;
	extern unsigned min_identities2;
	extern int		xdrop;
	extern unsigned window;
	extern int		min_hit_score;
	extern int		hit_band;
	extern unsigned	min_compressed_identities;
	extern int		min_seed_score;
	extern unsigned	seed_signatures;
	extern double	min_bit_score;
	extern unsigned	run_len;
	extern bool		alignment_traceback;
	extern double	max_seed_freq;
	extern string	tmpdir;
	extern bool		long_mode;
	extern int		gapped_xdrop;
	extern double	max_evalue;
	extern string	kegg_file;
	extern int		gap_open;
	extern int		gap_extend;
	extern string	matrix;
	extern string	seg;
	extern bool		verbose;
	extern bool		bug_infor;
	extern bool		have_ssse3;
	extern bool		salltitles;
	extern int		reward;
	extern int		penalty;
	extern string	db_type;
	extern double	min_id;
	extern unsigned	compress_temp;
	extern double	toppercent;
	extern string	daa_file;
	extern string	output_format;
	extern string	output_file;
	extern bool		forwardonly;
	extern unsigned fetch_size;
	extern bool		single_domain;
	extern size_t	db_size;

    typedef enum
    {
        fast = 0,
        sensitive = 1
    }AlignerMode;
    extern AlignerMode a_m;

    typedef enum
    {
        invalid = 0,
        createdb = 1,
        search = 4,
        view = 5
    } Command;

    extern Command cmd;

    inline uint32_t threads()
    {
        return std::max(threads_,1U);
    }
    template<typename _t>
    inline void setOption(_t& opt,_t value)
    {
        if(opt == 0)
        {
            opt = value;
        }
    }

    /**
     * Interface to receive paras from Cmd;
     * 
    */




template <typename _t>
void setOptions(double blk_size);

    template <typename _t>
    unsigned readPadding(size_t l);

    string getTempFile();

    bool memBuffered();


    inline unsigned getRunLen(unsigned l)
    {
        if(run_len == 0)
        {
            if(l < 30)
            {
                return 1;
            }
            else if(l < 100)
            {
                return 20;
            }
            else 
            {
                return 40;
            }
        }
        else
            return run_len;
    }

    inline bool outputRange(unsigned n_target_sequence, int score, int top_score)
    {
        if(toppercent < 100)
        {
            return (1.0 - (double)score / top_score) * 100 <= toppercent;
        }
        else
        {
            return n_target_sequence < max_alignments;
        }
    }


} // namespace VAToptions

#endif // __OPTIONS_H__