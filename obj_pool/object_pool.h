/**
  \file object_pool.h

  \brief ��������Ķ��󻺴��

  \note ʹ�ø��ö���صĶ��󶼱���̳���ObjectPoolElem

  \version 1.2
 */
#ifndef _OBJECTPOOL_H_
#define _OBJECTPOOL_H_
extern "C"
{
    #include <assert.h>
    #include <stdio.h>
};

class ObjectPoolElem
{
public:

	ObjectPoolElem()
	{
		m_next = NULL;
	}
	
	explicit ObjectPoolElem( ObjectPoolElem *next )
	{
		m_next = next;
	}
	
	~ObjectPoolElem()
	{
		m_next = NULL;
	}
	
	ObjectPoolElem *GetNext(void)
	{
		return m_next;
	}
	
	void SetNext(ObjectPoolElem *next)
	{
		m_next = next;
	}
private:

        /**
          \brief ָ����һ������ص�Ԫ�ص�ָ��
         */
        ObjectPoolElem *m_next;
};

/**
  \brief ��������ʵ�ֵĶ����
  \note  �ö������̳���ObjectPoolElem
 */
template <class Type>
class ObjectPool
{
public:

    ObjectPool(int size) 
        : m_listHead(NULL), m_listTail(NULL), m_size(size)
    {
        InitPool(size);
    }

    /**
      \note ʹ�ø�ģ��ʱ,���뱣֤�ڸö�������ʱ����
            ��Ԫ�ض���ʹ��ReleaseElem()���ص��ö����
            ���������ڴ�й©
     */
    ~ObjectPool()
    {
        ExitPool();
    }

    /**
      \brief �Ӷ���ػ�ȡһ���Ѵ����õ�Ԫ��
     */
    Type *GetObj()
    {
        ObjectPoolElem *eleTmp = NULL;
        if ( m_listHead == NULL ) 
        {
            return NULL;
        }
        eleTmp = m_listHead;
        m_listHead = m_listHead->GetNext();
		if ( m_listHead == NULL )
		{
			m_listTail = NULL;
		}
        m_size --;
        return (Type *)eleTmp;
    }

    /**
      \brief ����ʹ�õ�Ԫ�ط��ص������
     */
    void ReleaseObj(ObjectPoolElem  *elemPtr )
    {
		elemPtr->SetNext(NULL);

		if ( m_listTail == NULL )
		{
			m_listHead = elemPtr;
			m_listTail = elemPtr;
		}
		else
		{
			m_listTail->SetNext(elemPtr);
			m_listTail = elemPtr;
		}
		m_size ++;
    }

	/**
	  \brief ��ȡ�������Ԫ�ظ���
	 */
	int GetSize()
	{
		return m_size;
	}

protected:
    /**
      \brief ��ʼ�������
     */
    void InitPool( int size )
    {
        Type *elemTmp = NULL;

        for ( int i = 0; i < size; i++ ) 
        {
            elemTmp = new Type();
            assert(elemTmp);
            if ( m_listHead == NULL ) 
            {
                m_listHead = (ObjectPoolElem *)elemTmp;
                m_listTail = (ObjectPoolElem *)elemTmp;
            }
            else
            {
                elemTmp->SetNext(m_listHead);
                m_listHead = (ObjectPoolElem *)elemTmp;
            }
        }
    }

    void ExitPool(void)
    {
        Type *eleTmp;
        while ( m_listHead != NULL )
        {
            eleTmp = (Type *)m_listHead;
            m_listHead = m_listHead->GetNext();
            delete eleTmp;
        }
        m_listTail = NULL;
    }
private:
    /**
      \brief ����ض��е�ͷָ��
     */
    ObjectPoolElem *m_listHead;

    /**
      \brief ����ض��е�βָ��
     */
    ObjectPoolElem *m_listTail;

    /**
      \brief ����ر������Ļ��浥Ԫ����
     */
    int m_size;
};
#endif