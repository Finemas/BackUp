#pragma once
//C++ libraries
#include <set>
#include <map>
#include <vector>
#include <utility>

//Header files
#include "Document.hpp"
#include "Directory.hpp"

/**
 * This class handles the difference of backups
 */
class CBackDiff {
    std::set<CDirectory, SDirCompare> SOrig;    /**< Source directories to compare */
    std::set<CDirectory, SDirCompare> SBack;    /**< Second set of directories to compare */
    
    std::set<SCompDirs, SDiffCompare> SDiffs;   /**< The finish set of differences */

public:

    /**
     * Constructor
     * @param SOrig Original set of directories
     * @param SBack Backup set of directories
     */
    CBackDiff( const std::set<CDirectory, SDirCompare> &SOrig, const std::set<CDirectory, SDirCompare> &SBack ) 
        : SOrig(SOrig), SBack(SBack) {};

    /**
     * Check if folders are different
     * @return true if they are different, otherwise false
     */
    bool IsDiff();

    /**
     * Handles the difference of files
     * @return set of differences
     */
    std::set<SCompDirs, SDiffCompare> GetDiffs() const { return SDiffs; };

    /**
     * Compares directories
     */
    void DirCompare();

    /**
     * Compares documents
     * @param dir1 First directory
     * @param dir2 Second directory
     */
    void DocCompare( const CDirectory &dir1, const CDirectory &dir2 );
};