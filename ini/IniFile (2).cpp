/****************************************Copyright (c)**************************************************
**						Guangzhou ZHIYUAN electronics Co.,LTD.
**
**							http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:				IniFile.cpp
** Last modified Date: 		2013.01.17
** Last Version:			V1.1
** Description: 			INI file operating class(INI�ļ�������)
**
**------------------------------------------------------------------------------------------------------
** Modified By: 			liangyaozhan
** Modified date: 			2013.01.17
** Version: 				V1.1
** Descriptions:			ʹ��list����ʵ�֡� ��߶�д�ٶȡ�����д����ʱ���ܡ�
**
**------------------------------------------------------------------------------------------------------
** Created By: 				Fuqiang Song �θ�ǿ
** Created date: 			2007.12.16
** Version: 				V1.0
** Descriptions:			The original version ��ʼ�汾
**
**
********************************************************************************************************/

// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "IniFile.h"
#include <wchar.h>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(): 
#ifdef USE_THREAD_TIMER
CTimer(200), 
#endif
m_Lock()
{

}

CIniFile::~CIniFile()
{
}

#ifdef USE_THREAD_TIMER
void CIniFile::TimeProc( void *param )
{
    CTimer::TimeProc( param );
    Save();
}
#endif

void CIniFile::MakeDirty( void )
{
#ifdef USE_THREAD_TIMER
    CTimer::Reset();
#endif
}

BOOL CIniFile::Save( const CString &strFileName )
{
    BOOL ret = TRUE;

#ifdef USE_THREAD_TIMER
    CTimer::Stop();
#endif

    m_Lock.Lock();
    CFile file;
    INIFILEMAP::iterator sec;
    KEYMAP::iterator key;
    BOOL bRet;
    CString str;
    CString FileName;

    if ( strFileName == _T("") ) {
        FileName = m_strFilePath;
    } else {
        FileName = strFileName;
    }

    bRet = file.Open( FileName, CFile::modeCreate );
    if (!bRet)
    {
        ret = FALSE;
        goto done;
    }
    file.Close();
    bRet = file.Open( FileName, CFile::modeWrite );
    if (!bRet)
    {
        ret = FALSE;
        goto done;
    }
    if ( sizeof(TCHAR) == 2 ) {
        unsigned char head[3]={(unsigned char)0xff, (unsigned char)0xfe};
        file.Write( head, 2);
    }
    for (sec=m_IniMap.begin(); sec != m_IniMap.end(); sec++ )
    {
        // �ļ�ͷע��
        if ( sec->first != _T("[]") ) {
            str = _T("\r\n[")+sec->first +_T("]") + _T("\r\n");
            file.Write( str.GetBuffer(0), str.GetLength() * sizeof(TCHAR) );
        }
        for (key=sec->second.begin(); key!=sec->second.end(); key++ )
        {
            // ע�ͺͿ��б���������ʹ��_T(";")��ʾ
            // ��ˣ�д���ļ�ʱ��Ҫ��ע��д��ԭ����λ�á�
            if ( key->first == CString(_T(";")) ) {
                str = key->second + _T("\r\n");
                file.Write( str.GetBuffer(0), str.GetLength() * sizeof(TCHAR) );
                continue;
            }
            str = key->first + _T("=") + key->second + _T("\r\n");
            file.Write( str.GetBuffer(0), str.GetLength() * sizeof(TCHAR));
        }
    }

    file.Close();
done:
    m_Lock.Unlock();
    return ret;
}

/********************************************************************************************************
** Function name:           CIniFile::LoadIniFile()
** Descriptions:            ����INI�ļ��ı�����
** Input parameters:        strFilePath:	INI�ļ�·��
							bCreate:		��INI�ļ�������ʱ�Ƿ��½�
** Output parameters:		��
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::LoadIniFile(const CString strFilePath, BOOL bCreate)
{
	BOOL	bRet		= FALSE;										/*	����ֵ						*/
	DWORD	dwLength	= 0;											/*	�ļ�����					*/
	CFile	file;

#ifdef USE_THREAD_TIMER
    CTimer::Init();
    CTimer::Stop();
#endif

    INIFILEMAP::iterator itApp;
    for (itApp=m_IniMap.begin(); itApp!=m_IniMap.end(); itApp++ )
    {
        itApp->second.clear();
    }
    m_IniMap.clear();

	bRet = file.Open(strFilePath, CFile::modeRead);
	if (bRet) {															/*	�ж��ļ��Ƿ�򿪳ɹ�		*/
		m_strFilePath = strFilePath;
		dwLength = (DWORD)file.GetLength();
		BYTE *pBuf = new BYTE[dwLength + 2];
		if (!pBuf) {
			file.Close();
			return FALSE;
		}
		memset(pBuf, 0, dwLength + 2);
		file.Read(pBuf, dwLength);
		if ((pBuf[0] == 0xFF) && (pBuf[1] == 0xFE)) {					/*	�ж��ı��Ƿ�Ϊunicode����	*/
			m_strFileBuf = (LPCWSTR)(pBuf + 2);
		} else {
			m_strFileBuf = (LPCSTR)pBuf;
		}
		delete[] pBuf;
		file.Close();
	} else if (bCreate) {												/*	���ļ�ʧ��ʱ�ж��Ƿ��½�	*/
		m_strFileBuf = _T("");
		bRet = file.Open(strFilePath, CFile::modeCreate);
		if (bRet) {														/*	�ж��½��ļ��Ƿ�ɹ�		*/
			m_strFilePath = strFilePath;
			file.Close();
		} else {
			m_strFilePath = _T("");										/*	�½��ļ�ʧ��������ı�����	*/
		}
	}

    m_Lock.Lock();
    // analysis m_strFileBuf
    if ( Analysis() ) {
    }
    m_Lock.Unlock();
	return bRet;
}

