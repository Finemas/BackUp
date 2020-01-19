#pragma once
//C library
#include <sys/types.h>
#include <dirent.h> 
#include <cerrno>
#include <cstring>

//C++ library
#include <set>
#include <iostream>
#include <queue>
#include <utility>
#include <string>
#include <iomanip>

//Header files
#include "Exception.hpp"
#include "Directory.hpp"
#include "TextFile.hpp"
#include "BinaryFile.hpp"
#include "Link.hpp"

/**
 * Class for recursive loading directories
 */
class CLoader {
    std::string source;                      /**< Source */
    std::queue<std::string> QDir;            /**< Queue of directories */
    std::set<CDirectory, SDirCompare> SDir;  /**< set of directories */

    /**
     * Reads directory
     * @param dir dir
     * @param poped poped
     * @param name name
     * @param depth depth
     */
    void ReadDir( DIR * dir, const std::string &poped, const std::string &name, int &depth );

    /**
     * Iterates through the Queue
     */
    void ItirateQueue();

public:

    /**
     * Constructor
     * @param source Source
     */
    CLoader( const std::string &source ) : source(source) {};

    /**
     * Checks if is hardlink
     * @return true if yes, else false
     */
    bool IsHardlink( const std::string &pathname ) const;

    /**
     * Checks if is text file
     * @return true if yes, else false
     */
    bool IsText( const std::string &pathname ) const;
    
    /**
     * Load all files in the folder
     * @return set of directories and his files
     */
    std::set<CDirectory, SDirCompare> LoadDocs();
};
