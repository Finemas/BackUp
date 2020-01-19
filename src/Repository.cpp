#include "Repository.hpp"
using namespace std;

//Incomplete
void CRepository::Restore( unsigned int id ) const {
    pair<map<int, CSnapshot>::const_iterator, map<int, CSnapshot>::const_iterator > range;
    for( auto it = MIDs.begin(); it != MIDs.end(); ++it ) {
        if( it->second.GetFull() )
            range.first = it;
        if( it->first == (int)id ) {
            range.second = it;
            break;
        }
    }

    // if( range.first == range.second ) {
    //     range.first->second.SavePatch();
    // }

    auto it1 = range.first;
    auto it2 = ++range.first;
    CSnapshot patch = it1->second;
    patch.Load( patch.GetPath() );
    while( it2 != range.second ) {
        patch.Patch( it2->second );
        ++it2;
    }
}

void CRepository::Update() const {
    CTime time = GetTimestamp();
    string shotpath = pathname + '/' + time.GetStrTime();
    CSnapshot orig( NewID(), shotpath, time, false );
    orig.Load( originpath );
    orig.UpdateConfig( pathname );

    string reconpath = pathname + "/back.config/recon";
    CSnapshot recon( 0, reconpath, false );
    recon.Load( reconpath );

    orig.Backup( recon );
}

void CRepository::SetTime( const string &line ) const {
    stringstream ss(line);

    string min, hour, mday, month, wday;
    ss >> min >> hour >> mday >> month >> wday;

    //prepare shell script
    string script = "program.config/backup.sh";

    bool empty = false;

    ifstream ifile ( script );
    if ( ifile.peek() == EOF )
	empty = true;
    ifile.close();

    ofstream file( script, ios::app );

    char *path = NULL;
    path = getcwd ( path , 100 );

    if ( empty ) {
	    file << "#!/bin/bash" << endl << endl;
	    file << "cd " << path << endl;
	    file << "make compile" << endl << endl;
    }

    string open = "./provaja6 --open " + to_string(id) + " --backup";
    file << open << endl;

    file.close(); 

    ofstream cron("program.config/cron.txt");

    if( !cron )
        throw CExceptions("In function CRepository::SetTime() couldn't set cron.",red);

    cron << min + ' ' + hour + ' ' + mday + ' ' + month + ' ' + wday + "  cd " << path << "/program.config && ./backup.sh" << endl;
    
    //cron << crontabe << endl;
    cron.close();

    free ( path );

    string command = "crontab $PWD/program.config/cron.txt";
    system(command.c_str());
}

void CRepository::Delete() {
    std::set<CDirectory, SDirCompare> SRepo;    /**< The path of repository */
    CLoader load( pathname );
    SRepo = load.LoadDocs();

    string conf = pathname + "/back.config/.backs";
    remove(conf.c_str());
    
    for( auto it = SRepo.rbegin(); it != SRepo.rend(); ++it )
        (*it).Delete();
}

void CRepository::FullBackup() const {
    CTime time = GetTimestamp();
    //reconstruction
    string reconpath = pathname + "/back.config/recon";

    CSnapshot recon( 0, reconpath, true );
    recon.FullBackup(originpath);
    recon.UpdateConfig(pathname);

    //fullbackup
    string path = pathname + '/' + time.GetStringTime( true );

    if ( mkdir(path.c_str(), 0777) == -1 ) 
        throw CExceptions("In function CRepository::Backup() can't make a first backup.",red);
 
    CSnapshot snap( NewID(), path, true );
    snap.FullBackup( originpath );
    snap.UpdateConfig( pathname );
}

int CRepository::NewID() const {
    string conf = pathname + "/back.config/.backs";
    ifstream Rconfig(conf);

    if (!Rconfig.good())
        throw CExceptions("In function CRepository::GetNewID() snap config file is broken.",red);

    string tmp;
    int id = 0;
    while( 1 ) {
        getline(Rconfig, tmp);
        if( Rconfig.eof() )
            break;
        else     
            ++id;
    }

    Rconfig.close();
    return id;
}

CTime CRepository::GetTimestamp() const {
    time_t t = time(nullptr);  // get time now
    tm* now = localtime(&t);

    CTime time( now->tm_year+1900, now->tm_mon+1, 
                now->tm_mday, now->tm_hour, 
                now->tm_min, now->tm_sec );

    return time;
}

void CRepository::ShowBackups() const {
    CTable table(3, brightyellow);

    table.AddHeader( "ID" );
    table.AddHeader( "Created" );
    table.AddHeader( "Modified" );

    for( auto it : MBackups ) {
        table.AddToRow( to_string(it.second.GetID()) );
        table.AddToRow( it.second.GetName() );
        table.AddToRow( it.second.GetModified().GetStrTime() );
    }
      
    table.Print();
    table.PrintDots();

    cout << brightboldYellow << " Total backups: " << table.GetTableSize()-1 << nc << endl << endl; 
}

void CRepository::LoadSnaps() {
    DIR * dir = opendir( pathname.c_str() );

    if( dir == nullptr || errno == EDOM )
        throw CExceptions("In function CRepository::LoadSnapshots() couldn't load snapshots: " + (string)strerror(errno),red);

    dirent * snap;
    string name, path;
    while( ( snap = readdir( dir )) != nullptr ) {
        name = snap->d_name; 
        if( name[0] == '.' || name == "back.config" )
            continue;
    
        CTime time( stoi( name.substr(4,4) ),
                    stoi( name.substr(2,2) ),
                    stoi( name.substr(0,2) ),
                    stoi( name.substr(9,2) ),
                    stoi( name.substr(11,2 )),
                    stoi( name.substr(13,2) ) );

        CSnapshot found = FindID( name, pathname );
        MBackups.emplace( time, found );
        MIDs.emplace( found.GetID(), found );
    }
}       

CSnapshot CRepository::FindID( const string name, const string &pathname ) const {
    string conf = pathname + "/back.config/.backs";
    ifstream config(conf);
    if (!config.good())
        throw CExceptions("In function CRepository::FindID() the backup config file is broken.",red);

    string line;
    while( !config.eof() ) {
        getline(config,line); 

        if( line[0] == '0' )
            continue;
        if( line.find(name) != string::npos )
            break;
    }
    config.close();

    if( line.find(name) == string::npos )
        throw CExceptions("In function CRepository::FindID() no mention about the backup's id.",red);

    int id;
    string path;

    stringstream ssfir(line);
    string tmp;
    ssfir >> id >> path >> tmp;

    bool IsFull;
    if( tmp.substr(8,1) == "=" )
        IsFull = true;
    else 
        IsFull = false;

    return CSnapshot(id,path,IsFull);
}

void CRepository::BackupCompare( unsigned int id1, unsigned int id2 ) const {
    CSnapshot s1, s2;

    for( auto it : MBackups ) {
        if( it.second.GetID() == id1 ) 
            s1 = it.second;
        
        if( it.second.GetID() == id2 )
            s2 = it.second;
    }
 
    s1.Load( s1.GetPath() );
    s2.Load( s2.GetPath() );

    s1.PrintDiff(s2);
}     
