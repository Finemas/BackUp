#include "Document.hpp"
using namespace std;

CDocument::CDocument( const std::string &tpath ) : path(tpath) {
    if( path.back() == '/' )
        path.erase( path.find_last_of('/') );

    auto it = path.find_last_of('/');
    it++;
    name = path.substr( it );
}

CDocument::CDocument( const CDocument &doc ) {
    path = doc.GetPath();
    name = doc.GetName();
}
