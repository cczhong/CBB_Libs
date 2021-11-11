C      = g++
CFLAGS   = -std=c++11 -Wall -Wno-deprecated -fopenmp
OPT      = -O2

LIBPATH = -L/usr/local/lib -L/usr/local/lib64
LIBS     = -lrt -lm -lz -lgomp -lpthread -lboost_iostreams -lboost_program_options -lboost_filesystem -lboost_system



OBJS    = main_index.o seq_index.o loader.o gsa.o divsufsort.o  bio_alphabet.o interval_array.o sfa.o sssort.o trsort.o
EXE     = ../bin/seq-index


.cc.o:
	$(C) $(CFLAGS) $(OPT) -c $<
	@echo

install:  seq-index

seq-index: $(OBJS)
	$(C) $(CFLAGS) $(OPT) -o $(EXE) $(OBJS) $(LIBPATH) $(LIBS)
	@echo

all: clean install

clean:
	rm -f ../bin/* *.o *core
