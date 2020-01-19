#include "Table.hpp"
using namespace std;

CTable::CTable( int num, const std::string &scolor ) : colNum(num) {
    VLen.resize(colNum);

    for( size_t i = 0; i == colNum; ++i )
        VLen[i] = 0;

    color = scolor;
}

void CTable::UpdateMaxLen( const string &text ) {
    if( text.length() > VLen[VRow.size()] )
        VLen[VRow.size()] = text.length()+AddSpaces;
}

void CTable::AddHeader( const string &header ) {
    UpdateMaxLen( header );

    VRow.emplace_back(header);

    if( VRow.size() == colNum ) {
        VTable.emplace_back(VRow);
        VRow.clear();
    }
}

void CTable::AddToRow( const string &text ) {
    UpdateMaxLen( text );

    VRow.emplace_back(text);

    if( VRow.size() == colNum ) {
        VTable.emplace_back(VRow);
        VRow.clear();
    }
}

void CTable::PrintDots() const {
    int len = AddSpaces*2;
    for( auto it : VLen ) 
        len += it;

    cout << color;  
    for( int i = 0; i < len; ++i )
        cout << '.';
    cout << nc << endl;
}

void CTable::PrintHeader( int columns ) const {
    cout << color;
    for( int column = 0; column < columns; ++column ) {
        string header = VTable[0][column];
        int headLen = header.length();
        int colLen = VLen[column] - headLen;
        int avgLen = colLen/2;

        for( int space = 0; space < avgLen; ++space )
            cout << ' ';

        cout << header;

        for( int space = 0; space < avgLen; ++space )
            cout << ' ';
    }
    cout << nc << endl;
}

void CTable::Print() const {
    int rows = VTable.size();
    int columns = VTable[0].size();
    
    if( VTable.size() == 1 )
        throw CExceptions("Nothing to print.",yellow);

    PrintHeader(columns);
    PrintDots();

    cout << color;
    for( int row = 1; row < rows; ++row ) {
        for( int column = 0; column < columns; ++column ) {
            cout.width(VLen[column]);
            cout << left << setw(VLen[column]) << setfill(' ') << VTable[row][column];
        }
        cout << endl;
    }
    cout << nc;
}