#pragma once
//C++ library
#include <iostream>
#include <iomanip>

/**
 * Class represents time  
 */
class CTime {
    int year;               /**< year */
    int month;              /**< month */
    int day;                /**< day */
    int hour;               /**< hour */
    int min;                /**< min */
    int sec;                /**< second */
    std::string strTime;    /**< time in string */

public:

    /**
     * Constructor
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param min
     * @param sec
     * @param time
     */
    CTime( int year, int month, int day, int hour, int min, int sec, const std::string &time )
     : year(year), month(month), day(day), hour(hour), min(min), sec(sec), strTime(time) {};

     /**
     * Constructor
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param min
     * @param sec
     */
    CTime( int year, int month, int day, int hour, int min, int sec );

    /**
     * Default constructor
     */
    CTime() {};

    /**
     * Copy constructor
     * @param t Another timestamp
     */
    CTime( const CTime &t );

    /**
     * Print time in format "DDMMYYYY-HHMMSS"
     */
    void Print() const;

    /**
     * Compares two timestamps
     * @return true if the second timestamp is newer than the first one, else false
     */
    bool Compare( const CTime & ) const;

    /**
     * Finds differences of two timestamps
     * @return true if they are different, else false
     */
    bool IsDiff( const CTime & ) const;

    /**
     * Getter for name of timestamp
     * @param IsFull If true the name of timestamp of complete backup
     * @return timestamp
     */
    std::string GetStringTime( bool IsFull ) const;

    /**
     * Getter fortimestamp
     * @return timestamp
     */
    std::string GetStrTime() const { return strTime; };
};

/*
 * Struct contain method for compare snapshots by created
 */
struct STimeCompare {
    /**
     * Functor for comparing
     * @param t1 First timestamp
     * @param t2 Second timestamp
     * @return true if the second timestamp is newer than the first one
     */
    bool operator()( const CTime &t1, const CTime &t2 ) const {
        if( t1.IsDiff(t2) )
            return true;
        return false;
    }
};