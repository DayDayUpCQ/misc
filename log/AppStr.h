/**
  \file AppStr.h

  \brief Ó¦ÓÃ³ÌÐò×Ö·û·û´®´¦Àí
 */
#ifndef _APPSTR_H_
#define _APPSTR_H_
#include <windows.h>

/**
  \brief ASCII×Ö·û´®´¦Àí
 */
class APPSTR_ASCII
{
public:
    
    APPSTR_ASCII( CHAR *szInfo );

    ~APPSTR_ASCII();

    WCHAR *WCHAR_PTR();

    CHAR  *CHAR_PTR();

    DWORD  STR_LEN();

protected:

    void SZ2SWZ(void);

private:

    /**
      \brief Õ­×Ö½Ú×Ö·û´®
     */
    CHAR m_szInfo[MAX_PATH];

    /**
      \brief ¿í×Ö½Ú×Ö·û´®
     */
    WCHAR m_swzInfo[MAX_PATH];

    /**
      \brief ×Ö·û´®³¤¶È
     */
    DWORD m_strLen;
};

/**
  \brief UNICODE×Ö·û´®´¦Àí
 */
class APPSTR_WCHAR
{
public:
    
    APPSTR_WCHAR(WCHAR *swzInfo );

    ~APPSTR_WCHAR();

    WCHAR *WCHAR_PTR();

    CHAR *CHAR_PTR();

    DWORD  STR_LEN();

protected:

    void SWZ2SZ(void);

private:
    /**
      \brief Õ­×Ö½Ú×Ö·û´®
     */
    CHAR m_szInfo[MAX_PATH];

    /**
      \brief ¿í×Ö½Ú×Ö·û´®
     */
    WCHAR m_swzInfo[MAX_PATH];

    /**
      \brief ×Ö·û´®³¤¶È
     */
    DWORD m_strLen;
};

/**
  \brief ½«×Ö·û×ª»»ÎªÈ«´óÐ´
  \param[in]  str ±£´æ×Ö·û´®µÄ»º´æ
  \param[out] str ×ª»»ºóµÄÈ«´óÐ´×Ö·û´®
 */
extern void APPSTR_STR2UPPER( char *str );

#endif