#include "Options.hpp"
using namespace std;

COptions::COptions(int argc, char** targv) : argc(argc) {
    for (int i = 0; i < argc; ++i) {
        argv.emplace_back(targv[i]);

        //removing the last slash
        if (argv[i].back() == '/') 
            argv[i].erase( argv[i].find_last_of('/') );
    }

    LoadRepos();
}

void COptions::ToDo() const {
    if (argv[1].compare("--init") == 0) {
        string tarpath;
        tarpath = InitPath();
        Init(tarpath);
    } else if (argv[1].compare("--delete") == 0) {
        Delete();
    } else if (argv[1].compare("--show") == 0) {
        Show();
    } else if (argv[1].compare("--open") == 0) {
        if (argc < 3)
            throw CExceptions("Nothing to open.",blue);

        CRepository repo = FindRepo();
        repo.LoadSnaps();
        repo.ShowBackups();
        sleep(1);
      
        if ( argc == 4 && argv[3].compare("--backup") == 0 ) {
            repo.Update();
        } else {
            cout << endl << "At your service:" << endl;
            Open( repo );
        }
    
    } else if (argv[1].compare("--help") == 0) {
        Help();
    } else {
        cout << blue << "Invalid parameter" << endl;
        sleep(1);
        cout << "If you need a help, try --help." << nc << endl;
    }
}

void COptions::Delete() const {    
    if (argc < 3)
        throw CExceptions("Nothing to delete.",yellow);

    CRepository repo = FindRepo();
    repo.Delete();

    string pathname, originname;
    ifstream config("program.config/.repos");
    ofstream temp("temp.txt");

    if ( !config.good() || !temp.good() )
        throw CExceptions("In fuction COptions::Delete() the repos config si broken.", red);

    int id;
    string line, tmp;
    string del = argv[2];
    bool deleted = false; 
    while ( getline(config, line) ) {
        stringstream ss(line);
        ss >> id >> tmp;

        if( to_string(id).compare(del) != 0 && pathname != repo.GetPathname() && originname != repo.GetOrigpath() )
            temp << line << endl;
        else
            deleted = true;
    }
  
    config.close();
    temp.close();

    remove("program.config/.repos");
    rename("temp.txt", "program.config/.repos");

    if( deleted )
        cout << boldGreen << "Deleted succesfully" << endl;
    else
        cout << red << "Couldn't find it" << endl;
    cout << nc << endl;
}

string COptions::InitPath() const {
    if (argc == 2)
        throw CExceptions("Nothing to initialize.",yellow);

    if ( MStrRepos.find(argv[2]) != MStrRepos.end() )
        throw CExceptions("The repository with given path already exists.",yellow);

    string srcpath = argv[2]; //source path

    //check if target folder is the same as the source
    if (argc == 4) {
        auto it = srcpath.find_last_of('/');
        it++;
        string name = srcpath.substr(it);

        return argv[3] + '/' + name + ".backup";
    }

    return srcpath + ".backup";
}

void COptions::Repository( const string &tarpath ) const {
    if ( mkdir(tarpath.c_str(), 0777) )
        throw CExceptions("In function COptions::Repository() couldn't make the repository: " + (string)strerror(errno) + '.',red);

    //create a backup config directory
    string confpath = tarpath + "/back.config";
    if ( mkdir(confpath.c_str(), 0777) ) 
        throw CExceptions("In function COptions::Repository() couldn't make a backup config directory.",red);

    //create a backups config file
    confpath += "/.backs";
    ofstream conf;
    conf.open(confpath);
    if ( !conf )
        throw CExceptions("In function COptions::Repository() couldn't create a backups config file.",red);
    conf.close();

    //make a directory for reconstruction
    confpath = tarpath + "/back.config/recon";
    if ( mkdir(confpath.c_str(), 0777) ) 
        throw CExceptions("In function COptions::Repository() couldn't make a directory for reconstruction.",red);  
}

void COptions::Init( const string &tarpath ) const {
    Repository(tarpath);
    CRepository repo(0, tarpath, argv[2]);
    repo.FullBackup();

    //save repo into config file of repositories
    ofstream config("program.config/.repos", ios::app);

    if (!config.good())
        throw CExceptions("In function COptions::Init() repos config file is broken.",red);

    int id = NewRepoID();

    config << id << ' ' << tarpath << ' ' << argv[2] << endl;
    config.close();

    cout << boldGreen << "Initialization ended successfully." << nc << endl;
}

