#ifndef __VAT_EXCEPTIONS_H__
#define __VAT_EXCEPTIONS_H__




#include <string>
#include <boost/lexical_cast.hpp>
#include "../tool/TinyThread.h"

using std::exception;
using std::string;

#define THROW_EXCEPTION(exception, p1) throw exception(p1, __PRETTY_FUNCTION__, __LINE__)

struct VATException : public exception
{
	string msg;
	VATException(const char *function, unsigned line):
		msg(string("function ") + function + " line " + boost::lexical_cast<string>(line))
	{

	}
	VATException(const string &msg):
		msg (msg)
	{ 

	}
	~VATException() throw() { }
	virtual const char* what() const throw()
	{ 
		return msg.c_str(); 
	}
};

struct FileIOException : public VATException
{
	FileIOException(const string &file_name, const char* function, unsigned line):
		VATException(function, line)
	{ 
		msg += ". Error reading file " + file_name;
	}
};

struct FileOpenException: public VATException
{
	FileOpenException(const string &file_name, const char* function, unsigned line):
		VATException(function, line)
	{ 
		msg += ". Error opening file " + file_name;
	}
};

struct FileIOWriteException: public VATException
{
	FileIOWriteException(const string &file_name, const char* function, unsigned line):
		VATException(function, line)
	{ 
		msg += ". Error writing file " + file_name;
	}
};

struct MemAllocException: public exception
{
	virtual const char* what() const throw()
	{ 
		return "Failed to allocate memory";
	}
};

struct HashTableOverflowException : public exception
{
	virtual const char* what() const throw()
	{ 
		return "Hash table overflow";
	}
};

struct InvalidDBVersionException : public exception
{
	virtual const char* what() const throw()
	{ 
		return "Incompatible database version";
	}
};

struct InvalidParameterException : public exception
{
	virtual const char* what() const throw()
	{ 
		return "Invalid parameter";
	}
};

class InvalidSeqCharException : public exception
{
	public:

	const string msg;
	InvalidSeqCharException(char ch):
		msg (string("Invalid character (")+ch+"/" + boost::lexical_cast<string>(int(ch)) + ") in sequence")
	{ 

	}
	~InvalidSeqCharException() throw()
	{ 

	}
	virtual const char* what() const throw()
	{ 
		return msg.c_str(); 
	}
};

struct FileFormatException : public exception
{
	virtual const char* what() const throw()
	{ 
		return "Invalid input file format"; 
	}
};

class ExceptionState
{
	public:
	ExceptionState():
		active_ (false)
	{

	}
	void set(const std::exception &e)
	{
		if(!active_) {
			mtx_.lock();
			what_ = string(typeid(e).name()) + ": " + e.what();
			active_ = true;
			mtx_.unlock();
		}
	}
	void sync() const
	{
		if(active_)
			throw std::runtime_error(what_);
	}
	bool operator()() const
	{ return active_; }
private:
	tthread::mutex mtx_;
	bool active_;
	string what_;
};
//exception_state;

static ExceptionState exception_state;
#endif // __VAT_EXCEPTIONS_H__