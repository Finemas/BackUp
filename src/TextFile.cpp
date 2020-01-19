#include "TextFile.hpp"
using namespace std;

void CTextFile::Print() const {
    cout << "  T " << name << ' ' << size;
}

void CTextFile::LoadFile() {
    ifstream file(path);

    if( !file.good() ) 
        throw CExceptions("In function CBinaryFile::LoadFile() couldn't load file.",red);

    int num = 0;
    string line;
    while( !file.eof() ) {
        getline(file, line);
        ++num;

        char comp;
        if( line.empty() )
            comp = '/';
        else
            comp = '*';

        MDiff.emplace( num, make_pair(comp,line) );
    }

    file.close();
}

void CTextFile::LoadDiff( const string &diffpath ) {
    ifstream file(diffpath);

    if( !file.good() ) 
        throw CExceptions("In function CBinaryFile::LoadDiff() couldn't load file.",red);

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

        MDiff.emplace( num, make_pair( comp, text ) );
    }

    file.close();
}

void CTextFile::Diff( const std::shared_ptr<CDocument> &doc, const std::string &destpath ) {
    shared_ptr<CTextFile> text = dynamic_pointer_cast<CTextFile>(doc);

    LoadFile();
    for( auto it : MDiff ) {
        cout << it.second.first << ' ' << it.first << ' ' << it.second.first << endl;
    }

    text->LoadFile();

    multimap<int, pair<char, string> > MOrig = text->GetMap();

    char comp1 = '*';
    auto tmp = MOrig.begin();
    for( auto nw = MDiff.begin(); nw != MDiff.end(); ++nw ) {
        if( nw->second.second.empty() ) {
            if( nw->first < (int)MOrig.size() )
                nw->second.first = comp1;
            else
                nw->second.first = '+';
            continue;
        }

        char comp2 = '*';
        int found = 0;
        for( auto orig = tmp; orig != MOrig.end(); ++orig ) {
            if( orig->second.second.empty() ) {
                orig->second.first = comp2;
                continue;
            }
        
            if( nw->second.second.length() == orig->second.second.length() && nw->second.second.compare( orig->second.second ) == 0 ) {
                orig->second.first = '=';
                tmp = orig;
                nw->second.first = '=';
                found = 1;
            }

            comp2 = orig->second.first ;
        }

        if( !found )
            nw->second.first = '+';
        comp1 = nw->second.first;
    }

    for( auto it : MOrig ) {
        if( it.second.first == '*' )
            MDiff.emplace( it.first, make_pair('-',it.second.second) );
    }

    auto it = MDiff.begin();
    while( it != MDiff.end() ) {
        if( it->second.first == '=' )
            it = MDiff.erase(it);
        else 
            ++it;
    }
    
    string target = destpath + name;
    SaveDiff( target );
}
    
void CTextFile::Patch( const string &diffpath ) {
    LoadDiff( diffpath );
    ifstream file( path );

    if( !file )
        throw CExceptions("Can't open file ",red);

    string line;
    int numline = 0;
    auto it = MDiff.begin();
    while( !file.eof() ) {
        getline(file, line);
        ++numline;
        
        if( it == MDiff.end() ) {
            VContent.emplace_back( line );
            continue;
        }

        if( it->first == numline && it->second.first == '-'  ) {
            while( it->second.first == '-' ) {
                VContent.emplace_back( it->second.second );
                ++it;
            }
            continue;
        }

        if( it->second.second.compare( line ) != 0 ) {
            VContent.emplace_back( line );
        } else {
            ++it;
            --numline;
        }
    }

    file.close();
    SavePatch( diffpath );
}

void CTextFile::SavePatch( const string &target ) const {
    ofstream patch(target);

    if( !patch.good() ) 
        throw CExceptions("In function CTextFile::SavePatch() couldn't save patch.",red);

    if( VContent.empty() )
        return;

    if( VContent.size() == 1 ) {
        auto it = VContent.begin();
        patch << *it;
    }

    auto bck = VContent.end();
    --bck;
    for( auto it = VContent.begin(); it != bck; ++it )
        patch << *it << endl;
    patch << *bck;

    patch.close();
}

void CTextFile::SaveDiff( const string &target ) const {
    //writing difference into file
    ofstream backup(target, ios::binary | ios::app );

    if( !backup ) 
        throw CExceptions("Inc function CBinaryFile::Diff() couldn't save differences.",red);

    if( MDiff.empty() ) {
        backup.close();
        return;
    }

    if( MDiff.size() == 1 ) {
        auto it = MDiff.begin();
        cout << it->second.first << ' ' << it->first << it->second.second;
    }

    auto bck = MDiff.end();
    --bck;
    char comp = MDiff.begin()->first;
    for( auto it = MDiff.begin(); it != bck; ++it ) {
        if( MDiff.count(it->first) > 1 ) {
            if( it->first == comp ) {
                backup << it->second.first << ' ' << it->first << it->second.second << endl;
            } else {
                auto range = MDiff.equal_range(it->first);
                for( auto i = range.first; i != range.second; ++i ) {
                    if( i->second.first == comp )
                        backup << i->second.first << ' ' << i->first << i->second.second << endl;
                }
            }
        } else {
            backup << it->second.first << ' ' << it->first << it->second.second << endl;
        }
        comp = it->second.first;
    }

    if( MDiff.count(bck->first) > 1 ) {
            if( bck->first == comp ) {
                backup << bck->second.first << ' ' << bck->first << bck->second.second;
            } else {
                auto range = MDiff.equal_range(bck->first);
                for( auto i = range.first; i != range.second; ++i ) {
                    if( i->second.first == comp )
                        backup << i->second.first << ' ' << i->first << i->second.second;
                }
            }
    } else {
        backup << bck->second.first << ' ' << bck->first << bck->second.second;
    }

    backup.close();
}