// Author: Sirisha Thippabhotla
// Last modified: 09/12/22

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "MinimizerIndex.h"
#include "../Loader.h"
#include "../UtilFunctions.h"
#include "../../BioSequence/BioSequences.h"

using namespace std;


static string workspace_dir = "/home/tsirisha/Projects/Project2/results";
static string seq_file;
static string seq_type;
static string seq_role;
static string index_type;
static int kmer;
static int window;
static int num_blocks;
static int num_threads;

int main(int argc, char** argv)
{
  boost::program_options::options_description desc("Options for Minimizer indexing");
  desc.add_options()
      ("help", "print the help message")
      ("seq_type", boost::program_options::value<string>(&seq_type), "[required] type of sequence (nucl/prot)")
      ("seq_file", boost::program_options::value<string>(&seq_file), "[required] sequencing reads (in FASTQ/FASTA)")
      ("seq_role", boost::program_options::value<string>(&seq_role), "[required] 'query' if query sequence, 'ref' if reference sequence")
      ("index_type", boost::program_options::value<string>(&index_type)->default_value("mh"), "[optional] mh for minimizer" )
      ("kmer_size", boost::program_options::value<string>(&kmer)->default_value(15), "[optional] kmer size" )
      ("window_size", boost::program_options::value<string>(&window)->default_value(10), "[optional] window size" )
      ("out_dir", boost::program_options::value<string>(&workspace_dir)->default_value(workspace_dir), "[optional] output directory for indexing file dump")
      ("num_blocks", boost::program_options::value<int>(&num_blocks)->default_value(1), "[optional] num. of index blocks (should match the number of threads to be used during search)")
      ("num_threads", boost::program_options::value<int>(&num_threads)->default_value(1), "[optional] num. of threads to use")
  ;

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm
  );


boost::program_options::notify(vm);
    // print help message 
    if(vm.count("help"))  {
      cout << endl;
      cout << "Usage: ./seq_index [--seq_type (\"nucl\" or \"prot\")]  [--seq_file (FASTQ/A)] [--seq_role (\"query\" or \"ref\")]" << endl << endl;
      cout << desc << endl;
      return 0;
    }
    // check options validity
    if(seq_type != "nucl" && seq_type != "prot")  {
      cout << "Error: The specified seq_type is not supported (currently we only support \"nucl\" and \"prot\"):\t" << seq_type << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }
    if(seq_role != "ref" && seq_role != "query")  {
    cout << "Error: seq_role should only be set to either \"query\" or \"ref\")." << endl;
    cout << "Please use \'--help\' for more details." << endl;
    exit(0);
    }
    if(index_type != "mh"){
      cout<<"Error: index_type should only be set to mh (minhash)" <<endl;
      cout<<"Please use \'--help\' for more details." <<endl;
    }
    
    if(!boost::filesystem::exists(seq_file))  {
      cout << "Error: seq_file does not exist:\t" << seq_file << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }
    if(!boost::filesystem::is_directory(workspace_dir))  {
      boost::filesystem::create_directory(workspace_dir);
      cout <<workspace_dir <<"does not exist. Created working directory."<< endl;
    }
    boost::filesystem::path abs_workspace = workspace_dir;
    if(num_blocks <= 0)  {
      cout << "Error: num_blocks should be set to a positive value." << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }

  cout << "============================================================" << endl;
  cout << "Indexing input sequences.." << endl;
  double start_time;
  double current_time;
  BioAlphabet alphabet;


  if(seq_type == "nucl")  alphabet = BioAlphabet(DNA);
  else  alphabet = BioAlphabet(PROT);


  // load in sequence from file for forward sequences
  UtilFunc util;
  string seq_stem = util.GetFileStem(seq_file);

  start_time = util.MyTime();
  MinimizerIndex query_seq(alphabet);

  if(seq_role == "query"){
    start_time = util.MyTime();
    if(seq_type == "nucl"){
      query_seq.LoadSequences(seq_file, true);
    }
    else{
      query_seq.LoadSequences(seq_file, false);
    }

    vector<MinimizerIndex> block_seqs;
    block_seqs.resize(num_blocks);
    query_seq.SetBlockConfig(num_blocks, workspace_dir, seq_stem);
    query_seq.SplitSequence(block_seqs);
    #pragma omp parallel num_threads(num_threads)
    {
      #pragma omp for
      for(int i = 0; i < num_blocks; ++ i) {
        block_seqs[i].BuildMinimizerSketch(kmer, window);
        # pragma omp critical
        {
          std::string block_stem = seq_stem + "." + std::to_string(i);
          block_seqs[i].DumpSketch(workspace_dir, block_stem, 0);
        }
      }
    }

    }
    current_time = util.MyTime();
    util.PrintElapsed(start_time, current_time, "Minimizer: Block sketches constructed for query");
    cout << "Index has been writtin to [" << workspace_dir << "]" << endl;
    cout << "============================================================" << endl;
    

    return 0;
}


