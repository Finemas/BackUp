//C library
#include <unistd.h>

//Header files
#include "Options.hpp"
#include "Exception.hpp"
#include "Palette.hpp"
using namespace std;

int main( int argc, char** argv ) {
    if( argc == 1 ) {
        cout << blue << "No arguments" << endl;
        sleep(1);
        cout << "If you need a help, try --help." << nc << endl;
        return 1;
    } 

    try { 
        COptions opt( argc, argv );
        opt.ToDo();
    } catch( CExceptions &exp ) {
        cout << exp.what() << endl;
    }
   
    return 0; 
} 
