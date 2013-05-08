/**
  \file PQ61850Utility.h

  \brief 实用函数 
 */
#ifndef _PQ61850UTILITY_H_
#define _PQ61850UTILITY_H_
#include "..\ThirdPart\include\Rs61850Type.h"

/**
  \brief 日期间的时间差
 */
typedef struct  
{
    long long days;         /**< 天 */
    long long hours;        /**< 小时 */
    long long minutes;      /**< 分钟 */
    long long seconds;      /**< 秒 */
    long long milliSeconds; /**< 毫秒 */
}PQ61850UTILITY_DIFFTIME;

class PQ61850Utility
{
public:
    PQ61850Utility();
    
    virtual ~PQ61850Utility();

    /**
      \brief 获取对象单例
     */
    static PQ61850Utility *GetSingleton();

    /**
      \brief SYSTEMTIME转换为UTC时间
     */
    void St2UtcTime(const SYSTEMTIME *sysTime, UTCTIME *utcTime );

    /**
      \brief UTC时间转换为SYSTEMTIME
     */
    void UtcTime2St( const UTCTIME *utcTime, SYSTEMTIME* sysTime );

    /**
      \brief UTC时间转换为DTTIME
     */
    void UtcTime2Dt( const UTCTIME *utcTime, DTTIME  *dtTime);

    /**
      \brief DTTIME转换为UTC时间
     */
    void Dt2UtcTime( const DTTIME  *dtTime,  UTCTIME *utcTime);

protected:

    /**
      \brief 计算两系统时间间的时间差
      \param[in]  sysTime1 系统时间1
      \param[in]  sysTime2 系统时间2
      \param[out] diffTime 系统时间之间的时间差
     */
    void DiffSysTime( 
        const SYSTEMTIME *sysTime1, 
        const SYSTEMTIME *sysTime2, 
        PQ61850UTILITY_DIFFTIME *diffTime );

    /**
      \brief 判断指定月份是否为闰年
     */
    BOOL IsLeapYear(const int year );

    /**
      \brief SYSTEMTIME减去指定毫秒
      \param[in]  milliSeconds 需减去的秒数(需小于1000)
      \param[out] sysTime      待操作的系统时间和处理后的系统时间
     */
    void StSubMilliSecond( const int milliSeconds, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME减去指定秒
      \param[in]  seconds 需减去的秒数(需小于 60)
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubSecond( const int seconds, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME减去指定分钟
      \param[in]  minutes 需减去的分钟数据(需小于60)
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubMinute( const int minutes, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME减去指定小时
      \param[in]  hours   需减去的小时(需小于24)
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubHour( const int hours, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME减去指定天数(需小于28)
      \param[in]  days    需减去的天数
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubDay( const int days, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME减去指定月
      \param[in]  months  需减去的月数(需小于12)
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubMonth( const int months, SYSTEMTIME *sysTime );
    
    /**
      \brief SYSTEMTIME减去指定年
      \param[in]  years   需减去的年数
      \param[out] sysTime 待操作的系统时间和处理后的系统时间
     */
    void StSubYear( const int years, SYSTEMTIME *sysTime);
};

#endif