/********************************************************************************************************
** Function name:           CIniFile::ReadConfigString()
** Descriptions:            ��ȡINI�ļ��������ַ���ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							strDefault:		������������ʧ��ʱ���͵�Ĭ��ֵ
** Output parameters:		*pReturn:		����INI�ļ��������ַ���ֵ
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::ReadConfigString(CString strAppName,
								CString strKeyName,
								CString *pReturn,
								CString strDefault)
{
    INIFILEMAP::iterator itApp;
    KEYMAP::iterator     itKey;

    m_Lock.Lock();
    itApp = SectionFind( strAppName );
    if ( itApp == m_IniMap.end() ) {
        goto errDone;
    }
    itKey = ItemFind( itApp, strKeyName );
    if ( itKey == itApp->second.end() ) {
        goto errDone;
    }
    *pReturn = itKey->second;
    m_Lock.Unlock();
    return TRUE;

errDone:
    m_Lock.Unlock();
    *pReturn = strDefault;
    return FALSE;
}


/********************************************************************************************************
** Function name:           CIniFile::ReadConfigInt()
** Descriptions:            ��ȡINI�ļ�����������ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							nDefault:		������������ʧ��ʱ���͵�Ĭ��ֵ
** Output parameters:		*pReturn:		����INI�ļ�����������ֵ
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::ReadConfigInt(CString  strAppName,
							 CString  strKeyName,
							 int     *pReturn,
							 int      nDefault)
{
	int		nReturn		 = nDefault;
	BOOL	bRet         = FALSE;
    CString strResult;

    if ( ReadConfigString( strAppName, strKeyName, &strResult, "") ) {
        _stscanf(strResult, _T("%ld"), &nReturn);						/*��������������ֵ�õ�������ֵ	*/
        bRet = TRUE;
    }
	*pReturn = nReturn;
	return bRet;
}

/********************************************************************************************************
** Function name:           CIniFile::ReadConfigReal()
** Descriptions:            ��ȡINI�ļ�������ʵ��(˫���ȸ�����)ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							lfDefault:		������������ʧ��ʱ���͵�Ĭ��ֵ
** Output parameters:		*pReturn:		����INI�ļ�������˫���ȸ�����ֵ
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::ReadConfigReal(CString  strAppName,
							  CString  strKeyName,
							  double  *pReturn,
							  double   lfDefault)
{
	double	lfReturn	 = lfDefault;
	BOOL	bRet		 = FALSE;
    CString strResult;

    if ( ReadConfigString( strAppName, strKeyName, &strResult, "") ) {
        _stscanf(strResult, _T("%lf"), &lfReturn);						/*��������������ֵ�õ�������ֵ	*/
        bRet = TRUE;
    }
	*pReturn = lfReturn;
	return bRet;
}