void COptions::Show() const {
    ifstream config("program.config/.repos");

    if (!config.good())
        throw CExceptions("In function CSwitch::Show() the repo config file is missing.",red);

    CTable table(3, brightblue);
    table.AddHeader( "ID" );
    table.AddHeader( "Repositories" );
    table.AddHeader( "Origin data" );

    string line;
    while (!config.eof()) {
        getline(config, line);

        if (!line.empty()) {
            stringstream ssfir(line);

            int id;
            string repopath, origpath;
            ssfir >> id >> repopath >> origpath;  

            table.AddToRow( to_string(id) );
            table.AddToRow( repopath );
            table.AddToRow( origpath );
        }
    }
    config.close();

    table.Print();
    table.PrintDots();
    cout << brightboldBlue << " Total repositories: " << table.GetTableSize()-1 << nc << endl;
}

void COptions::Help() const {
    string line;
    ifstream helping("program.config/options.txt");

    if (!helping.good())
        throw CExceptions("In function COptions::Help() the documentation file is broken.",red);

    cout << brightpink;
    while (!helping.eof()) {
        getline(helping, line);
        cout << line << endl;
    }
    cout << nc;

    helping.close();
}

const CRepository &COptions::FindRepo() const {
    int id = 0;
    string path;
    CRepository repo;
    //check if the argument was id of repository or path
    if( argv[2].length() < 4 ) {
        id = stoi(argv[2]);

        auto it = MIntRepos.find( id );
        if( it == MIntRepos.end() )
            throw CExceptions("Invalid id of repository.",yellow);
        return it->second;
     
    } else if( MStrRepos.find( argv[2] ) != MStrRepos.end() ) {
        path = argv[2];

        auto it = MStrRepos.find( path );
        if( it == MStrRepos.end() )
            throw CExceptions("Invalid origin path of repository.",yellow);
        return it->second;

    } else 
        throw CExceptions("Invalid indetifier of repository.",yellow);
}

void COptions::Open( const CRepository &repo ) const {
    string command;
    string option;
    int id1, id2;
    while (1) {
        getline(cin,command);
        stringstream ss(command);
        ss >> option >> id1 >> id2;
    
        if ( cin.eof() ) {                
            break; 
        } else if (option.compare("") == 0) {
            continue;
        } else if (option.compare("--diff") == 0) {
            repo.BackupCompare( id1, id2 );
        } else if (option.compare("--backup") == 0) {
            repo.Update();
        } else if (option.compare("--restore") == 0) {
            repo.Restore( id1 );
        } else if (option.compare("--time") == 0) {
            cout << ".---------------- minute (0 - 59)" << endl;
            cout << "| .------------- hour (0 - 23)" << endl;
            cout << "| | .---------- day of month (1 - 31)" << endl;
            cout << "| | | .------- month (1 - 12)" << endl;
            cout << "| | | | .---- day of week (0 - 6) (Sunday=0 or 7)" << endl;
            cout << "| | | | |" << endl;
            cout << "* * * * *" << endl;
            sleep(1);

            string line;
            getline(cin,line);
            repo.SetTime( line );

        } else if (option.compare("--help") == 0) {                
            Help();
        } else if (option.compare("exit") == 0) {                
            break; 
        } else {    
            cout << blue << "Invalid parameter." << endl;
            sleep(1);
            cout << "If you need help, try --help." << nc << endl;
        }
        option.clear();
    }
    cout << boldGreen << "The program succesfully ended." << nc << endl;
}

void COptions::LoadRepos() {
    string pathname, originname;
    ifstream config("program.config/.repos");

    if (!config.good())
        throw CExceptions("In fuction COptions::LoadRepos() the repos config si broken.", red);

    string line;
    unsigned int id;
    while (!config.eof()) {
        getline(config, line);

        stringstream ss(line);
        ss >> id >> pathname >> originname;

        CRepository repo(id, pathname, originname);
        MIntRepos.emplace(id,repo);
        MStrRepos.emplace(originname,repo);
    }

    config.close();
}

int COptions::NewRepoID() const {
    ifstream Rconfig("program.config/.repos");

    if (!Rconfig.good())
        throw CExceptions("In function COptions::IDCount() repo config file is broken.",red);

    string tmp;
    int id = 0;
    while( !Rconfig.eof() ) {
        getline(Rconfig, tmp);
        ++id;
    }

    Rconfig.close();
    return id;
}
