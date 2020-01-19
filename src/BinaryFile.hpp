#pragma once
//C libraries
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

//C++ libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

//Header file
#include "File.hpp"

/**
 *  Class represents a binary file
 *  - file which has twice more unprintable character than printable 
 */
class CBinaryFile : public CFile {
    std::multimap<int, uint8_t> MDiff;  /**< Map of difference sort by number of line and with value one byte */
    std::vector<uint8_t> VContent;      /**< Vector of bytes */
  
public:
    /**
	 * Constructor.
     * @param path The path of file.
	 */
    CBinaryFile( const std::string &path ) : CFile(path) {};

    /**
     * Copy constructor
     * bin Binary file
     */
    CBinaryFile( const CBinaryFile &bin ) : CFile(bin.GetPath()), MDiff(bin.GetMap()) {};

    /**
     * Prints the binary file stats, inherited method
     */
    virtual void Print() const;

    /**
     * Tries to match the state of the file before and now, inherited method
     * @param doc Document
     * @param destpath Path to the destination file
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath );

    /**
     * Tries to merge one document with document with differences, inherited method
     * @param destpath Path to the destination file
     */
    virtual void Patch( const std::string &destpath );

    /**
     * Load content of file
     */
    void LoadFile();

    /**
     * Load difference from file and stored into map
     * @param diffpath Path to the file
     */
    void LoadDiff( const std::string &diffpath );

    /**
     * Save differences between two files into file
     * @param target Target path
     */
    void SaveDiff( const std::string &target ) const;

    /**
     * Save patch from two files into file
     * @param target Target path
     */
    void SavePatch( const std::string &target ) const;

    /**
     * Getter for map of differences
     * @return multimap of bytes sorted by number of lines
     */
    std::multimap<int, uint8_t> GetMap() const { return MDiff; };
};
