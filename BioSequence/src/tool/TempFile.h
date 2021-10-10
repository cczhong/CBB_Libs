#ifndef __TEMP_FILE_H__
#define __TEMP_FILE_H__
#include "../paras/Options.h"


class TempFile
{
    public:
    TempFile():file_des_ (-1)
    {
        char *name = new char[VATOptions::tmpdir.length()+20];
        sprintf(name,"%s/VAT-tmp-XXXXXX",VATOptions::tmpdir.c_str());
        file_des_ = mkstemp(name);
        if(file_des_ == -1)
			throw std::runtime_error("Error opening temporary file.");
        
        unlink(name);
        delete[] name;
    }

    int file_descriptor() const
    {
        return file_des_;
    }

    private:
    /* data */
        int file_des_;
};

#endif // __TEMP_FILE_H__