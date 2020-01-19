#pragma once
//C library
#include <dirent.h>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <math.h>

//C++ library
#include <chrono>
#include <iostream>
#include <set>
#include <string>

//Header files
#include "Exception.hpp"
#include "Snapshot.hpp"
#include "Time.hpp"
#include "Table.hpp"
#include "Loader.hpp"

/**
 * Class represents backup repository
 */
class CRepository {
    unsigned int id;
    std::string pathname;                                  /**< The path of repository */
    std::string originpath;                                /**< The data path of repository */
    std::map<CTime, CSnapshot, STimeCompare> MBackups;     /**< The map of backups sorted by created */
    std::map<int, CSnapshot> MIDs;                         /**< The map of backups sorted by ids */

public:

    /**
     * Constructor
     * @param id ID
     * @param pathname Path
     * @param originpath Original path
     */
    CRepository( unsigned int id, const std::string &pathname, const std::string &originpath ) 
        : id(id), pathname(pathname), originpath(originpath) {};

    /**
     * Default constructor
     */
    CRepository() {};

    /**
     * Delete method
     */
    void Delete();

    /**
     * Restore method
     */
    void Restore( unsigned int id ) const;

    /**
     * Update method
     */
    void Update() const;

    /**
     * Makes full backup
     */
    void FullBackup() const;

    /**
     * Cron function for settings specific time for automatic backup
     */
    void SetTime( const std::string &line ) const;

    /**
     * Loads snapshots
     */
    void LoadSnaps();

    /**
     * Shows backups
     */
    void ShowBackups() const;
    
    /**
     * Compares the two backups
     * @param id1 ID of the first snapshot
     * @param id2 ID of the second snapshot
     */
    void BackupCompare( unsigned int id1, unsigned int id2 ) const;
    
    /**
     * Finds snapshot by id
     * @param name Name
     * @param pathname Path
     * @return snapshot
     */
    CSnapshot FindID( const std::string name, const std::string &pathname ) const;

    /**
     * Creates new id
     * @return new id
     */
    int NewID() const;

    /**
     * Gets the current timestamp
     * @return CTime with the time
     */
    CTime GetTimestamp() const;

    /**
     * Getter for path
     * @return pathname path
     */
    std::string GetPathname() const { return pathname; };

    /**
     * Getter for original path
     * @return originpath original path
     */
    std::string GetOrigpath() const { return originpath; };

    /**
     * Getter for id
     * @return id id
     */
    int GetID() const { return id; };
};
