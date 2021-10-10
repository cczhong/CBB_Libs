#include "BinaryFile.h"

void copy_file(boost::iostreams::filtering_ostream &s, const char* file_name)
{
	s << file_name << endl;
	try {
		InputStreamController f (file_name);
		s << f.rdbuf() << endl;
	} catch (FileOpenException &e) {
	}
}