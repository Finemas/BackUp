#pragma once
//C++ library
#include <exception>
#include <string>
#include <map>

/**
 * Class represents all exceptions and inherits system class exception and class of colors
 */
class CExceptions : public std::exception {
  	std::string Error;                  /**< String that gives different statement */
    const std::string nc = "\033[0m";   /**< Default color */
 
public:
    /**
	 * Constructor.
     * @param error An error statement
     * @param color An color
	 */
	CExceptions( const std::string &error, const std::string &color ) {
        Error = color + error + nc;
    };

    /**
	 * Returning an error statement.
     * @return return string of statement;
	 */
    virtual const char* what() const throw() {
        return Error.c_str();
    }
};
