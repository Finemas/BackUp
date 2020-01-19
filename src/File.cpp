#include "File.hpp"
using namespace std;

CFile::CFile( const std::string &path ) : CDocument(path) {
    struct stat info;
    if( stat(path.c_str(), &info) )
        throw CExceptions("In constructor CFile couldn't get a stats.",red);

    size = info.st_size;
}

CFile::CFile( const CFile &file ) : CDocument(file.GetPath()) {
    size = file.GetSize();
}

bool CFile::IsDiff( const string &pathname ) const {
    struct stat file;

    if (stat(pathname.c_str(), &file) == -1)
        throw CExceptions("In function CDirectory::isCurrent() - Can't get stat info: " + (string)strerror(errno),"red");
    
    int size2 = file.st_size;

    if( size != size2)
        return true;
    return false;
}

void CFile::Delete() const {
    if( remove( path.c_str() ) != 0 )
         throw CExceptions("In function  CFile::Delete() couldn't remove the file: " + (string)strerror(errno),red);
}

void CFile::Copy( const std::string &dest ) const {
    string target = dest + '/' + name;
    vector<uint8_t> bytes;
    uint8_t byte;

    ifstream input(path, ios::in | ios::binary);
    if (!input.good() || !input || !input.is_open()) 
        throw CExceptions("In function CFile::BackUp() couldn't reload the file (" + path + ')',red);;

    while (input.read((char *)&byte, sizeof(byte))) {
        bytes.emplace_back(byte);
    }
    input.close();

    ofstream output( target, ios::binary );
    if (!output.good() || !output || !output.is_open()) 
        throw CExceptions("In function CFile::BackUp() couldn't copy the file (" + target + ')',red);

    for( auto it : bytes )
        output.write(reinterpret_cast<char*>(&it), sizeof it);

    output.close();
}