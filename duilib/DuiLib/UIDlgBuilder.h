#ifndef __UIDLGBUILDER_H__
#define __UIDLGBUILDER_H__

#pragma once

namespace DuiLib {

class IDialogBuilderCallback
{
public:
    virtual CControlUI* CreateControl(LPCTSTR pstrClass) = 0;
};

/**
  \brief �ؼ�������,��Ҫ�����о����£�
         1. ��ȡXML�ű��������ű������ؼ���
		 2. �����ؼ�����
 */
class UILIB_API CDialogBuilder
{
public:
    
	CDialogBuilder();
    
	/**
	  \brief ����UI
	  \param[in] xml        ������һ���ڴ�xml���ݻ�һ���ļ���
      \param[in] type       ���xml����λ��EXE��Դ����typeָ����Դ����
	  \param[in] pCallBack  �ص�����
	  \param[in] pManager   ���ƹ�����ָ��
	 */
	CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
        CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);
    
	CControlUI* Create(IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
        CControlUI* pParent = NULL);

    CMarkup* GetMarkup();

    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;

    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

private:

	/**
	  \brief ����xml
	 */
    CControlUI* _Parse( CMarkupNode* parent, 
		CControlUI* pParent = NULL, 
		CPaintManagerUI* pManager = NULL);

	/**
	  \brief CMarkup����
	 */
    CMarkup m_xml;

	/**
	  \brief �����ؼ��ص�
	 */
    IDialogBuilderCallback* m_pCallback;
    
	LPCTSTR m_pstrtype;
};

} // namespace DuiLib

#endif // __UIDLGBUILDER_H__
