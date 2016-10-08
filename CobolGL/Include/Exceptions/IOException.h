#pragma once

#include <exception>
#include <string>
class IOException : public std::exception {
public:
	IOException(std::string _msg) :  msg(_msg.c_str()) {
	}
protected:
	virtual const char * what() const throw() {
		return msg;
	}
	const char * msg;
};