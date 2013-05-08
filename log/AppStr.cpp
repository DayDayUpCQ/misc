/**
  \file AppStr.cpp

  \brief 应用程序字符串处理
 */
#include "..\include\AppStr.h"

// 窄字节字符串处理
APPSTR_ASCII::APPSTR_ASCII( CHAR *szInfo )
{
    m_strLen = strlen(szInfo);
    memcpy_s( m_szInfo, 
        MAX_PATH, 
        szInfo, 
        (m_strLen+1)*sizeof(CHAR));
    SZ2SWZ();
}

APPSTR_ASCII::~APPSTR_ASCII()
{

}

WCHAR * APPSTR_ASCII::WCHAR_PTR()
{
    return m_swzInfo;
}

CHAR  * APPSTR_ASCII::CHAR_PTR()
{
    return m_szInfo;
}

void APPSTR_ASCII::SZ2SWZ( void )
{
    MultiByteToWideChar (CP_ACP, 0, m_szInfo, -1, m_swzInfo, MAX_PATH);
}

DWORD APPSTR_ASCII::STR_LEN()
{
    return m_strLen;
}

// 宽字节字符串处理
APPSTR_WCHAR::APPSTR_WCHAR( WCHAR *swzInfo )
{
    m_strLen = wcslen(swzInfo);
    memcpy_s( (BYTE *)m_swzInfo, 
        MAX_PATH*sizeof(WCHAR),
        swzInfo,
        (m_strLen+1) * sizeof(WCHAR) );
    SWZ2SZ();
}
APPSTR_WCHAR::~APPSTR_WCHAR()
{

}

WCHAR * APPSTR_WCHAR::WCHAR_PTR()
{
    return m_swzInfo;
}

CHAR * APPSTR_WCHAR::CHAR_PTR()
{
    return m_szInfo;
}

void APPSTR_WCHAR::SWZ2SZ( void )
{
    WideCharToMultiByte( CP_OEMCP, NULL, 
        m_swzInfo, -1, m_szInfo, MAX_PATH, NULL, FALSE );
}

DWORD APPSTR_WCHAR::STR_LEN()
{
    return m_strLen;
}

void APPSTR_STR2UPPER( char *str )
{
    char *tmpPtr = str;
    while ( *tmpPtr != '\0' ) 
    { 
        if ( islower( *tmpPtr ))
        {
            *tmpPtr = (char )toupper( *tmpPtr );
        }
        tmpPtr++;
    }
}