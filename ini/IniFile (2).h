/****************************************Copyright (c)**************************************************
**						Guangzhou ZHIYUAN electronics Co.,LTD.
**
**							http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:				IniFile.h
** Last modified Date: 		2013.01.17
** Last Version:			V1.1
** Description: 			INI file operating class(INI文件操作类)
**
**------------------------------------------------------------------------------------------------------
** Modified By: 			liangyaozhan
** Modified date: 			2013.01.17
** Version: 				V1.1
** Descriptions:			使用list重新实现。 提高读写速度。具有写入延时功能。
**
**------------------------------------------------------------------------------------------------------
** Created By: 				Fuqiang Song 宋福强
** Created date: 			2007.12.16
** Version: 				V1.0
** Descriptions:			The original version 初始版本
**
**
********************************************************************************************************/
// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__5CB4DCA9_0F48_455E_A287_996CFCA5C91F__INCLUDED_)
#define AFX_INIFILE_H__5CB4DCA9_0F48_455E_A287_996CFCA5C91F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#include <list>
#include <afxmt.h>
using namespace std;

#undef USE_THREAD_TIMER
#ifdef USE_THREAD_TIMER
#include "CTimer.h"
class CIniFile:public CTimer
#else
class CIniFile
#endif
{
public:
	BOOL	LoadIniFile(const CString strFilePath, BOOL bCreate = FALSE);
	BOOL	ReadConfigString(CString strAppName, CString strKeyName, CString *pReturn, CString strDefault = _T(""));
	BOOL	ReadConfigInt(CString strAppName, CString strKeyName, int *pReturn, int nDefault = 0);
	BOOL	ReadConfigReal(CString strAppName, CString strKeyName, double *pReturn, double lfDefault = 0);
	BOOL	WriteConfigString(CString strAppName, CString strKeyName, CString strKeyValue);
	BOOL	WriteConfigInt(CString strAppName, CString strKeyName, int nKeyValue);
	BOOL	WriteConfigReal(CString strAppName, CString strKeyName, double lfKeyValue);
    BOOL	WriteConfigFloat(CString strAppName, CString strKeyName, float lfKeyValue);

    // Save data to disk.
    BOOL    Save( const CString &saveAsFilePath=CString(_T("")) );

	CIniFile();
	virtual	~CIniFile();

protected:
	CString	m_strFilePath;		/*	加载的INI文件的路径			*/
	CString	m_strFileBuf;		/*	加载的INI文件的文本内容		*/
    BOOL    Analysis( void );
    void    MakeDirty( void );

    typedef list<pair<CString, CString> > KEYMAP;
    typedef list<pair<CString, KEYMAP> > INIFILEMAP;

    INIFILEMAP::iterator SectionFind( const CString strAppName )
    {
        INIFILEMAP::iterator itApp;
        for (itApp=m_IniMap.begin(); itApp!=m_IniMap.end(); itApp++ )
        {
            if ( itApp->first == strAppName ) {
                break;
            }
        }
        return itApp;
    }
    KEYMAP::iterator ItemFind( INIFILEMAP::iterator itApp, const CString strKeyName )
    {
        KEYMAP::iterator itKey;
        for (itKey=itApp->second.begin(); itKey!=itApp->second.end(); itKey++ )
        {
            if ( itKey->first == strKeyName ) {
                break;
            }
        }
        return itKey;
    }


    INIFILEMAP   m_IniMap;
    CMutex       m_Lock;

#ifdef USE_THREAD_TIMER
    virtual void TimeProc( void *param );
#endif // USE_THREAD_TIMER
};

#endif // !defined(AFX_INIFILE_H__5CB4DCA9_0F48_455E_A287_996CFCA5C91F__INCLUDED_)
