#ifndef __VATSETUP_H__
#define __VATSETUP_H__
#include <boost/iostreams/tee.hpp>
#include "../paras/Options.h"
#include "../tool/System.h"
#include "../tool/Utils.h"
#include <iostream>
#include <utility>      // std::pair, std::make_pair
#include "../tool/TinyThread.h"
#include "../paras/ScoreMatrix.h"

using std::cout;
using std::endl;
using std::pair;

void Setup(const string &command, int ac, const char **av)
{
	namespace io = boost::iostreams;
	namespace vo = VATOptions;

  //  cout << "Setup" << endl;
    AppendExtension(vo::database, ".vatdb");
    AppendExtension(vo::daa_file, ".vout");

	vo::setOption(vo::threads_, tthread::thread::hardware_concurrency());

    if ("createdb" == command)
    {
        vo::cmd = vo::createdb;
    } 
    else if("search" == command)
    {
        vo::cmd = vo::search;
    }
    else if(command== "view")
    {
        vo::cmd == vo::view;
        //cout << "view" << endl;
    }
    else
    {
        vo::cmd == vo::invalid;
    }
    vo::have_ssse3 = check_SSSE3();

    if (vo::have_ssse3)
    {
        cout << "sse3 is avaiable" << endl;
    }

		if(vo::gap_open == -1)
			vo::gap_open = 11;
		if(vo::gap_extend == -1)
			vo::gap_extend = 1;
		ScoreMatrix::instance = auto_ptr<ScoreMatrix> (new ScoreMatrix(vo::matrix,
				vo::gap_open,
				vo::gap_extend,
				vo::reward,
				vo::penalty,
				Protein ()));
		ScoreMatrix::get().print<Protein>();
	

}
// void SetSearchParams(pair<size_t, size_t> query_len_bounds, size_t chunk_db_letters);
void SetSearchParams(pair<size_t, size_t> query_len_bounds, size_t chunk_db_letters)
{
    namespace po = VATOptions;

	po::setOption(po::hit_cap, 32u);
	po::setOption(po::min_identities, 18u);
	po::min_ungapped_raw_score = 19;
	po::setOption(po::window, 40u);
	po::setOption(po::hit_band, 5);
	po::min_hit_score = 19;
  //  cout << "SetSearchParams" << endl;

}



#endif // __VATSETUP_H__