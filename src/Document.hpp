#pragma once
//C++ libray
#include <string>
#include <sys/stat.h>
#include <memory>

//Header files
#include "Exception.hpp"
#include "Palette.hpp"

/**
 * Abstract class represents all kind of document
 */
class CDocument {
protected:
    std::string path; /**< path */
    std::string name; /**< name */

public:
    /**
     * Constructor
     * @param tpath Path
     */
    CDocument( const std::string &tpath );

    /**
     * Default constructor
     */
    CDocument() {};

    /**
     * Constructor
     * @param doc Document
     */
    CDocument( const CDocument &doc );

    /**
     * Virtual destructor
     */
    virtual ~CDocument() = default;

    /**
     * Pure virtual function determining whether file has changed
     * @param pathname Path
     */
    virtual bool IsDiff( const std::string &pathname ) const = 0;

    /**
     * Pure virtual function for copying files
     * @param dest Destination file
     */
    virtual void Copy( const std::string &dest ) const = 0;

    /**
     * Pure virtual diff function
     * @param doc pointer to the document
     * @param destpath Destination path
     */
    virtual void Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath ) = 0;

    /**
     * Pure virtual patch function
     * @param destpath Destination path
     */
    virtual void Patch( const std::string &destpath ) = 0;

    /**
     * Pure virtual delete method
     */
    virtual void Delete() const = 0;

    /**
     * Pure virtual print method
     */
    virtual void Print() const = 0;

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
};

/**
 * Custom comparator
 */
struct CDocCompare {
    /**
     * Functor for comparing
     * @param orig first document
     * @param back second document
     * @return true if first name is shorter than second
     */
    bool operator()( const std::shared_ptr<CDocument> &orig, const std::shared_ptr<CDocument> &back ) const {
        return orig->GetName().compare( back->GetName() ) < 0;
    }
};

/**
 * Strucf for comparing two documents
 */
struct SCompDocs {
    std::shared_ptr<CDocument> orig;    /**< Pointer to new document */
    char comp;                          /**< Character of compare */
    std::shared_ptr<CDocument> back;    /**< Pointer to reconstruction file */

    /**
     * Constructor
     * @param orig Original document
     * @param comp Compare char
     * @param back Backup document
     */
    SCompDocs( std::shared_ptr<CDocument> orig, char comp, std::shared_ptr<CDocument> back )
        : orig(orig), comp(comp), back(back) {};
};
