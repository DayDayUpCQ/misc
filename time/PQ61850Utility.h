/**
  \file PQ61850Utility.h

  \brief ʵ�ú��� 
 */
#ifndef _PQ61850UTILITY_H_
#define _PQ61850UTILITY_H_
#include "..\ThirdPart\include\Rs61850Type.h"

/**
  \brief ���ڼ��ʱ���
 */
typedef struct  
{
    long long days;         /**< �� */
    long long hours;        /**< Сʱ */
    long long minutes;      /**< ���� */
    long long seconds;      /**< �� */
    long long milliSeconds; /**< ���� */
}PQ61850UTILITY_DIFFTIME;

class PQ61850Utility
{
public:
    PQ61850Utility();
    
    virtual ~PQ61850Utility();

    /**
      \brief ��ȡ������
     */
    static PQ61850Utility *GetSingleton();

    /**
      \brief SYSTEMTIMEת��ΪUTCʱ��
     */
    void St2UtcTime(const SYSTEMTIME *sysTime, UTCTIME *utcTime );

    /**
      \brief UTCʱ��ת��ΪSYSTEMTIME
     */
    void UtcTime2St( const UTCTIME *utcTime, SYSTEMTIME* sysTime );

    /**
      \brief UTCʱ��ת��ΪDTTIME
     */
    void UtcTime2Dt( const UTCTIME *utcTime, DTTIME  *dtTime);

    /**
      \brief DTTIMEת��ΪUTCʱ��
     */
    void Dt2UtcTime( const DTTIME  *dtTime,  UTCTIME *utcTime);

protected:

    /**
      \brief ������ϵͳʱ����ʱ���
      \param[in]  sysTime1 ϵͳʱ��1
      \param[in]  sysTime2 ϵͳʱ��2
      \param[out] diffTime ϵͳʱ��֮���ʱ���
     */
    void DiffSysTime( 
        const SYSTEMTIME *sysTime1, 
        const SYSTEMTIME *sysTime2, 
        PQ61850UTILITY_DIFFTIME *diffTime );

    /**
      \brief �ж�ָ���·��Ƿ�Ϊ����
     */
    BOOL IsLeapYear(const int year );

    /**
      \brief SYSTEMTIME��ȥָ������
      \param[in]  milliSeconds ���ȥ������(��С��1000)
      \param[out] sysTime      ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubMilliSecond( const int milliSeconds, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME��ȥָ����
      \param[in]  seconds ���ȥ������(��С�� 60)
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubSecond( const int seconds, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME��ȥָ������
      \param[in]  minutes ���ȥ�ķ�������(��С��60)
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubMinute( const int minutes, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME��ȥָ��Сʱ
      \param[in]  hours   ���ȥ��Сʱ(��С��24)
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubHour( const int hours, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME��ȥָ������(��С��28)
      \param[in]  days    ���ȥ������
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubDay( const int days, SYSTEMTIME *sysTime );

    /**
      \brief SYSTEMTIME��ȥָ����
      \param[in]  months  ���ȥ������(��С��12)
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubMonth( const int months, SYSTEMTIME *sysTime );
    
    /**
      \brief SYSTEMTIME��ȥָ����
      \param[in]  years   ���ȥ������
      \param[out] sysTime ��������ϵͳʱ��ʹ�����ϵͳʱ��
     */
    void StSubYear( const int years, SYSTEMTIME *sysTime);
};

#endif