#include "Time.hpp"
using namespace std;

CTime::CTime( int year, int month , int day, int hour, int min, int sec )
        : year(year), month(month), day(day), hour(hour), min(min), sec(sec) {
    strTime = GetStringTime( false );
}

CTime::CTime( const CTime &t ) {
    year = t.year;
    month = t.month;
    day = t.day;
    hour = t.hour;
    min = t.min;
    sec = t.sec;
    strTime = t.strTime;
}    

void CTime::Print() const {
    cout << setw(2) << setfill( '0' ) << day << "." 
         << setw(2) << setfill( '0' ) << month << "." << year << " " 
         << setw(2) << setfill( '0' ) << hour << ":" 
         << setw(2) << setfill( '0' ) << min << ":" 
         << setw(2) << setfill( '0' ) << sec << "  str: " << strTime << "end";
}

bool CTime::IsDiff( const CTime & t ) const {
    if( year == t.year && month == t.month && day == t.day && hour == t.hour && min == t.min )
        return sec < t.sec ? true : false;
    if( year == t.year && month == t.month && day == t.day && hour == t.hour )
        return min < t.min ? true : false;
    if( year == t.year && month == t.month && day == t.day )
        return hour < t.hour ? true : false;
    if( year == t.year && month == t.month )
        return day < t.day ? true : false;
    if( year == t.year )
        return month < t.month ? true : false;
    return year < t.year ? true : false;
}

string CTime::GetStringTime( bool IsFull ) const {
    string time;

    if( day < 10 ) 
        time += '0' + to_string(day);
    else 
        time += to_string(day);

    if( month < 10 )
        time += '0' + to_string(month);
    else 
        time += to_string(month);

    if( IsFull )
        time += to_string(year) + '=';
    else
        time += to_string(year) + '-';

    if( hour < 10 )
        time += '0' + to_string(hour);
    else
        time += to_string(hour);
    
    if( min < 10 )
        time += '0' + to_string(min);
    else
        time += to_string(min);

    if( sec < 10 )
        time += '0' + to_string(sec);
    else
        time += to_string(sec);

    return time;
}
