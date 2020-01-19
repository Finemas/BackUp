#include "Link.hpp"
using namespace std;

CLink::CLink( const std::string &path, bool IsSymlink ) 
        : CDocument(path), IsSymlink(IsSymlink) {
            hash = Hashfile( path );
}  

bool CLink::IsDiff( const string &pathname ) const {
    if( hash.compare( Hashfile( pathname ) ) != 0 ) 
        return true;
    return false;
}

void CLink::Print() const {
    if ( IsSymlink  ) 
        cout << "  S " << name << ' ' << hex << hash;       
    else 
        cout << "  H " << name << ' ' << hex << hash;
}

void CLink::Copy( const string &dest ) const {
    string target = dest + '/' + name;
    if( !IsSymlink ) {
        if( link( target.c_str(), dest.c_str() ) != 0 )
            throw CExceptions("In function CLink::Copy() couldn't create the hardlink: " + (string)strerror(errno),red);
        return;
    }
    
    char buf[512];
    int count = readlink( path.c_str(), buf, sizeof(buf) );
    if (count < 0) 
        throw CExceptions("In function CLink::Copy() couldn't read the symlink (" + path + ')',red);
    buf[count] = '\0';
    target = buf;

    if( symlink( target.c_str(), dest.c_str() ) != 0 )
        throw CExceptions("In function CLink::Copy() couldn't create the symlink: " + (string)strerror(errno),red);
}

void CLink::Delete() const {
    if( remove( path.c_str() ) != 0 )
         throw CExceptions("In function CLink::Delete() couldn't remove the link: " + (string)strerror(errno),red);
}

/***************************************************
 * Author: Alex Belanger
 * Date: Nov 12, 2015
 * Availability: https://gist.github.com/nitrix/34196ff0c93fdfb01d51
 ***************************************************/
string CLink::Hashfile(const string &pathname) const {
    ifstream fp(pathname);
    stringstream ss;

    if (!fp.is_open())
        throw CExceptions("In function CLink::Hashfile unable to get hash of the file.",red);

    // Hashing
    uint32_t magic = 5381;
    char c;
    while (fp.get(c)) {
        magic = ((magic << 5) + magic) + c;  // magic * 33 + c
    }

    return ss.str();
}

void CLink::Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath ) {
    shared_ptr<CLink> link = dynamic_pointer_cast<CLink>(doc);

    string tarBack, tarOrig;
    char buf[512];
    int count = readlink( path.c_str(), buf, sizeof(buf) );
    if (count < 0) 
        throw CExceptions("In function CLink::Copy() couldn't read the symlink (" + path + ')',red);
    buf[count] = '\0';
    tarBack = buf;

    count = readlink( path.c_str(), buf, sizeof(buf) );
    if (count < 0) 
        throw CExceptions("In function CLink::Copy() couldn't read the symlink (" + path + ')',red);
    buf[count] = '\0';
    tarBack = buf;

    if( tarBack.compare(tarOrig) == 0 )
        return;
        
    Copy( destpath );
}

void CLink::Patch( const std::string &destpath ) {
    Copy( destpath );
}