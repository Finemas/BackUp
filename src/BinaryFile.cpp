#include "BinaryFile.hpp"
using namespace std;

void CBinaryFile::Print() const {
    cout << "  B " << name << ' ' << size;
}

void CBinaryFile::LoadFile() {
    ifstream file( path, ios::binary );

    if( !file.good() ) 
        throw CExceptions("In function CBinaryFile::LoadFile() couldn't load file.",red);

    int num = 0;
    uint8_t bytes8;
    while( file.read((char *)&bytes8, sizeof(bytes8)) ) {
        MDiff.emplace( num, bytes8 );
        ++num;
    } 

    file.close();
}

void CBinaryFile::LoadDiff( const string &diffpath ) {
    ofstream backup( diffpath, ios::binary | ios::trunc );

    if( !backup.good() )
        throw CExceptions("In function CBinaryFile::LoadDiff() couldn't load file.",red);
    
    for( auto it : MDiff ) {
        backup.write((char*)(&it.first), sizeof(it.first));
        backup.write((char*)(&it.second), sizeof(it.second));
    }

    backup.close();
}

void CBinaryFile::Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath ) {
    shared_ptr<CBinaryFile> bin = dynamic_pointer_cast<CBinaryFile>(doc);

    LoadFile();
    bin->LoadFile();
 
    ifstream file( path, ios::binary );

    int num = 0;
    uint8_t newbyte;
    auto it = bin->GetMap().begin();
    while( file.read((char *)&newbyte, sizeof(newbyte)) ) {
        if( it == bin->GetMap().end() ) {
            MDiff.emplace( num, newbyte );
            continue;
        }

        if( it->second != newbyte )
            MDiff.emplace( num, newbyte );
        ++num; ++it;
    }

    while( num < (int)bin->GetMap().size() ) {
        MDiff.emplace( num, it->second );
        ++num; ++it;
    }

    string target = destpath + '/' + name;
    SaveDiff( target );
}

void CBinaryFile::Patch( const string &destpath ) {
    LoadDiff( path );
    string target = destpath + '/' + name;

    ifstream file( destpath, ios::binary );

    if( !file.good() )
        throw CExceptions("In function CBinaryFile::Patch() couldn't open file.",red);
 
    uint8_t newbyte;
    int numline = 0;
    auto it = MDiff.begin();
    while( file.read((char *)&newbyte, sizeof(newbyte)) ) { 
        if( numline == it->first && newbyte != it->second ) {
            VContent.emplace_back( it->second );
            ++it;
        } else {
            VContent.emplace_back( newbyte );
        }
        ++numline;
    }

    while( numline < (int)MDiff.size() ) {
        VContent.emplace_back( it->second );
        ++it; ++numline;
    }

    SavePatch( destpath );
}

void CBinaryFile::SaveDiff( const string &target) const {
    //writing difference into file
    ofstream backup( target, ios::binary | ios::trunc );

    if( !backup.good() )
        throw CExceptions("Inc function CBinaryFile::Diff() couldn't save differences.",red);
    
    for( auto it : MDiff ) {
        backup.write((char*)(&it.first), sizeof(it.first));
        backup.write((char*)(&it.second), sizeof(it.second));
    }

    backup.close();
}

void CBinaryFile::SavePatch( const std::string &target ) const {
    ofstream patch(target, ios::binary );

    if( !patch.good() )
        throw CExceptions("In function CBinaryFile::SavePatch() couldn't save patch.",red);
    
    for( auto it : VContent )
        patch.write((char*)(&it), sizeof(it));

    patch.close();
}