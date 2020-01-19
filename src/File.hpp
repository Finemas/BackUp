#pragma once
//C library
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <stdio.h>

//C++ library
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Header files
#include "Document.hpp"

/**
 * Class represents regular file
 */
class CFile : public CDocument {
protected:
    int size;   /**< size */
  
public:

    /**
     * Constructor
     * @param path Path
     * @param size Size
     */
    CFile( const std::string &path, int size ) : CDocument(path), size(size) {};

    /**
     * Constructor
     * @param path Path
     */
    CFile( const std::string &path );

    /**
     * Virtual destructor
     */
    virtual ~CFile() = default;

    /**
     * Copy constructor
     * @param file Another file
     */
    CFile( const CFile &file );

    /**
     * Method checking if files has diffent size
     * @param pathname Path
     * @return true if changed, false otherwise
     */
    virtual bool IsDiff( const std::string &pathname ) const;

    /**
     * Virtual method for copying files byte by byte
     * @param dest Destination file
     */
    virtual void Copy( const std::string &dest ) const;

    /**
     * Delete function
     */
    virtual void Delete() const;

    /**
     * Pure virtual Print method
     */
    virtual void Print() const = 0;

    /**
     * Pure virtual diff function
     * @param doc pointer to files
     * @param destpath Destination path
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath ) = 0;

    /**
     * Pure virtual diff function
     * @param diffpath Path of file with differences
     */
    virtual void Patch( const std::string &diffpath ) = 0;

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
     * Getter for size
     * @return size
     */
    int GetSize() const { return size; };
};