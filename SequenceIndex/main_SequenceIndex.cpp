#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "SequenceIndex.h"
#include "Loader.h"
#include "UtilFunctions.h"
#include "BioAlphabet.h"

using namespace std;

static string workspace_dir = "/home/tsirisha/Projects/Project2/Sequence_Index/Git-SeqIndexModule/results";
static string seq_file;
static string seq_type;
static string seq_role;
static string ds_type;
static int num_blocks;
static int num_threads;


int main(int argc, char** argv)
{
  boost::program_options::options_description desc("Options for indexing");
  desc.add_options()
      ("help", "print the help message")
      ("seq_type", boost::program_options::value<string>(&seq_type), "[required] type of sequence (nucl/prot)")
      ("seq_file", boost::program_options::value<string>(&seq_file), "[required] sequencing reads (in FASTQ/FASTA)")
      ("seq_role", boost::program_options::value<string>(&seq_role), "[required] Q if query sequence, R if reference sequence")
      ("ds_type", boost::program_options::value<string>(&ds_type)->default_value("SA"), "[optional] SA for Suffix Array, BWT for BWT/FM-Index" )
      ("work_space", boost::program_options::value<string>(&workspace_dir)->default_value(workspace_dir), "[optional] working directory for indexing file dump")
      ("num_blocks", boost::program_options::value<int>(&num_blocks)->default_value(64), "[optional] num. of index blocks (should match the number of threads to be used during search)")
      ("num_threads", boost::program_options::value<int>(&num_threads)->default_value(8), "[optional] num. of threads to use")
  ;

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm
  );

  boost::program_options::notify(vm);
    if(vm.count("help"))  {
      cout << endl;
      cout << "Usage: ./seq-index [seq_type (\"nucl\" or \"prot\")]  [seq_file (FASTQ/A)] [seq_role (\"Q\" or \"R\")]" << endl << endl;
      cout << desc << endl;
      return 0;
    }
    // check options validity
    if(seq_type != "nucl" && seq_type != "prot")  {
      cout << "Error: The specified seq_type is not supported (currently we only support \"nucl\" and \"prot\"):\t" << seq_type << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }
    if(seq_role != "R" && seq_role != "Q")  {
    cout << "Error: seq_role should only be set to either Q (query) or R (reference)." << endl;
    cout << "Please use \'--help\' for more details." << endl;
    exit(0);
    }
    if(ds_type != "SA" && ds_type != "BWT"){
      cout<<"Error: ds_type should only be set to either SA (suffix array) or BWT" <<endl;
      cout<<"Please use \'--help\' for more details." <<endl;
    }
    boost::filesystem::path abs_workspace = workspace_dir;
    if(!boost::filesystem::exists(seq_file))  {
      cout << "Error: seq_file does not exist:\t" << seq_file << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }
    if(!boost::filesystem::is_directory(workspace_dir))  {
      cout << workspace_dir << endl;
      cout << "Error: working space does not exist (please provide full path)." << endl;
      cout << "Please use \'--help\' for more details." << endl;
      exit(0);
    }
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
  /** checking the type and validity of the input sequence will be done by BioSequence class **/
  if(seq_type == "nucl")  alphabet = BioAlphabet(DNA);
  else alphabet = BioAlphabet(PROT);
  // load in sequence from file for forward sequences
  UtilFunc util;
  string seq_stem = util.GetFileStem(seq_file);

  start_time = util.MyTime();
  SequenceIndex query_seq(alphabet);

  if(seq_role == "Q"){
    start_time = util.MyTime();
    if(seq_type == "nucl"){
      query_seq.LoadSequences(seq_file, true);
    }
    else{
      query_seq.LoadSequences(seq_file, false);
    }

    vector<SequenceIndex> block_seqs;
    block_seqs.resize(num_blocks);
    query_seq.SetBlockConfig(num_blocks, workspace_dir, seq_stem);
  /*  query_seq.SplitSequence(block_seqs);
    #pragma omp parallel num_threads(num_threads)
    {
      #pragma omp for
      for(int i = 0; i < num_blocks; ++ i) {
        block_seqs[i].BuildSFADefault();
        # pragma omp critical
        {
          std::string block_stem = seq_stem + "." + std::to_string(i);
          block_seqs[i].DumpSFA(workspace_dir, block_stem, 0);
        }
      }
    }

    current_time = util.MyTime();
    util.PrintElapsed(start_time, current_time, "SequenceIndex: Block suffix arrays constructed for query");
    cout << "Index has been writtin to [" << workspace_dir << "]" << endl;
    cout << "============================================================" << endl;
    */
  }
  /*else if(seq_role == 'R'){
    start_time = util.MyTime();
    if(seq_type == "nucl")  {
      query_seq.LoadSequences(seq_file, true);
    } else  {
      query_seq.LoadSequences(seq_file, false);
    }
    current_time = util.MyTime();
    util.PrintElapsed(start_time, current_time, "Reference Sequence loaded.");
    query_seq.BuildSFADefault();
    query_seq.DumpSFA(workspace_dir, db_stem, 0);
    current_time = util.MyTime();
    util.PrintElapsed(start_time, current_time, "SequenceIndex: Suffix array constructed for reference");
  }*/

return 0;
}
