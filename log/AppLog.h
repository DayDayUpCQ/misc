/**
  \file AppLog.h

  \brief Ӧ�ó���LOG��ӡ

  \copyright ������Զ���ӹɷ����޹�˾�����
 */

#ifndef _APPLOG_H_
#define _APPLOG_H_

#include <tchar.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
using namespace std;

/**
  \brief ����
 */
#define APPLOG_MODULE       _T("[IEC61850] ")

/**
  \brief ������Ϣ
 */
#define APPLOG_APPEND       _T(" ")

/**
  \brief ���泤��
 */
#define APPLOG_BUFFER_SIZE  (256)

/**
  \brief Ӧ�ó���LOG
 */
#if 1
#define APP_LOG( swzFormat, ... )\
    {\
    wchar_t swzBuffer[APPLOG_BUFFER_SIZE];\
    int idx = 0; \
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE, _T("%s "), APPLOG_MODULE );\
    idx += _tcslen(APPLOG_MODULE);\
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE-idx, _T("%s "), APPLOG_APPEND );\
    idx += _tcslen(APPLOG_APPEND);\
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE - idx, swzFormat, __VA_ARGS__ );\
    OutputDebugString(swzBuffer);\
    }
#else
#define APP_LOG(swzFormat, ... ) 
#endif

/**
  \brief Ӧ�ó���DEBUG��Ϣ
 */
#ifdef _DEBUG
#define APP_DBG_LOG(swzFormat, ... )\
    {\
    wchar_t swzBuffer[APPLOG_BUFFER_SIZE];\
    int idx = 0;\
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE, _T("%s "), APPLOG_MODULE );\
    idx += _tcslen(APPLOG_MODULE);\
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE-idx, _T("%s "), APPLOG_APPEND );\
    idx += _tcslen(APPLOG_APPEND);\
    swprintf_s( swzBuffer+idx, APPLOG_BUFFER_SIZE - idx, swzFormat, __VA_ARGS__ );\
    OutputDebugString(swzBuffer);\
    }
#else
#define APP_DBG_LOG(swzFormat, ... ) 
#endif

#if 0
class APP_STEP_LOG
{
public:
	APP_STEP_LOG()
	{
		m_isFuncLog = FALSE;
	}

	static int GetStep()
	{
		return s_callStep++;
	}

	APP_STEP_LOG( WCHAR *funcName )
	{
		m_isFuncLog = TRUE;
		m_wstrFuncName = funcName;
		APP_LOG(_T("Step %d: Enter %s \r\n"), s_callStep++, m_wstrFuncName.c_str() );
	}

	~APP_STEP_LOG()
	{
		if ( m_isFuncLog )
		{
			APP_LOG(_T("Step %d: Exit %s \r\n"), s_callStep++, m_wstrFuncName.c_str() );
		}
	}

private:

	/**
	  \brief ��������
	 */
	wstring m_wstrFuncName;

	/**
	  \brief ��ʾLOG�Ƿ�Ϊ��������LOG
	 */
	BOOL m_isFuncLog;

	/**
	  \brief ����ִ�в���
	 */
	static int s_callStep;
};

#else

class APP_STEP_LOG
{
public:
	APP_STEP_LOG()
	{

	}

	static int GetStep()
	{
		return 0;
	}

	APP_STEP_LOG( WCHAR *funcName )
	{
	}

	~APP_STEP_LOG()
	{
	}

private:

	/**
	  \brief ����ִ�в���
	 */
	static int s_callStep;
};

#endif

#endif