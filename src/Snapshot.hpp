#pragma once
//C libraries
#include <stdio.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <cerrno>
#include <cstdint>
#include <cstring>

//C++ libraries
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <utility>
#include <string>
#include <sstream>

//Header files
#include "Exception.hpp"
#include "Document.hpp"
#include "Directory.hpp"
#include "TextFile.hpp"
#include "BinaryFile.hpp"
#include "Link.hpp"
#include "Table.hpp"
#include "Loader.hpp"
#include "Diff.hpp"
#include "Time.hpp"

/**
 * Class represents backup snapshots
 */
class CSnapshot {
    bool IsFull;                               /**< Time of modification */
    CTime modified;                            /**< Time of modification */
    unsigned int id;                           /**< ID */
    std::string name;                          /**< Name */
    std::string snappath;                      /**< Path to snapshot */
    std::set<CDirectory, SDirCompare> SDir;    /**< Set of directories */
    std::set<SCompDirs, SDiffCompare> SDiffs;  /**< Set of differences */

public:

    /**
     * Constructor
     * @param id ID
     * @param path Path
     * @param time Time
     * @param IsFull If is it complete backup or not
     */
    CSnapshot( unsigned int id, const std::string &path, const CTime &time, bool IsFull ) 
        : IsFull(IsFull),  modified(time), id(id), snappath(path) {};

    /**
     * Constructor
     * @param id ID
     * @param path Path
     * @param IsFull If is it complete backup or not
     */
    CSnapshot( unsigned int id, const std::string &path, bool IsFull );

    /**
     * Default constructor
     */
    CSnapshot() {};

    /**
     * Function handles load content of directory
     * @param path Path
     */
    void Load( const std::string &path );

    /**
     * Loads snapshot
     * @param repopath Repository path
     */
    void UpdateConfig( const std::string &repopath ) const;

    /**
     * Loads file into map for finding differences
     * @param path Path
     * @param tmp Map to fill
     */
    void LoadFile( const std::string &path, std::multimap<int, std::pair<char, std::string> > &tmp );

    /**
     * Patch function
     * @param next Next snapshot for merging differences
     */
    void Patch( const CSnapshot &next );

    /**
     * Method for patch save
     */
    void SavePatch() const;

    /**
     * Updates reconstruction directory
     * @param SDiffs Set of differences
     */
    void UpdateRecon( const std::set<SCompDirs, SDiffCompare> &SDiffs ) const;

    /**
     * Method for incremental backup
     * @param recon Reconstruction directory
     */
    void Backup( const CSnapshot &recon );

    /**
     * Prints differents between backups
     * @param sec Another snapshot for compare
     */
    void PrintDiff( const CSnapshot &sec ) const;

    /**
     * Compares two backups
     * @return true if they different, else false
     */
    bool Compare( const CSnapshot &s1 );

    /**
     * Method for complete backup
     * @param origpath Original path
     */
    void FullBackup( const std::string &origpath );

    /**
     * Compares two backups by time of created
     * @param snap Snapshot
     */
    bool SnapCompare( const CSnapshot &snap ) const;

    /**
     * Print the whole backup
     */  
    void Print() const;

    /**
     * Method for reconstruction backups
     * @param last The last snapshot to need reconstruction
     */  
    void Reconstruction( const CSnapshot &last );

    /**
     * Getter for set of directories
     * @return set
     */  
    std::set<CDirectory, SDirCompare> GetSet() const { return SDir; };

    /**
     * Getter for id
     * @return id
     */  
    unsigned int GetID() const { return id; };

    /**
     * Getter for modification
     * @return time of modification
     */  
    CTime GetModified() const { return modified; };

    /**
     * Getter for snapshot path
     * @return path
     */  
    std::string GetPath() const { return snappath; };

    /**
     * Getter for name
     * @return name
     */  
    std::string GetName() const { return name; };

    /**
     * Getter for boolean
     * @return true if is it complete backup, otherwise not
     */  
    bool GetFull() const { return IsFull; };
};