BOOL CIniFile::Analysis( void )
{
    TCHAR       *wstrContext = NULL;
    TCHAR       *wstrToken;
    CString      appName = _T("[]");;
    const TCHAR *wstrDelimit = _T("\r\n");
    TCHAR       *equalPosition;
    KEYMAP       CurrentSection;

    // getline
    if ( sizeof(TCHAR)>1 ) {
        wstrToken = (TCHAR*)(void*)wcstok_s( (wchar_t*)(void*)m_strFileBuf.GetBuffer(0),
                                             (const wchar_t*)(const void*)wstrDelimit,
                                             (wchar_t **)(void*)&wstrContext );
    } else {
        wstrToken = (TCHAR*)(void*)strtok_s( (char*)(void*)m_strFileBuf.GetBuffer(0),
                                             (const char*)(const void*)wstrDelimit,
                                             (char**)(void*)&wstrContext );
    }
    while (wstrToken) {
        // skip all spaces: L' ' L'\t';
        int n;
        if ( sizeof(TCHAR) > 1 ) {
            n = wcsspn( wstrToken, _T(" \t") );
        } else {
            n = strspn( (char*)(void*)wstrToken, " \t" );
        }

        wstrToken += n;
        //if ( wstrToken[0] == L';' ) {
        if ( wstrToken[0] == _T(';') ) {
            // ����ԭ�е�ע�͵����ݡ�
            CString temp = _T(";");
            CString value = wstrToken - n;
            CurrentSection.push_back( make_pair(temp, value) );
        } else if ( wstrToken[0] == _T('[') ) {
            TCHAR *wstrEnd;
            if ( sizeof(TCHAR)>1 ) {
                wstrEnd = (TCHAR*)(void*)wcschr( (wchar_t*)(void*)wstrToken, L']' );
            } else {
                wstrEnd = (TCHAR*)(void*)strchr( (char*)(void*)wstrToken, ']' );
            }
            if ( NULL == wstrEnd ) {
                // missing ']'
                return FALSE;
            }

            if ( appName.GetLength() ) {
                m_IniMap.push_back( make_pair( appName, CurrentSection) );
            } else {
                // �ļ�ͷע��
                m_IniMap.push_back( make_pair( appName, CurrentSection) );
            }

            *wstrEnd = _T('\0');
            appName = wstrToken + 1;
            CurrentSection.clear();
        } else if ( ( (sizeof(TCHAR)>1)    && (NULL != (equalPosition=(TCHAR*)(void*)wcschr( (wchar_t*)(void*)wstrToken, L'=' )))) ||
                    ( (sizeof(TCHAR) == 1) && (NULL != (equalPosition=(TCHAR*)(void*)strchr( (char*)(void*)wstrToken, '=' )) ) ) ) {
            *equalPosition = _T('\0');
            TCHAR *p = equalPosition;
            while ( p>wstrToken && (*--p == _T(' ') ) ) {
                *p = _T('\0');
            }
            CString key=wstrToken;
            for (p=equalPosition+1; *p!=_T('\0')&&*p==_T(' '); p++) {
            }
            CString value=p;
            CurrentSection.push_back( make_pair(key, value) );
        } else {
            // ����ԭ�е����ݡ�
            CString temp = _T(";");
            CString value = wstrToken - n;
            CurrentSection.push_back( make_pair(temp, value) );
        }
        // get next line
        if ( sizeof(TCHAR)>1 ) {
            wstrToken = (TCHAR*)(void*)wcstok_s( NULL,
                                                 (const wchar_t*)(const void*)wstrDelimit,
                                                 (wchar_t **)(void*)&wstrContext );
        } else {
            wstrToken = (TCHAR*)(void*)strtok_s( NULL,
                                                 (const char*)(const void*)wstrDelimit,
                                                 (char**)(void*)&wstrContext );
        }
    }
    if ( appName.GetLength() ) {
        m_IniMap.push_back( make_pair( appName, CurrentSection) );
    }
    return true;
}

/********************************************************************************************************
** Function name:           CIniFile::WriteConfigString()
** Descriptions:            ��INI�ļ�������д���ַ���ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							strKeyValue:	Ҫд����ַ���
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::WriteConfigString(CString strAppName,
								 CString strKeyName,
								 CString strKeyValue)
{
    INIFILEMAP::iterator itApp;
    KEYMAP::iterator     itKey;

    m_Lock.Lock();
    //itApp = m_IniMap.find( strAppName );
    itApp = SectionFind( strAppName );
    if ( itApp == m_IniMap.end() ) {
        // create new.
        KEYMAP km;
        km.push_back( make_pair(strKeyName, strKeyValue) );
        m_IniMap.push_back( make_pair( strAppName, km ) );
        goto done;
    }
    //itKey = itApp->second.find( strKeyName );
    itKey = ItemFind( itApp, strKeyName );
    if ( itKey == itApp->second.end() ) {
        // create new
        itApp->second.push_back( make_pair(strKeyName, strKeyValue) );
        goto done;
    }
    itKey->second = strKeyValue;

done:
    m_Lock.Unlock();

    MakeDirty();

    return true;
}


/********************************************************************************************************
** Function name:           CIniFile::WriteConfigString()
** Descriptions:            ��INI�ļ�������д������ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							nKeyValue:		Ҫд�������ֵ
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::WriteConfigInt(CString strAppName,
							  CString strKeyName,
							  int     nKeyValue)
{
	CString	strKeyValue;

	strKeyValue.Format(_T("%d"), nKeyValue);
	return WriteConfigString(strAppName, strKeyName, strKeyValue);
}

/********************************************************************************************************
** Function name:           CIniFile::WriteConfigString()
** Descriptions:            ��INI�ļ�������д��ʵ��(˫���ȸ�����)ֵ
** Input parameters:        strAppName:		INI�ļ�����
							strKeyName:		INI�ļ���������
							lfKeyValue:		Ҫд���˫���ȸ�����ֵ
** Returned value:			TRUE: �ɹ�	FALSE: ʧ��
*********************************************************************************************************/
BOOL CIniFile::WriteConfigReal(CString strAppName,
							   CString strKeyName,
							   double  lfKeyValue)
{
	CString	strKeyValue;

	strKeyValue.Format(_T("%.5G"), lfKeyValue);
	return WriteConfigString(strAppName, strKeyName, strKeyValue);
}

BOOL CIniFile::WriteConfigFloat(CString strAppName, CString strKeyName, float lfKeyValue)
{

    CString	strKeyValue;

    strKeyValue.Format(_T("%.5f"), lfKeyValue);
    return WriteConfigString(strAppName, strKeyName, strKeyValue);
}
