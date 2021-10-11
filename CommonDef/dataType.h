#ifndef __DATATYPE_H_
#define __DATATYPE_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <assert.h>


//  This is the index for a large sequence database. 
//  We need to use unsigned 64bit because the database can be huge (e.g., a large metagenomic sequencing dataset). 
//  Note that using unsigned 32bit we can only handle ~4B letters, which is smaller than typical large databases.
//  Hence we are using 64bit here. 
typedef uint64_t DBIdxType;


//  This is the index for a single sequence. 
//  The worst-case scenario is that we handle a long chromosome, which is typically shorter than 4B letters. 
//  So, unsigned 32bit is fine. 
//  When handling cases where length of sequence is involved, such as sequence overlap, longest common prefix, alignment length etc., 
//  SeqIdxType should also be used.
typedef uint32_t SeqIdxType;  


//  This is the index for the read IDs. (Or other IDs if applicable.) 
//  Typically, an NGS dataset will contain less than 4B reads, although with many exceptions. 
//  Since we cannot handle that much data anyway, we will split the large dataset into smaller ones. 
//  Each small dataset should have <4B reads, which can be represented using unsigned 32bit.
typedef uint32_t IDType;




#endif      // __DATATYPE_H_