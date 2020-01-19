#pragma once
//C libraries
#include <sys/stat.h>
#include <dirent.h> 
#include <cstring>
#include <cstdlib>

//C++ libraries
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//Header files
#include "Repository.hpp"
#include "Exception.hpp"
#include "Snapshot.hpp"

/**
 * Class handles options given by user
 */
class COptions {
    int argc;                                       /**< Count of arguments */
    std::vector<std::string> argv;                  /**< Vector of arguments */ 
    std::map<std::string, CRepository> MStrRepos;   /**< Map of repositories sorted by names */   
    std::map<int, CRepository> MIntRepos;           /**< Map of repositories sorted by ids */

public:

    /**
     * Constructor
     * @param argc Number of arguments
     * @param argv Text of the arguments
     */
    COptions( int argc, char **argv );

    /**
     * Handles the input given by user
     */
    void ToDo() const;
    
    /**
     * Initializes path
     * @return string
     */
    std::string InitPath() const;

    /**
     * Initializes
     * @param tarpath Path
     */
    void Init( const std::string &tarpath ) const;

    /**
     * Prepares for backup
     * @param path path
     */
    void Repository( const std::string &path ) const;

    /**
     * Delete method
     */
    void Delete() const;

    /**
     * Shows possible options
     */
    void Show() const;

    /**
     * Shows help menu
     */
    void Help() const;

    /**
     * Finds repository
     * @return & to repository
     */
    const CRepository &FindRepo() const;

    /**
     * Opens repository
     * @param repo repository
     */
    void Open( const CRepository &repo ) const;
    
    /**
     * Diff function
     */
    void Diff();

    /**
     * Cron function
     */
    void SetTime( const std::string &line  ) const;

    /**
     * Loads repositories
     */
    void LoadRepos();

    /**
     * Makes backup
     * @param repo Repository
     */
    void BackUp( const CRepository &repo ) const;

    /**
     * New ID of repository
     * @return id
     */
    int NewRepoID() const;
};