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
  \brief 控件布局类,主要作用列举如下：
         1. 读取XML脚本，分析脚本构建控件树
		 2. 创建控件对象
 */
class UILIB_API CDialogBuilder
{
public:
    
	CDialogBuilder();
    
	/**
	  \brief 构建UI
	  \param[in] xml        可以是一个内存xml数据或一个文件名
      \param[in] type       如果xml数据位于EXE资源中则type指定资源类型
	  \param[in] pCallBack  回调函数
	  \param[in] pManager   绘制管理者指针
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
	  \brief 解析xml
	 */
    CControlUI* _Parse( CMarkupNode* parent, 
		CControlUI* pParent = NULL, 
		CPaintManagerUI* pManager = NULL);

	/**
	  \brief CMarkup对象
	 */
    CMarkup m_xml;

	/**
	  \brief 构建控件回调
	 */
    IDialogBuilderCallback* m_pCallback;
    
	LPCTSTR m_pstrtype;
};

} // namespace DuiLib

#endif // __UIDLGBUILDER_H__
