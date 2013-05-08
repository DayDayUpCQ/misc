/**
 \file PQ61850Utility.cpp

 \brief ÊµÓÃº¯Êý
*/
#include "PQ61850Utility.h"

PQ61850Utility::PQ61850Utility()
{

}

PQ61850Utility::~PQ61850Utility()
{

}

PQ61850Utility * PQ61850Utility::GetSingleton()
{
    static PQ61850Utility singletonObj;
    return &singletonObj;
}

void PQ61850Utility::St2UtcTime( const SYSTEMTIME *sysTime, UTCTIME *utcTime )
{

}

void PQ61850Utility::UtcTime2St( const UTCTIME *utcTime, SYSTEMTIME* sysTime )
{

}

void PQ61850Utility::UtcTime2Dt( const UTCTIME *utcTime, DTTIME *dtTime )
{

}

void PQ61850Utility::Dt2UtcTime( const DTTIME *dtTime, UTCTIME *utcTime )
{

}

void PQ61850Utility::DiffSysTime(
        const SYSTEMTIME *sysTime1, 
        const SYSTEMTIME *sysTime2,
        PQ61850UTILITY_DIFFTIME *diffTime )
{


    if ( sysTime1 == NULL || sysTime2 == NULL 
        || diffTime == NULL )
    {
        memset( diffTime, 0, sizeof(*diffTime));
        return;
    }
    SYSTEMTIME tmpSysTime2;
    memcpy_s(&tmpSysTime2, sizeof(SYSTEMTIME), sysTime2, sizeof(SYSTEMTIME));
    if ( tmpSysTime2.wMilliseconds < sysTime1->wMilliseconds )
    {
        diffTime->milliSeconds = 
            tmpSysTime2.wMilliseconds+1000 - sysTime1->wMilliseconds;
    }
    tmpSysTime2
    

    diffTime->days
    for ( int year = sysTime1->wYear; year < sysTime2->wYear; year++ )
    {
        if ( IsLeapYear())
        {
        }
    }
#if 0
    SYSTEMTIME tmpSysTime;
    memcpy_s(&tmpSysTime, sizeof(SYSTEMTIME), sysTime2, sizeof(SYSTEMTIME));
    if ( sysTime2->wMilliseconds < sysTime1->wMilliseconds )
    {
        diffTime->milliSeconds 
            = sysTime2->wMilliseconds + 1000 - sysTime1->wMilliseconds;
        tmpSysTime.wSecond =  tmpSysTime.wSecond  - 1;
    }
    CTime t1(
        sysTime1->wYear, 
        sysTime1->wMonth, 
        sysTime1->wDay,
        sysTime1->wHour,
        sysTime1->wMinute,
        sysTime1->wSecond);
    CTime t2(
        tmpSysTime.wYear, 
        tmpSysTime.wMonth, 
        tmpSysTime.wDay,
        tmpSysTime.wHour,
        tmpSysTime.wMinute,
        tmpSysTime.wSecond);
    CTimeSpan span=t2-t1;
    diffTime->days    = span.GetDays();
    diffTime->hours   = span.GetTotalHours();
    diffTime->minutes = span.GetTotalMinutes();
    diffTime->seconds = span.GetTotalSeconds();
#endif
}

BOOL PQ61850Utility::IsLeapYear(const int year )
{
    if ( (year % 4 == 0 && year % 100 != 0)
        || year % 400 == 0 )
    {
        return TRUE;
    }
    return FALSE;
}

void PQ61850Utility::StSubMilliSecond( const int milliSeconds, SYSTEMTIME *sysTime )
{
    if ( sysTime->wMilliseconds < milliSeconds )
    {
        sysTime->wMilliseconds = sysTime->wMilliseconds + 1000 - milliSeconds;
        StSubSecond(1);
    }
    else
    {
        sysTime->wMilliseconds = sysTime->wMilliseconds - milliSeconds;
    }
}

void PQ61850Utility::StSubSecond( const int seconds, SYSTEMTIME *sysTime )
{
    if ( sysTime->wSecond < seconds )
    {
        sysTime->wSecond = sysTime->wSecond + 60 - seconds;
        StSubMinute(1);
    }
    else
    {
        sysTime->wSecond = sysTime->wSecond - seconds;
    }
}

void PQ61850Utility::StSubMinute( const int minutes, SYSTEMTIME *sysTime )
{
    if ( sysTime->wMinute < minutes )
    {
        sysTime->wMinute = sysTime->wMinute + 60 - minutes;
        StSubHour(1);
    }
    else
    {
        sysTime->wMinute = sysTime->wMinute - minutes;
    }
}

void PQ61850Utility::StSubHour( const int hours, SYSTEMTIME *sysTime )
{
    if ( sysTime->wHour < hours )
    {
        sysTime->wHour = sysTime->wHour + 60 - hours;
        StSubDay(1);
    }
    else
    {
        sysTime->wHour = sysTime->wHour - hours;
    }
}

void PQ61850Utility::StSubDay( const int days, SYSTEMTIME *sysTime )
{
    static int monthDaysOfLeapYear[] = 
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30 
    };
    static int monthDaysOfUnLeapYear[] = 
    {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 
    };
    if ( sysTime->wDay < days )
    {
        if ( IsLeapYear(sysTime->))
        {
        }
    }
}

void PQ61850Utility::StSubMonth( const int months, SYSTEMTIME *sysTime )
{

}

void PQ61850Utility::StSubYear( const int years, SYSTEMTIME *sysTime )
{

}