

#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include "../basic/options.h"
#include "../util/log_stream.h"
#include "../data/reference.h"
#include "../run/make_db.h"
#include "../run/master_thread.h"
#include "../util/complexity_filter.h"
#include "../basic/setup.h"


using std::cout;
using std::cerr;
using std::endl;

int main(int ac, const char* av[])
{

	namespace po = boost::program_options;

	try {

		string command;

        po::options_description general("General options");
        general.add_options()
            ("help,h", "produce help message")
            ("threads,p", po::value<uint32_t>(&program_options::threads_)->default_value(2), "number of cpu threads")
            ("db,d", po::value<string>(&program_options::database), "database file") 
            ;

        po::options_description makedb("Makedb options");
        makedb.add_options()
        	("in", po::value<string>(&program_options::input_ref_file), "input reference file in FASTA format")
        	("block-size,b", po::value<double>(&program_options::chunk_size), "sequence block size in billions of letters (default=2)")
        	;

        po::options_description aligner("Aligner options");
        aligner.add_options()
        	("gapopen", po::value<int>(&program_options::gap_open)->default_value(-1), "gap open penalty, -1=default (11 for protein)")
        	("gapextend", po::value<int>(&program_options::gap_extend)->default_value(-1), "gap extension penalty, -1=default (1 for protein)")
        	("matrix", po::value<string>(&program_options::matrix)->default_value("blosum62"), "score matrix for protein alignment")
            ;

        po::options_description hidden("Hidden options");
        hidden.add_options()
        	("command", po::value<string>(&command));

        po::options_description cmd_line_options("Command line options");
        cmd_line_options.add(general).add(hidden).add(makedb).add(aligner);

        po::positional_options_description positional;
        positional.add("command", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).options(cmd_line_options).positional(positional).run(), vm);
        po::notify(vm);

        setup(command, ac, av);
        if (program_options::command == program_options::makedb && vm.count("in") && vm.count("db")) {
        	if(vm.count("block-size") == 0)
        		program_options::chunk_size = 2;
        		//make_db(AminoAcid());
            make_db(Nucleotide());
        }
    }


       // make_db(AminoAcid());
    //}
    catch(...) {
        cerr << "Exception of unknown type!\n";
        return 1;
    }

    return 0;
}
