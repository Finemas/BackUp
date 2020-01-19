#pragma once
//C library
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdio.h>

//C++ library
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>

//Header file
#include "Document.hpp"

/**
 * Class represents links
 */
class CLink : public CDocument {    
    std::string hash;       /**< hash of file */
    std::string target;     /**< target of link */
    bool IsSymlink;         /**< If is symlink */

public:
    /**
     * Constructor
     * @par path Path
     * @par IsSymlink Bool if is it symlink or not
     */
    CLink( const std::string &path, bool IsSymlink );

    /**
     * Copy constructor
     * @par link Another link
     */
    CLink( const CLink &link ) 
        : CDocument(link.GetPath()), hash(link.GetHash()), target(link.GetTarget()), IsSymlink(link.GetIsSymlink()) {};

    /**
     * Method checking if links has different hash
     * @param pathname Path
     * @return true if changed, false otherwise
     */
    virtual bool IsDiff( const std::string &pathname ) const;

    /**
     * Pure virtual method for copying files
     * @param dest Destination file
     */
    virtual void Copy( const std::string &dest ) const;

    /**
     * Delete function
     */
    virtual void Delete() const;

    /**
     * Virtual Print method
     */
    virtual void Print() const;

    /**
     * Virtual diff function
     * @param doc pointer to files
     * @param destpath Destination path
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath );

    /**
     * Renames link if find difference, inherited method
     * @param destpath Destination path
     */
    virtual void Patch( const std::string &destpath );

    /**
     * Getter for path
     * @return path
     */
    virtual std::string GetPath() const { return path; };

    /**
     * Getter for name
     * @return name
     */
    virtual std::string GetName() const { return name; };

    /**
     * Getter for hash
     * @return hash
     */
    std::string GetHash() const { return hash; };

    /**
     * Getter for target
     * @return target of link
     */
    std::string GetTarget() const { return target; };

    /**
     * Getter for boolean if is it symlink or not
     * @return bool IsSymlink
     */
    bool GetIsSymlink() const { return IsSymlink; };

    /*
	 * make hash
     * @param[in] pathname - the path of another document.
     * @return true if documents are different vice versa false when not
	 */
    std::string Hashfile( const std::string &pathname ) const;
};
