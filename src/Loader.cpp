#include "Loader.hpp"
using namespace std;

bool CLoader::IsHardlink(const string &pathname) const {
    struct stat sb;

    if (stat(pathname.c_str(), &sb) == -1) 
        throw CExceptions("In function CSnapshot::IsHardlink() - can't get stat: " + (string)strerror(errno),red);

    if (sb.st_nlink > 1) 
        return true;
    return false;
}

bool CLoader::IsText(const string &pathname) const {
    int print = 0;
    int nonprint = 0;

    fstream file(pathname);

    if (!file.good() || !file || !file.is_open()) 
        throw CExceptions("In function CSnapshot::IsText() - Can't reload the file (" + pathname + ')',red);

    while (!file.eof()) {
        if (isprint(file.get()) == 0)
            ++nonprint;
        else
            ++print;

        if (nonprint == 2 * print) break;
    }

    file.close();

    if (nonprint >= print) return false;
    return true;
}

void CLoader::ReadDir( DIR * dir, const string &poped, const string &name, int &depth ) {
    string fname, newpath;
    CDirectory direc(poped, name, depth);
    dirent *files;
    while ((files = readdir(dir)) != nullptr) {
        fname = files->d_name;
        if (fname[0] == '.') 
            continue;

        if (files->d_type == DT_DIR) {
            newpath = poped + '/' + fname;
            QDir.push(newpath);

        } else if (files->d_type == DT_REG) {
            string fpath = poped + '/' + fname;

            if( IsText( fpath ) )
                direc.AddDocument( CTextFile( fpath) );    
            else
                direc.AddDocument( CBinaryFile( fpath ) );
                
        } else if (files->d_type == DT_LNK) {
            string fpath = poped + '/' + fname;

            if ( IsHardlink( fpath )  )
                direc.AddDocument( CLink(fpath, false) );
            else
                direc.AddDocument( CLink(fpath, true) );
            
        } else if (files->d_type == DT_UNKNOWN) {
            throw CExceptions("In function CLoader::ReadDir() cannot identify type of document.",red);
        }
    }
    SDir.emplace( direc );
}

void CLoader::ItirateQueue() {
    int depth = 0;
    string name, poped;
    while( !QDir.empty() ) {
        int qsize = QDir.size();
        for( int i = 0; i < qsize; ++i ) {
            poped = QDir.front();
            QDir.pop();

            auto it = poped.find(source);
            it += source.length();
            name = poped.substr(it);

            DIR *dir = opendir(poped.c_str());

            if (dir == nullptr || errno == EDOM)
                throw CExceptions("In function CLoader::ItirateQueue() couldn't open the directory: " + (string)strerror(errno),red);

            ReadDir( dir, poped, name, depth );
 
            closedir (dir);
        }
        ++depth;
    } 
}

std::set<CDirectory, SDirCompare> CLoader::LoadDocs() {
    string path = source;

    QDir.push(path);
    ItirateQueue();

    return SDir;
}