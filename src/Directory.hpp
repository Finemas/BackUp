#pragma once
//C libraries
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <dirent.h>

//C++ libraries
#include <string>
#include <set>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

//Header files
#include "Document.hpp"
#include "Time.hpp"

/**
 * class represents directory
 */
class CDirectory : public CDocument {
    int depth;                                                  /**< Depth of the search */
    CTime modified;                                             /**< Time of modification */
    std::string depname;                                        /**< Name relative to the depth */
    std::set<std::shared_ptr<CDocument>, CDocCompare> SDoc;     /**< Set of subfiles */

public:

    /**
     * Constructor
     * @param path path to the file
     * @param depname Name relative to the depth
     * @param depth Depth of the search
     */
    CDirectory( const std::string &path, const std::string &depname, int depth );

    /**
     * Default constructor
     */
    CDirectory() {};

    /**
     * Copy constructor
     * @param dir Directory
     */
    CDirectory( const CDirectory &dir );

    /**
     * Checks if directory is different from the directory with given path
     * @param pathname Path to the file
     * @return true if it has changed, false otherwise
     */
    virtual bool IsDiff( const std::string &pathname ) const;

    /**
     * Copies a directory
     * @param dest Destionation file
     */
    virtual void Copy( const std::string &dest ) const; 

    /**
     * Diff function for directories
     * @param doc Pointer of the second documentsmk
     * @param destpath Path to the destination file 
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath );

    /**
     * Renames directory if find difference, inherited method
     * @param destpath Destination path
     */
    virtual void Patch( const std::string &destpath );

    /**
     * Prints the name of the directory and all subfiles
     */
    virtual void Print() const;

    /**
     * Deletes the content of directory
     */
    virtual void Delete() const;

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
     * Prints directory name
     */
    void PrintDir() const;
    
    /**
     * Getter for depth
     * @return depth
     */
    int GetDepth() const { return depth; };

    /**
     * Getter for time of modification
     * @return modified
     */
    CTime GetModified() const { return modified; };

    /**
     * Getter depname
     * @return depname
     */
    std::string GetDepname() const { return depname; };

    /**
     * Getter for set
     * @return SDoc set of documents
     */
    auto GetSet() const { return SDoc; };

    /**
     * Loads content of file
     */
    void LoadFiles();

    /**
     * Loads subfiles of the directory
     * @par path Path of second directory
     * @par SDoc Set of file names
     */
    void LoadDir( const std::string &path, std::set<std::string> &SDoc ) const;

    /**
     * Finding file partner prepared for size compare
     * @param path Path to the second file
     */
    bool SizeCompare( const std::string &path ) const;

    /**
     * Compares two files by file size
     * @par file1 Path of the first file
     * @par file2 Path of the second file
     * @return true if they are different, otherwise false
     */
    bool IsSizeDiff( const std::string &file1, const std::string &file2 ) const;

    /*
	 * template method for inserting CDomunect into vector
     * @param[in] TYPE Template for link, file or directory.
	 */
    template<typename TYPE>
    void AddDocument( const TYPE &doc );
};

template<typename TYPE>
void CDirectory::AddDocument( const TYPE &doc ) {
    SDoc.emplace( std::make_shared<TYPE>( doc ) );
}

/**
 * Struct wiht compare function for two directories by depth and name
 */
struct SDirCompare {
    /**
     * Functor for comparing
     * @param dir1 First directory
     * @param dir2 Second directory
     * @return true if depth of first directory is less or name
     */
    bool operator()( const CDirectory &dir1, const CDirectory &dir2 ) const { 
        if( dir1.GetDepth() == dir2.GetDepth() ) 
            return ( dir1.GetName().compare( dir2.GetName() ) < 0 ) ? true : false;
        return dir1.GetDepth() < dir2.GetDepth();
    }
};

/**
 * Struct for comparing two direct
 */
struct SCompDirs {
    std::shared_ptr<CDirectory> orig;         /**< Pointer to original directory which might change */
    char comp;                                /**< Character of compare */
    std::shared_ptr<CDirectory> back;         /**< Pointer to reconstruction directory */
    std::vector<SCompDocs> VDoc;              /**< Vector of subfiles */

    /**
     * Constructor
     * @param orig First directory
     * @param comp Charater of compare
     * @param back Second directory
     */
    SCompDirs( const CDirectory &orig, char comp, const CDirectory &back )
        : orig(std::make_shared<CDirectory>(orig)), comp(comp), back(std::make_shared<CDirectory>(back)) {};

    /**
     * Copy constructor
     * @param cp Struct of comparing two directories
     */
    SCompDirs( const SCompDirs &cp ) {
        orig = cp.orig;
        comp = cp.comp;
        back = cp.back;

        for( auto it : cp.VDoc )
            VDoc.emplace_back( it );
    };

    /**
     * Add document into vector of documents
     * @param doc1 First document
     * @param comp Charater of compare
     * @param doc2 Second document
     */
    void AddCompDoc( const std::shared_ptr<CDocument> doc1, char comp, const std::shared_ptr<CDocument> doc2  ) {
        VDoc.emplace_back( SCompDocs( doc1, comp, doc2 ) );
    };
};

/**
 * Struct for comparing two directories 
 */
struct SDiffCompare {
    /**
     * Functor for comparing by depth and name
     * @param cp1 First struct of two directories
     * @param cp2 Second struct of two directories
     * @return true if depthm of the first struct or name is less
     */
    bool operator()( const SCompDirs &cp1, const SCompDirs &cp2 ) const {
        if( cp1.back->GetDepth() == cp2.back->GetDepth() ) 
            return cp1.back->GetName().compare( cp2.back->GetName() ) < 0;
        return cp1.back->GetDepth() < cp2.back->GetDepth();
    }
};
