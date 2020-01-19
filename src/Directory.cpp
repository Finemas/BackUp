#include "Directory.hpp"
using namespace std;

CDirectory::CDirectory( const string &path, const string &depname, int depth ) 
                : CDocument(path), depth(depth), depname(depname) {
    struct stat info;

    if( stat( path.c_str(), &info ) )
        throw CExceptions("Constructor CDirectory couldn't get stats.",red);
    
    struct tm *mod;
    mod = gmtime(&(info.st_mtime));
    
    CTime time( mod->tm_year+1900, mod->tm_mon+1, mod->tm_mday, mod->tm_hour+2, mod->tm_min, mod->tm_sec ); 
    modified = time;
}

CDirectory::CDirectory( const CDirectory &dir ) : CDocument( dir.GetPath() ) {
    depth = dir.GetDepth();
    modified = dir.GetModified();
    depname = dir.GetDepname();
    SDoc = dir.GetSet();
}

void CDirectory::Diff( const shared_ptr<CDocument> &doc, const string &destpath ) {
    shared_ptr<CDirectory> dir = dynamic_pointer_cast<CDirectory>(doc);
    string target = destpath + depname;

    if( !IsDiff( dir->GetPath() ) )
        return;
    SDoc.emplace( dir );
}

void CDirectory::Patch( const std::string &destpath ) {
    Copy( destpath );
}

void CDirectory::LoadDir( const string &spath, set<string> &SPaths ) const {
    DIR *dir = opendir(spath.c_str());

    if ( dir == nullptr )
        throw CExceptions("In functioin CDirectory::SizeCompare() couldn't open the directory.",red);

    string fname;
    dirent *files;
    while ((files = readdir(dir)) != nullptr) {
        fname = files->d_name;
        if ( fname[0] == '.' )
            continue;

        SPaths.emplace( fname );
    }
    closedir (dir);
}

bool CDirectory::IsSizeDiff( const string &file1, const string &file2 ) const {
    struct stat info1;
    struct stat info2;

    if( stat(file1.c_str(), &info1 ) )
        throw CExceptions("In function CDirectory::IsSizeDiff() couldn't get stat first of file: " + file1,red);

    if( stat(file2.c_str(), &info2 ) )
        throw CExceptions("In function CDirectory::IsSizeDiff() couldn't get stat second of file: " + file2,red);

    int size1 = info1.st_size;
    int size2 = info2.st_size;
    
    cout << size1 << " x " << size2 << endl;
    if( size1 != size2 )
        return true;
    return false;
}

bool CDirectory::SizeCompare( const string &secpath ) const {
    set<string> SPaths;
    string target = path + depname;
    LoadDir( path, SPaths);

    DIR *dir = opendir( secpath.c_str() );

    if( dir == nullptr )
        throw CExceptions("In functoin CDirectory::SizeCompare() couldn't open the directory: " + secpath,red);

    dirent *files;
    string fname, file1path, file2path;
    while ((files = readdir(dir)) != nullptr) {
        fname = files->d_name;
        if (fname[0] == '.')
            continue;

        for( auto it : SPaths ) {
            if( it== fname ) {
                file1path = secpath + '/' + it;
                file2path = path + '/' + fname;

                if( IsSizeDiff( file1path, file2path ) ) 
                    return true;
            }
        }
    }
    closedir (dir);
    return false;
}

bool CDirectory::IsDiff( const string &pathname ) const {
    if( SizeCompare(pathname) )
        return true;
    return false;
}

void CDirectory::Copy( const string &dest ) const {
    string target = dest;
    if( !depname.empty() ) {
        string pathname = depname.substr(1);
        int slashNum = count(pathname.begin(), pathname.end(), '/')+1;

        string name = "";
        for( int i = 0; i != slashNum; ++i ) {
            auto it = pathname.find_first_of('/');
            ++it;
            name += '/' + pathname.substr(0,it-1);

            target = dest + name;
            if ( mkdir(target.c_str(), 0777) && errno != EEXIST )
                throw CExceptions("In function CDirectory::Copy() couldn't copy the directory: " + (string)strerror(errno),red);

            pathname.erase(0,it);
        }   

    }

    for( auto it : SDoc )
        it->Copy( target );
}

void CDirectory::Print() const {
    cout << depname << endl;
    for( auto it : SDoc ) {
        cout << ' ';
        it->Print();
        cout << endl;
    }
}

void CDirectory::Delete() const {
    for( auto it : SDoc )
        it->Delete();

    if ( rmdir(path.c_str()) )
        throw CExceptions("In function CDirectory::Delete() couldn't remove the directory " + path + ": " + (string)strerror(errno),red);
}

void CDirectory::PrintDir() const {
    cout << name;
}
