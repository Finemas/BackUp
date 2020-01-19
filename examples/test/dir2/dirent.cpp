#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <map>
#include <queue>
#include <utility>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    multimap<int, pair<char,string> > MFiles;
    queue<string> QDir;
    int depth = 1;
    string path = argv[1];
    string name;

    if( path.back() == '/' ) 
        path.erase( path.find_last_of('/') );

    QDir.push(path);

    string poped;
    string newpath;     
    while( !QDir.empty() ) {
        poped = QDir.front();
        QDir.pop();
        const char *dest = poped.c_str();

        cout << "Opening dir " << dest << endl;
        DIR * dir = opendir( dest );

        if( dir == nullptr || errno == EDOM ) {
           cout << "Open the directory failed: " << strerror(errno) << endl;
           return 1;
        } 

        dirent * files;
        while( (files = readdir( dir )) != nullptr ) {
            if( files->d_name[0] == '.' )
                continue;

            if( files->d_type == DT_DIR ) {
                cout << "D ";
                newpath = poped + '/' + files->d_name;
                cout << newpath << " ";
                QDir.push( newpath );
                MFiles.emplace( depth, make_pair( 'D',files->d_name ) );
            } else if( files->d_type == DT_REG ) {
                cout << "F ";
                MFiles.emplace( depth, make_pair( 'R',files->d_name ) );
            } else if( files->d_type == DT_LNK ) {
                cout << "S ";
                MFiles.emplace( depth, make_pair( 'S',files->d_name ) );
            } else if( files->d_type == DT_UNKNOWN ) {
                cout << "UNKNOWN ";
            }

            cout << files->d_name << endl;
        }  
        
        ++depth;
        closedir (dir);
        cout << endl;
    } 

    int temp = 0;
    for( auto it : MFiles ) {
        if( temp < it.first ) {
            temp = it.first;
            cout << endl;
        }

        cout << it.first << " - " << it.second.first << " " << it.second.second << endl;
    }

    return 0;
}
