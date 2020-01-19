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
 * Class represents a text file 
 *  - file which has twice more printable character than unprintable 
 */
class CTextFile : public CFile {
    std::multimap<int, std::pair<char, std::string> > MDiff;    /**< Map of differences sorted by number of line with value pair of compare character and different text */
    std::vector<std::string> VContent;                          /**< Vector of file content */
  
public:

    /**
     * Constructor
     * @param path Path
     */
    CTextFile( const std::string &path ) : CFile(path) {};


    /**
     * Copy constructor
     * @param text Another text file
     */
    CTextFile( const CTextFile &text ) : CFile(text.GetPath()), MDiff(text.GetMap()) {};

    /**
     * Virtual method for print
     */
    virtual void Print() const;

    /**
     * Tries to match the state of the file before and now, inherited method
     * @param doc Document
     * @param destpath Path to the destination file
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath );

    /**
     * Tries to merge one document with another document with differences, inherited method
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
     * Getter for multimap of differences
     * @return multimap of different strings sorted by number of lines
     */
    std::multimap<int, std::pair<char, std::string> > GetMap() const { return MDiff; };
};
