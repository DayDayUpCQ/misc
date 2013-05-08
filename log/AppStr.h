/**
  \file AppStr.h

  \brief Ӧ�ó����ַ���������
 */
#ifndef _APPSTR_H_
#define _APPSTR_H_
#include <windows.h>

/**
  \brief ASCII�ַ�������
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
      \brief խ�ֽ��ַ���
     */
    CHAR m_szInfo[MAX_PATH];

    /**
      \brief ���ֽ��ַ���
     */
    WCHAR m_swzInfo[MAX_PATH];

    /**
      \brief �ַ�������
     */
    DWORD m_strLen;
};

/**
  \brief UNICODE�ַ�������
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
      \brief խ�ֽ��ַ���
     */
    CHAR m_szInfo[MAX_PATH];

    /**
      \brief ���ֽ��ַ���
     */
    WCHAR m_swzInfo[MAX_PATH];

    /**
      \brief �ַ�������
     */
    DWORD m_strLen;
};

/**
  \brief ���ַ�ת��Ϊȫ��д
  \param[in]  str �����ַ����Ļ���
  \param[out] str ת�����ȫ��д�ַ���
 */
extern void APPSTR_STR2UPPER( char *str );

#endif