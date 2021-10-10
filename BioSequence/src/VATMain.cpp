#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "paras/Options.h"
#include "sequence/Reference.h"
#include "makedb/DBMaker.h"
#include "workflow/AlignFlows.h"
#include "tool/VATSetup.h"
#include "./paras/SequenceType.h"
#include "output/Show.h"

using std::cout;
using std::endl;


int main(int ac, const char* av[])
{

    namespace po = boost::program_options;
    try {
            string command;
            po::options_description general("General Options");
            general.add_options()
                ("help,h", "produce help message")
                ("threads,p", po::value<uint32_t>(&VATOptions::threads_)->default_value(0), "number of cpu threads")
                ("db,outdb", po::value<string>(&VATOptions::database), "database file")
                ("vat,a", po::value<string>(&VATOptions::daa_file), "VAT alignment archive (VAT) file")
                //("out,o", po::value<string>(&VATOptions::output_file), "output file")
                ;
            po::options_description makedb("Makedb options");
            makedb.add_options()
                ("input", po::value<string>(&VATOptions::input_ref_file), "input reference file in FASTA format")
                ("block-size,b", po::value<double>(&VATOptions::chunk_size), "sequence block size in billions of letters (default=2)");
            ;
            po::options_description aligner("Aligner options");
            aligner.add_options()
                ("query,q", po::value<string>(&VATOptions::query_file), "input query file")
                ("max-target-seqs,k", po::value<uint64_t>(&VATOptions::max_alignments)->default_value(25), "maximum number of target sequences to report alignments for")
                ("top", po::value<double>(&VATOptions::toppercent)->default_value(100), "report alignments within this percentage range of top alignment score (overrides --max-target-seqs)")
                ("compress", po::value<unsigned>(&VATOptions::compression)->default_value(0), "compression for output files (0=none, 1=gzip)")
                ("evalue,e", po::value<double>(&VATOptions::max_evalue)->default_value(0.001), "maximum e-value to report alignments")
                ("min-score", po::value<double>(&VATOptions::min_bit_score)->default_value(0), "minimum bit score to report alignments (overrides e-value setting)")
                ("id", po::value<double>(&VATOptions::min_id)->default_value(0), "minimum identity% to report an alignment")
                ("sensitive", "enable sensitive mode (default: fast)")
                ("index-chunks,c", po::value<unsigned>(&VATOptions::lowmem)->default_value(4), "number of chunks for index processing")
                ("tmpdir,t", po::value<string>(&VATOptions::tmpdir)->default_value("/dev/shm"), "directory for temporary files")
                ("gapopen", po::value<int>(&VATOptions::gap_open)->default_value(-1), "gap open penalty, -1=default (11 for protein)")
                ("gapextend", po::value<int>(&VATOptions::gap_extend)->default_value(-1), "gap extension penalty, -1=default (1 for protein)")
                ("reward", po::value<int>(&VATOptions::reward)->default_value(2), "match reward score (blastn only)")
                ("penalty", po::value<int>(&VATOptions::penalty)->default_value(-3), "mismatch penalty score (blastn only)")
                ("matrix", po::value<string>(&VATOptions::matrix)->default_value("blosum62"), "score matrix for protein alignment")
                ("seg", po::value<string>(&VATOptions::seg), "enable SEG masking of queries (yes/no)");


            po::options_description advanced("Advanced options (0=auto)");
            advanced.add_options()
                ("seed-freq", po::value<double>(&VATOptions::max_seed_freq)->default_value(-15), "maximum seed frequency")
                ("run-len,l", po::value<unsigned>(&VATOptions::run_len)->default_value(0), "mask runs between stop codons shorter than this length")
                ("max-hits,C", po::value<unsigned>(&VATOptions::hit_cap)->default_value(0), "maximum number of hits to consider for one seed")
                ("id2", po::value<unsigned>(&VATOptions::min_identities)->default_value(0), "minimum number of identities for stage 1 hit")
                ("window,w", po::value<unsigned>(&VATOptions::window)->default_value(0), "window size for local hit search")
                ("xdrop", po::value<int>(&VATOptions::xdrop)->default_value(20), "xdrop for ungapped alignment")
                ("gapped-xdrop,X", po::value<int>(&VATOptions::gapped_xdrop)->default_value(20), "xdrop for gapped alignment in bits")
                ("ungapped-score", po::value<int>(&VATOptions::min_ungapped_raw_score)->default_value(0), "minimum raw alignment score to continue local extension")
                ("hit-band", po::value<int>(&VATOptions::hit_band)->default_value(0), "band for hit verification")
                ("hit-score", po::value<int>(&VATOptions::min_hit_score)->default_value(0), "minimum score to keep a tentative alignment")
                ("band", po::value<int>(&VATOptions::padding)->default_value(0), "band for dynamic programming computation")
                ("shapes,s", po::value<unsigned>(&VATOptions::shapes)->default_value(0), "number of seed shapes (0 = all available)")
                ("index-mode", po::value<unsigned>(&VATOptions::index_mode)->default_value(0), "index mode (1=4x12, 2=16x9)")
                ("fetch-size", po::value<unsigned>(&VATOptions::fetch_size)->default_value(4096), "trace point fetch size")
                ("single-domain", "Discard secondary domains within one target sequence")
                ("no-traceback,r", "disable alignment traceback")
                ("dbsize", po::value<size_t>(&VATOptions::db_size)->default_value(0), "effective database size (in letters)");
                //("compress-temp", po::value<unsigned>(&program_options::compress_temp)->default_value(0), "compression for temporary output files (0=none, 1=gzip)");

            po::options_description view_options("View options");
            view_options.add_options()
                ("out,o", po::value<string>(&VATOptions::output_file), "output file")
                ("outfmt,f", po::value<string>(&VATOptions::output_format)->default_value("tab"), "output format (tab/sam)")
                ("forwardonly", "only show alignments of forward strand");

            po::options_description hidden("Hidden options");
            hidden.add_options()("command", po::value<string>(&command));


            po::options_description cmd_line_options("Command line options");

            cmd_line_options.add(general).add(hidden).add(makedb).add(aligner).add(advanced).add(view_options);

            po::positional_options_description positional;
            positional.add("command", -1);

            po::variables_map vm;
            po::store(po::command_line_parser(ac, av).options(cmd_line_options).positional(positional).run(), vm);
            po::notify(vm);

            
            VATOptions::alignment_traceback = (vm.count("no-traceback") == 0);
            VATOptions::long_mode = vm.count("long") > 0;
            VATOptions::verbose = vm.count("verbose") > 0;
            //VATOptions::debug_log = vm.count("log") > 0;
            VATOptions::salltitles = vm.count("salltitles") > 0;
            VATOptions::forwardonly = vm.count("forwardonly") > 0;
            VATOptions::single_domain = vm.count("single-domain") > 0;
            Setup(command, ac, av);

            if (vm.count("help")) 
            {
                cout << endl << "Syntax:" << endl;
                cout << "  VAT COMMAND [OPTIONS]" << endl << endl;
                cout << "Commands:" << endl;
                cout << endl;
                cout << general << endl << makedb << endl << aligner << endl << view_options << endl;
            }
            else if (VATOptions::cmd == VATOptions::createdb && vm.count("input") && vm.count("db"))
            {
                    cout << "createdb" << endl;
                    if(vm.count("block-size") == 0)
                		VATOptions::chunk_size = 2;
                    DBMake dbm;
                    dbm.DBMakeConroller(Protein());
            }
            else if (VATOptions::cmd == VATOptions::search && vm.count("query") && vm.count("db") && vm.count("vat"))
            {                       
                cout << "search" << endl;
                WorkFlows wf;
                VATOptions::chunk_size = 0;
                wf.run(Protein());
            }
            //&& vm.count("vat") && vm.count("out") VATOptions::cmd
            else if(command == "view" && vm.count("vat"))
            {
            // cout << "ShowResults" << endl;

                // ShowResults();
            }
            else
            {
                cout << "Insufficient arguments. Use VAT -h for help.\n";
            }
        }
        catch(...) {
            //cout << "Exception of unknown type!\n";
            return 1;
        }

    return 0;
}
