#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Options.h"
#include "SequenceType.h"

namespace VATOptions
{
    string		input_ref_file;
    uint32_t	threads_;
    string		database;
    string		query_file;
    uint32_t	merge_seq_treshold;
    uint32_t	block_size;
    uint32_t	hit_cap;
    int			min_ungapped_raw_score;
    uint32_t	shapes;
    uint32_t	index_mode;
    uint64_t	max_alignments;
    string		output_file;
    string		match_file1;
    string		match_file2;
    int			padding;
    uint32_t	output_threads;
    uint32_t	compression;
    uint32_t	lowmem;
    double		chunk_size;
    unsigned	min_identities;
    unsigned	min_identities2;
    int			xdrop;
    unsigned	window;
    int			min_hit_score;
    int			hit_band;
    unsigned	min_compressed_identities;
    int			min_seed_score;
    unsigned	seed_signatures;
    double		min_bit_score;
    unsigned	run_len;
    bool		alignment_traceback;
    double		max_seed_freq;
    string		tmpdir;
    bool		long_mode;
    int			gapped_xdrop;
    double		max_evalue;
    string		sam_output;
    string		kegg_file;
    int			gap_open;
    int			gap_extend;
    string		matrix;
    string		seg;
    bool		verbose;
    bool		bug_infor;
    bool		have_ssse3;
    bool		salltitles;
    int			reward;
    int			penalty;
    string		db_type;
    double		min_id;
    unsigned	compress_temp;
    double		toppercent;
    string		daa_file;
    string		output_format;
    bool		forwardonly;
    unsigned	fetch_size;
    bool		single_domain;
    size_t		db_size;

    AlignerMode a_m;
    Command cmd;

    template <typename _t>
    void setOptions(double blk_size)
    {
        if(a_m == sensitive)
        {
            setOption(seed_signatures,1u);
            setOption(index_mode,2u);
        }
        else if(a_m == fast)
        {
            setOption(seed_signatures,1u);
            setOption(index_mode,1u);
        }
        setOption(chunk_size,blk_size);
    }



    template <typename _t>
    unsigned readPadding(size_t l)
    {
        if(padding == 0)
        {
            if(l <= 255)
            {
                return 10;
            }
            else
            {
                return 32;
            }
        }
        else
        {
            return padding;
        }
    }
template<>
unsigned readPadding<Protein>(size_t len)
{
	if(padding == 0) {
		if(len<=35)
			return 5;
		else if(len<=55)
			return 16;
		else
			return 32;
	} else
		return padding;
}

    string getTempFile()
    {
        if(strlen(getenv("TMPDIR")) > 0)
        {
            return string(getenv("TMPDIR"))+"/vat.tmp";
        }
        else
        {
            std::cerr << "TMP env is bad";
            return output_file + ".tmp";
        }
    }

    bool memBuffered()
    {
        return tmpdir == "/dev/shm";
    }

    template void setOptions<Protein>(double blk_size);
    template void setOptions<Nucleotide>(double blk_size);

    template unsigned readPadding<Protein>(size_t l);
    template unsigned readPadding<Nucleotide>(size_t l);
}