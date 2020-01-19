#include "Snapshot.hpp"
using namespace std;

CSnapshot::CSnapshot( unsigned int id, const std::string &path, bool IsFull ) : IsFull(IsFull), id(id), snappath(path) {
    struct stat snap;
    if (stat(snappath.c_str(), &snap) == -1)
        throw CExceptions("In Constructor of CSnapshot couldn't get stat info: " + (string)strerror(errno)
                            + '\n' + snappath,red);

    struct tm *mod;
    mod = gmtime(&(snap.st_mtime));
    
    CTime time( mod->tm_year+1900, mod->tm_mon+1, mod->tm_mday, mod->tm_hour+2, mod->tm_min, mod->tm_sec );
    modified = time;

    auto it = path.find_last_of('/');
    ++it;
    name = path.substr(it);
}

//Incomplete
void CSnapshot::Patch( const CSnapshot &next ) {
    Load( snappath );
    for( auto it : SDir )
        it.Print();

    //next.Load( next.GetPath() );
    for( auto it : next.GetSet() )
        it.Print();
}

void CSnapshot::Backup( const CSnapshot &recon ) {
    if( !Compare( recon ) )
        throw CExceptions("Backup is updated.",blue);

    auto it = SDiffs.begin();
    it = SDiffs.erase(it);

    if( mkdir(snappath.c_str(), 0777) )
            throw CExceptions("In function CSnapshot::Backup() couldn't create backup directory.",red);

    string confpath = snappath + "/.deleted";
    ofstream config(confpath);

    if( !config.good() )
        throw CExceptions("In function CSnapshot::Backup() couldn't create config files for deleted document: " + (string)strerror(errno),red);

    //iterate directories
    for( auto it : SDiffs ) {
        if( it.comp == '!' ) {
            it.back->Diff( it.orig, snappath );

            //iterate documents
            for( auto ite : it.VDoc ) {
                if( ite.comp == '!' ) {
                    string dest = snappath + it.back->GetDepname();
                    ite.back->Diff( ite.orig, dest );
                } else if( ite.comp == '-' ) {
                    config << "F " << ite.back->GetPath();
                }
            }

        } else if( it.comp == '-' ) {        
            config << "D " << it.back->GetPath();
        } else if( it.comp == '+' ) {
            it.orig->Copy( snappath );
        } 
    }
    recon.UpdateRecon( SDiffs );
}

void CSnapshot::LoadFile( const string &path, multimap<int, pair<char, string> > &tmp ) {
    ifstream file(path);

    if( !file.good() )
        throw CExceptions("In function CSnapshot::LoadFile() couldn't load file for patching.",red);

    int num = 0;
    string line;
    while( !file.eof() ) {
        getline(file, line);
        ++num;
        
        stringstream ss(line);

        int num;
        char comp;
        string text;

        ss >> comp >> num; 
        getline( ss, text );

        tmp.emplace( num, make_pair( comp, text ) );
    }

    file.close();
}

//Incomplete
void CSnapshot::Reconstruction( const CSnapshot &last ) {
    //deleting files in recon
    string confpath = last.GetPath() + "/.deleted";
    ifstream config(confpath);

    if( !config.good() )
        throw CExceptions("In function CSnapshot::Reconstruction() config files of deleted document is broken.",red);

    string line;
    while( getline(config, line) ) {
        stringstream ss(line);

        char type;
        string path;
        ss >> type >> path;

        if( type == 'D' ) {
            if( rmdir( path.c_str() ) )
                throw CExceptions("In function CSnapshot::Reconstruction() couldn't remove the directory from recon.",red);
        } else {
            if( remove( path.c_str() ) )
                throw CExceptions("In function CSnapshot::Reconstruction() couldn't remove the file from recon.",red);
        }
    }
}

void CSnapshot::UpdateRecon( const set<SCompDirs, SDiffCompare> &SDiffs ) const {
    //iterate directories
    for( auto it : SDiffs ) {
        if( it.comp == '!' || it.comp == '+' ) {
            it.back->Copy( snappath );

            //iterate documents
            for( auto ite : it.VDoc ) {
                if( ite.comp == '!' || ite.comp == '+' ) {
                    ite.back->Copy( snappath );
                }
            }
        } else if( it.comp == '-' ) {        
            it.back->Delete();
        }
    }
}

void CSnapshot::UpdateConfig( const string &repopath ) const {
    string confpath = repopath + "/back.config/.backs";
    ofstream conf(confpath, ios::app);

    if ( !conf.good() )
        throw CExceptions("In function CSnapshot::UpdateConfing() the backup config file is broken.",red);

    conf << id << ' ' << snappath << ' ' << modified.GetStrTime() << endl;
    conf.close();
}

bool CSnapshot::Compare( const CSnapshot &s1 ) {
    CBackDiff diffs( SDir, s1.GetSet() );

    if( !diffs.IsDiff() )
        return false;

    SDiffs = diffs.GetDiffs();
    return true;
}

void CSnapshot::Load( const std::string &origpath ) {
    CLoader load(origpath);
    SDir = load.LoadDocs();
}

void CSnapshot::FullBackup( const string &origpath ) {
    Load( origpath );

    for( auto it : SDir )
        it.Copy( snappath );
}

void CSnapshot::Print() const {
    for( auto it : SDir )
        it.Print();
}

bool CSnapshot::SnapCompare( const CSnapshot &snap ) const {
    if( modified.IsDiff( snap.modified ) ) 
        return true;
    return false;
}

void CSnapshot::PrintDiff( const CSnapshot &sec ) const {
    CTable table(3,white);

    table.AddHeader( GetModified().GetStrTime() );
    table.AddHeader( "Compare" );
    table.AddHeader( sec.GetModified().GetStrTime() );

     for( auto it : SDiffs ) {
        table.AddToRow( it.back->GetDepname() );
        table.AddToRow( string(1, it.comp) );
        table.AddToRow( it.orig->GetDepname() );
        
        if( it.comp == '!' || it.comp == '+' || it.comp == '-' ) {
            for( auto ite : it.VDoc ) {
                table.AddToRow( " " + ite.back->GetName() );
                table.AddToRow( string(1, ite.comp) );
                table.AddToRow( " " + ite.orig->GetName() );
            }
        }
    }

    table.Print();
    table.PrintDots();
}
